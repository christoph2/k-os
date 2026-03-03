#include "Preprocessor.hpp"

#include <cstdlib>
#include <fstream>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace kos::oilcpp {

namespace {

std::vector<fs::path> envIncludePaths() {
    std::vector<fs::path> paths;
    if (const char* env = std::getenv("KOS_INCLUDE")) {
        std::string val(env);
        std::size_t start = 0;
        while (true) {
            auto pos = val.find(';', start);
            auto part = val.substr(start, pos == std::string::npos ? std::string::npos : pos - start);
            if (!part.empty()) {
                paths.emplace_back(part);
            }
            if (pos == std::string::npos) break;
            start = pos + 1;
        }
    }
    return paths;
}

fs::path canonicalize(const fs::path& p) {
    try {
        return fs::weakly_canonical(p);
    } catch (...) {
        return p.lexically_normal();
    }
}

}  // namespace

Preprocessor::Preprocessor(std::vector<fs::path> include_paths) : include_paths_(std::move(include_paths)) {
    auto env_paths = envIncludePaths();
    include_paths_.insert(include_paths_.end(), env_paths.begin(), env_paths.end());
}

std::string Preprocessor::process(const fs::path& input_path) {
    std::ifstream in(input_path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open input file: " + input_path.string());
    }
    std::set<fs::path> include_guard;
    return processStream(in, fs::absolute(input_path), include_guard);
}

std::string Preprocessor::processStream(std::istream& in,
                                        const fs::path& logical_path,
                                        std::set<fs::path>& include_guard) {
    static const std::regex include_re("^\\s*#\\s*include\\s*([<\"])([^\">]+)([>\"]).*");
    static const std::regex use_re("^\\s*USE\\s*=\\s*\"([^\"]+)\"\\s*;.*");

    std::string output;
    std::string line;
    auto canonical_path = canonicalize(logical_path);
    if (!include_guard.insert(canonical_path).second) {
        return output;  // avoid infinite recursion on circular includes
    }

    while (std::getline(in, line)) {
        std::smatch m;
        if (std::regex_match(line, m, include_re)) {
            bool quoted = m[1].str() == "\"";
            auto target = m[2].str();
            auto resolved = resolveInclude(canonical_path.parent_path(), target, quoted);
            std::ifstream inc(resolved, std::ios::binary);
            if (!inc) {
                throw std::runtime_error("Cannot open include file: " + resolved.string());
            }
            output += processStream(inc, resolved, include_guard);
            continue;
        }
        if (std::regex_match(line, m, use_re)) {
            auto target = m[1].str();
            auto resolved = resolveInclude(canonical_path.parent_path(), target, true);
            std::ifstream inc(resolved, std::ios::binary);
            if (!inc) {
                throw std::runtime_error("Cannot open USE file: " + resolved.string());
            }
            output += processStream(inc, resolved, include_guard);
            continue;
        }
        output.append(line);
        output.push_back('\n');
    }
    include_guard.erase(canonical_path);
    return output;
}

fs::path Preprocessor::resolveInclude(const fs::path& base,
                                      const std::string& target,
                                      bool quoted) const {
    if (quoted) {
        fs::path local = base / target;
        if (fs::exists(local)) {
            return canonicalize(local);
        }
    }
    for (const auto& p : include_paths_) {
        fs::path candidate = p / target;
        if (fs::exists(candidate)) {
            return canonicalize(candidate);
        }
    }
    return canonicalize(base / target);  // last attempt; will fail later if missing
}

}  // namespace kos::oilcpp
