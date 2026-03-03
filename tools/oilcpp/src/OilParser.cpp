#include "OilParser.hpp"

#include <algorithm>
#include <cctype>
#include <regex>
#include <stdexcept>
#include <unordered_map>
#include <string_view>
#include <set>

namespace kos::oilcpp {

namespace {

std::string trim(std::string_view sv) {
    size_t start = 0;
    while (start < sv.size() && std::isspace(static_cast<unsigned char>(sv[start]))) {
        ++start;
    }
    size_t end = sv.size();
    while (end > start && std::isspace(static_cast<unsigned char>(sv[end - 1]))) {
        --end;
    }
    return std::string(sv.substr(start, end - start));
}

bool isNumber(const std::string& value) {
    if (value.empty()) return false;
    if (value.rfind("0x", 0) == 0 || value.rfind("0X", 0) == 0) {
        return value.size() > 2 &&
               std::all_of(value.begin() + 2, value.end(), [](unsigned char c) { return std::isxdigit(c); });
    }
    return std::all_of(value.begin(), value.end(), [](unsigned char c) { return std::isdigit(c); });
}

AttributeValue parseValue(const std::string& raw) {
    AttributeValue val{};
    auto t = trim(raw);
    if (t == "TRUE" || t == "FALSE" || t == "STD_ON" || t == "STD_OFF") {
        val.kind = AttributeValue::Kind::kBoolean;
        val.text = t;
    } else if (t == "AUTO") {
        val.kind = AttributeValue::Kind::kAuto;
        val.text = t;
    } else if (!t.empty() && t.front() == '"' && t.back() == '"' && t.size() >= 2) {
        val.kind = AttributeValue::Kind::kString;
        val.text = t.substr(1, t.size() - 2);
    } else if (isNumber(t)) {
        val.kind = AttributeValue::Kind::kNumber;
        val.text = t;
    } else {
        val.kind = AttributeValue::Kind::kIdentifier;
        val.text = t;
    }
    return val;
}

}  // namespace

OilModel OilParser::parse(const std::string& preprocessed, const std::filesystem::path&) const {
    OilModel model{};
    model.preprocessed_text = preprocessed;

    std::regex version_re("OIL_VERSION\\s*=\\s*\"([^\"]+)\"");
    std::smatch m;
    if (std::regex_search(preprocessed, m, version_re) && m.size() > 1) {
        model.oil_version = m[1].str();
    } else {
        model.oil_version = "unknown";
    }

    model.implementation_specs = parseImplementation(preprocessed);

    static const std::regex obj_start_re(
        R"(\b(OS|TASK|COUNTER|ALARM|RESOURCE|EVENT|ISR|MESSAGE|COM|NM|APPMODE|IPDU|NETWORKMESSAGE)\s+([A-Za-z_]\w*)[^{};]*\{)");

    size_t search_pos = 0;
    while (search_pos < preprocessed.size()) {
        std::smatch m;
        auto begin_it = preprocessed.cbegin() + static_cast<std::ptrdiff_t>(search_pos);
        if (!std::regex_search(begin_it, preprocessed.cend(), m, obj_start_re)) {
            break;
        }
        size_t match_pos = static_cast<size_t>(m.position(0) + std::distance(preprocessed.cbegin(), begin_it));
        size_t match_end = match_pos + m.length(0);

        size_t brace_depth = 1;
        size_t body_end = match_end;
        for (; body_end < preprocessed.size(); ++body_end) {
            if (preprocessed[body_end] == '{') ++brace_depth;
            if (preprocessed[body_end] == '}') {
                if (--brace_depth == 0) {
                    break;
                }
            }
        }
        if (brace_depth != 0 || body_end > preprocessed.size()) {
            throw std::runtime_error("Unbalanced braces while parsing OIL object");
        }

        std::string body = preprocessed.substr(match_end, body_end - match_end);
        ObjectInstance inst;
        inst.object_type = m[1].str();
        inst.name = m[2].str();

        // Scan attributes with brace-awareness: consume until semicolon at depth 0.
        size_t pos = 0;
        while (pos < body.size()) {
            while (pos < body.size() && std::isspace(static_cast<unsigned char>(body[pos]))) ++pos;
            size_t name_start = pos;
            while (pos < body.size() && (std::isalnum(static_cast<unsigned char>(body[pos])) || body[pos] == '_')) ++pos;
            if (name_start == pos) { ++pos; continue; }
            std::string name = body.substr(name_start, pos - name_start);
            while (pos < body.size() && std::isspace(static_cast<unsigned char>(body[pos]))) ++pos;
            if (pos >= body.size() || body[pos] != '=') { ++pos; continue; }
            ++pos;
            while (pos < body.size() && std::isspace(static_cast<unsigned char>(body[pos]))) ++pos;
            size_t val_start = pos;
            int depth = 0;
            for (; pos < body.size(); ++pos) {
                char ch = body[pos];
                if (ch == '{') ++depth;
                else if (ch == '}') { if (depth > 0) --depth; }
                else if (ch == ';' && depth == 0) {
                    break;
                }
            }
            std::string raw_val = body.substr(val_start, pos - val_start);
            if (!name.empty()) {
                Attribute attr;
                attr.name = trim(name);
                attr.value = parseValue(raw_val);
                inst.attributes.push_back(std::move(attr));
            }
            // Skip past ';'
            if (pos < body.size() && body[pos] == ';') ++pos;
        }

        model.application_objects.push_back(std::move(inst));
        search_pos = body_end + 1;
    }

    // Handle simple declarations like "APPMODE Foo;"
    static const std::regex appmode_decl_re(R"(\bAPPMODE\s+([A-Za-z_]\w*)\s*;)");
    auto abegin = std::sregex_iterator(preprocessed.begin(), preprocessed.end(), appmode_decl_re);
    auto aend = std::sregex_iterator();
    for (auto it = abegin; it != aend; ++it) {
        if ((*it).size() > 1) {
            auto name = (*it)[1].str();
            bool exists = std::any_of(model.application_objects.begin(), model.application_objects.end(),
                                      [&](const ObjectInstance& obj) {
                                          return obj.object_type == "APPMODE" && obj.name == name;
                                      });
            if (!exists) {
                ObjectInstance inst;
                inst.object_type = "APPMODE";
                inst.name = name;
                model.application_objects.push_back(std::move(inst));
            }
        }
    }

    return model;
}

std::vector<ImplementationSpec> OilParser::parseImplementation(const std::string& text) const {
    std::vector<ImplementationSpec> specs;
    auto impl_pos = text.find("IMPLEMENTATION");
    if (impl_pos == std::string::npos) return specs;

    auto brace_pos = text.find('{', impl_pos);
    if (brace_pos == std::string::npos) return specs;

    size_t depth = 0;
    size_t end_pos = brace_pos;
    for (; end_pos < text.size(); ++end_pos) {
        if (text[end_pos] == '{') ++depth;
        else if (text[end_pos] == '}') {
            if (depth == 0) break;
            --depth;
            if (depth == 0) {
                break;
            }
        }
    }
    if (depth != 0 || end_pos >= text.size()) return specs;

    std::string body = text.substr(brace_pos + 1, end_pos - brace_pos - 1);
    size_t search_pos = 0;
    static const std::regex obj_re(R"(([A-Za-z_][A-Za-z0-9_]*)\s*\{)");
    while (search_pos < body.size()) {
        std::smatch m;
        auto begin_it = body.cbegin() + static_cast<std::ptrdiff_t>(search_pos);
        if (!std::regex_search(begin_it, body.cend(), m, obj_re)) break;
        size_t match_pos = search_pos + static_cast<size_t>(m.position());
        size_t block_start = match_pos + m.length() - 1;  // at '{'
        size_t block_depth = 0;
        size_t block_end = block_start;
        for (; block_end < body.size(); ++block_end) {
            if (body[block_end] == '{') ++block_depth;
            else if (body[block_end] == '}') {
                if (--block_depth == 0) {
                    break;
                }
            }
        }
        if (block_end >= body.size()) break;
        ImplementationSpec spec;
        spec.object_type = m[1].str();
        spec.raw_definition = body.substr(block_start + 1, block_end - block_start - 1);
        specs.push_back(std::move(spec));
        search_pos = block_end + 1;
    }
    return specs;
}

}  // namespace kos::oilcpp
