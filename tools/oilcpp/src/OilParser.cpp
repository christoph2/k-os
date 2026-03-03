#include "OilParser.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

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

std::optional<double> parseNumeric(const std::string& value) {
    auto t = trim(value);
    if (t.empty()) return std::nullopt;
    try {
        size_t idx = 0;
        if (t.rfind("0x", 0) == 0 || t.rfind("0X", 0) == 0) {
            auto v = std::stoll(t, &idx, 0);
            if (idx == t.size()) return static_cast<double>(v);
            return std::nullopt;
        }
        auto v = std::stod(t, &idx);
        if (idx == t.size()) return v;
    } catch (...) {
    }
    return std::nullopt;
}

bool isNumber(const std::string& value) {
    return parseNumeric(value).has_value();
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

std::set<std::string> extractAttributes(const std::string& raw) {
    std::set<std::string> attrs;
    size_t stmt_start = 0;
    int depth = 0;
    for (size_t i = 0; i < raw.size(); ++i) {
        char ch = raw[i];
        if (ch == '{') {
            ++depth;
        } else if (ch == '}') {
            if (depth > 0) --depth;
        } else if (ch == ';' && depth == 0) {
            auto stmt = trim(std::string_view(raw).substr(stmt_start, i - stmt_start));
            stmt_start = i + 1;
            if (stmt.empty()) continue;
            auto eq = stmt.find('=');
            if (eq != std::string::npos) {
                stmt = trim(stmt.substr(0, eq));
            }
            size_t end = stmt.size();
            while (end > 0 && std::isspace(static_cast<unsigned char>(stmt[end - 1]))) --end;
            size_t pos = end;
            while (pos > 0 && (std::isalnum(static_cast<unsigned char>(stmt[pos - 1])) || stmt[pos - 1] == '_')) --pos;
            if (pos < end) {
                attrs.insert(stmt.substr(pos, end - pos));
            }
        }
    }
    return attrs;
}

enum class ConstraintKind { kUnknown, kBoolean, kEnum, kInteger, kFloat, kString };

struct AttributeConstraint {
    ConstraintKind kind{ConstraintKind::kUnknown};
    bool allow_auto{false};
    bool is_array{false};
    std::string type_token;
    std::optional<std::string> reference_target;
    std::vector<std::string> enum_values;
    std::vector<double> numeric_values;
    std::optional<double> min_value;
    std::optional<double> max_value;
};

std::vector<std::string> splitTopLevel(const std::string& input, char delimiter) {
    std::vector<std::string> parts;
    size_t start = 0;
    int depth = 0;
    bool in_string = false;
    for (size_t i = 0; i < input.size(); ++i) {
        char ch = input[i];
        if (ch == '"' && (i == 0 || input[i - 1] != '\\')) {
            in_string = !in_string;
        }
        if (in_string) continue;
        if (ch == '{') ++depth;
        else if (ch == '}' && depth > 0) --depth;
        else if (ch == delimiter && depth == 0) {
            parts.push_back(trim(std::string_view(input).substr(start, i - start)));
            start = i + 1;
        }
    }
    if (start <= input.size()) {
        parts.push_back(trim(std::string_view(input).substr(start)));
    }
    return parts;
}

std::optional<std::pair<std::string, std::string>> extractRange(const std::string& content) {
    int depth = 0;
    bool in_string = false;
    for (size_t i = 0; i + 1 < content.size(); ++i) {
        char ch = content[i];
        if (ch == '"' && (i == 0 || content[i - 1] != '\\')) {
            in_string = !in_string;
        }
        if (in_string) continue;
        if (ch == '{') ++depth;
        else if (ch == '}' && depth > 0) --depth;
        else if (ch == '.' && content[i + 1] == '.' && depth == 0) {
            auto left = trim(std::string_view(content).substr(0, i));
            auto right = trim(std::string_view(content).substr(i + 2));
            return std::make_pair(left, right);
        }
    }
    return std::nullopt;
}

std::string leadingToken(const std::string& value) {
    size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) ++start;
    size_t end = start;
    while (end < value.size() &&
           (std::isalnum(static_cast<unsigned char>(value[end])) || value[end] == '_' || value[end] == '+' ||
            value[end] == '-')) {
        ++end;
    }
    if (end <= start) return {};
    return value.substr(start, end - start);
}

std::optional<std::pair<std::string, AttributeConstraint>> parseAttributeConstraint(const std::string& stmt_raw) {
    auto stmt = trim(stmt_raw);
    if (stmt.empty()) return std::nullopt;

    size_t pos = 0;
    while (pos < stmt.size() && !std::isspace(static_cast<unsigned char>(stmt[pos])) && stmt[pos] != '[') ++pos;
    std::string type_token = stmt.substr(0, pos);
    if (type_token.empty()) return std::nullopt;

    AttributeConstraint constraint;
    constraint.type_token = type_token;
    std::string type_upper = type_token;
    std::transform(type_upper.begin(), type_upper.end(), type_upper.begin(), [](unsigned char c) { return std::toupper(c); });
    if (type_upper == "BOOLEAN") {
        constraint.kind = ConstraintKind::kBoolean;
    } else if (type_upper == "ENUM") {
        constraint.kind = ConstraintKind::kEnum;
    } else if (type_upper == "STRING") {
        constraint.kind = ConstraintKind::kString;
    } else if (type_upper.rfind("UINT", 0) == 0 || type_upper.rfind("INT", 0) == 0) {
        constraint.kind = ConstraintKind::kInteger;
    } else if (type_upper.rfind("FLOAT", 0) == 0) {
        constraint.kind = ConstraintKind::kFloat;
    } else {
        return std::nullopt;
    }

    while (pos < stmt.size() && std::isspace(static_cast<unsigned char>(stmt[pos]))) ++pos;
    if (stmt.compare(pos, 8, "WITH_AUTO") == 0) {
        constraint.allow_auto = true;
        pos += 8;
    }
    while (pos < stmt.size() && std::isspace(static_cast<unsigned char>(stmt[pos]))) ++pos;

    std::string bracket_content;
    if (pos < stmt.size() && stmt[pos] == '[') {
        int bracket_depth = 0;
        size_t start = pos;
        for (; pos < stmt.size(); ++pos) {
            char ch = stmt[pos];
            if (ch == '[') ++bracket_depth;
            else if (ch == ']') {
                if (--bracket_depth == 0) {
                    bracket_content = stmt.substr(start + 1, pos - start - 1);
                    ++pos;
                    break;
                }
            }
        }
    }

    while (pos < stmt.size() && std::isspace(static_cast<unsigned char>(stmt[pos]))) ++pos;
    size_t name_start = pos;
    while (name_start < stmt.size() &&
           !(std::isalpha(static_cast<unsigned char>(stmt[name_start])) || stmt[name_start] == '_')) {
        ++name_start;
    }
    size_t name_end = name_start;
    while (name_end < stmt.size() &&
           (std::isalnum(static_cast<unsigned char>(stmt[name_end])) || stmt[name_end] == '_')) {
        ++name_end;
    }
    if (name_end <= name_start) return std::nullopt;
    std::string name = stmt.substr(name_start, name_end - name_start);

    size_t after_name = name_end;
    while (after_name < stmt.size() && std::isspace(static_cast<unsigned char>(stmt[after_name]))) ++after_name;
    if (after_name + 1 < stmt.size() && stmt[after_name] == '[' && stmt[after_name + 1] == ']') {
        constraint.is_array = true;
    }

    if (!bracket_content.empty()) {
        if ((constraint.kind == ConstraintKind::kInteger || constraint.kind == ConstraintKind::kFloat)) {
            if (auto range = extractRange(bracket_content)) {
                auto min_v = parseNumeric(range->first);
                auto max_v = parseNumeric(range->second);
                if (min_v && max_v) {
                    constraint.min_value = *min_v;
                    constraint.max_value = *max_v;
                }
            } else {
                auto parts = splitTopLevel(bracket_content, ',');
                for (const auto& part : parts) {
                    auto tok = leadingToken(part);
                    if (tok.empty()) continue;
                    if (auto num = parseNumeric(tok)) {
                        constraint.numeric_values.push_back(*num);
                    }
                }
            }
        } else {
            auto parts = splitTopLevel(bracket_content, ',');
            for (const auto& part : parts) {
                auto tok = leadingToken(part);
                if (!tok.empty()) constraint.enum_values.push_back(tok);
            }
        }
    }

    if (constraint.kind == ConstraintKind::kBoolean && constraint.enum_values.empty()) {
        constraint.enum_values = {"TRUE", "FALSE", "STD_ON", "STD_OFF"};
    }

    if (type_upper.size() > 5 && type_upper.rfind("_TYPE", type_upper.size() - 5) != std::string::npos) {
        auto base = type_upper.substr(0, type_upper.size() - 5);
        constraint.reference_target = base;
    }

    return std::make_pair(std::move(name), std::move(constraint));
}

std::unordered_map<std::string, AttributeConstraint> parseAttributeConstraints(const std::string& raw) {
    std::unordered_map<std::string, AttributeConstraint> constraints;
    size_t stmt_start = 0;
    int depth = 0;
    for (size_t i = 0; i < raw.size(); ++i) {
        char ch = raw[i];
        if (ch == '{') ++depth;
        else if (ch == '}') {
            if (depth > 0) --depth;
        } else if (ch == ';' && depth == 0) {
            auto stmt = trim(std::string_view(raw).substr(stmt_start, i - stmt_start));
            stmt_start = i + 1;
            if (stmt.empty()) continue;
            if (auto parsed = parseAttributeConstraint(stmt)) {
                constraints[parsed->first] = std::move(parsed->second);
            }
        }
    }
    return constraints;
}

std::vector<std::string> parseIdentifierList(const std::string& raw) {
    auto t = trim(raw);
    if (t.empty()) return {};
    if (t.front() == '{' && t.back() == '}') {
        t = trim(std::string_view(t).substr(1, t.size() - 2));
    }
    if (t.empty()) return {};
    auto parts = splitTopLevel(t, ',');
    std::vector<std::string> out;
    for (auto& p : parts) {
        auto tok = leadingToken(p);
        if (!tok.empty()) out.push_back(tok);
    }
    if (out.empty() && !t.empty()) {
        out.push_back(t);
    }
    return out;
}

void validateAttributeValue(const Attribute& attr,
                            const std::unordered_map<std::string, AttributeConstraint>& constraints,
                            const std::unordered_map<std::string, std::set<std::string>>& object_names,
                            const std::string& object_type, const std::string& object_name) {
    auto it = constraints.find(attr.name);
    if (it == constraints.end()) return;
    const auto& constraint = it->second;

    auto fail = [&](const std::string& reason) {
        std::ostringstream oss;
        oss << "Invalid value for " << attr.name << " in " << object_type << " " << object_name << ": " << reason;
        throw std::runtime_error(oss.str());
    };

    if (attr.value.kind == AttributeValue::Kind::kAuto) {
        if (!constraint.allow_auto) {
            fail("AUTO not allowed");
        }
        return;
    }

    switch (constraint.kind) {
    case ConstraintKind::kBoolean:
        if (attr.value.kind != AttributeValue::Kind::kBoolean) {
            fail("expected boolean");
        }
        if (!constraint.enum_values.empty() &&
            std::find(constraint.enum_values.begin(), constraint.enum_values.end(), attr.value.text) ==
                constraint.enum_values.end()) {
            fail("unexpected boolean value");
        }
        break;
    case ConstraintKind::kEnum:
        if (!constraint.enum_values.empty() &&
            std::find(constraint.enum_values.begin(), constraint.enum_values.end(), attr.value.text) ==
                constraint.enum_values.end()) {
            fail("expected one of the declared enum values");
        }
        break;
    case ConstraintKind::kInteger:
    case ConstraintKind::kFloat: {
        auto num = parseNumeric(attr.value.text);
        if (!num) {
            fail("expected numeric value");
        }
        if (constraint.kind == ConstraintKind::kInteger) {
            auto rounded = std::llround(*num);
            if (std::fabs(*num - static_cast<double>(rounded)) > 1e-9) {
                fail("expected integer value");
            }
        }
        if (constraint.min_value && *num < *constraint.min_value) {
            fail("below minimum");
        }
        if (constraint.max_value && *num > *constraint.max_value) {
            fail("above maximum");
        }
        if (!constraint.numeric_values.empty()) {
            bool match = false;
            for (double allowed : constraint.numeric_values) {
                if (std::fabs(*num - allowed) < 1e-9) {
                    match = true;
                    break;
                }
            }
            if (!match) {
                fail("unexpected numeric value");
            }
        }
        break;
    }
    case ConstraintKind::kString:
        if (attr.value.kind != AttributeValue::Kind::kString) {
            fail("expected string literal");
        }
        break;
    default:
        break;
    }

    if (constraint.reference_target) {
        auto target_it = object_names.find(*constraint.reference_target);
        if (target_it == object_names.end()) {
            fail("no objects of referenced type present");
        }
        const auto& names = target_it->second;
        if (constraint.is_array) {
            auto list = parseIdentifierList(attr.value.text);
            for (const auto& item : list) {
                if (names.find(item) == names.end()) {
                    fail("unknown referenced object '" + item + "'");
                }
            }
        } else {
            auto token = leadingToken(attr.value.text);
            if (!token.empty() && names.find(token) == names.end()) {
                fail("unknown referenced object '" + token + "'");
            }
        }
    }
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

    std::unordered_map<std::string, std::set<std::string>> object_names;
    for (const auto& inst : model.application_objects) {
        object_names[inst.object_type].insert(inst.name);
    }

    std::unordered_map<std::string, std::set<std::string>> allowed;
    std::unordered_map<std::string, std::unordered_map<std::string, AttributeConstraint>> constraints;
    for (const auto& spec : model.implementation_specs) {
        allowed[spec.object_type] = extractAttributes(spec.raw_definition);
        constraints[spec.object_type] = parseAttributeConstraints(spec.raw_definition);
    }
    for (const auto& inst : model.application_objects) {
        auto allowed_it = allowed.find(inst.object_type);
        auto constraint_it = constraints.find(inst.object_type);
        for (const auto& attr : inst.attributes) {
            if (allowed_it != allowed.end() && allowed_it->second.find(attr.name) == allowed_it->second.end()) {
                std::ostringstream oss;
                oss << "Unknown attribute '" << attr.name << "' in " << inst.object_type << " " << inst.name;
                throw std::runtime_error(oss.str());
            }
            if (constraint_it != constraints.end()) {
                validateAttributeValue(attr, constraint_it->second, object_names, inst.object_type, inst.name);
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
