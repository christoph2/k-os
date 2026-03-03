#pragma once

#include <filesystem>
#include <istream>
#include <set>
#include <string>
#include <vector>

namespace kos::oilcpp {

class Preprocessor {
public:
    explicit Preprocessor(std::vector<std::filesystem::path> include_paths = {});

    /// Preprocess an input OIL stream, resolving #include and USE directives.
    /// Returns the flattened text. Throws std::runtime_error on failure.
    std::string process(const std::filesystem::path& input_path);

private:
    std::string processStream(std::istream& in,
                              const std::filesystem::path& logical_path,
                              std::set<std::filesystem::path>& include_guard);

    std::filesystem::path resolveInclude(const std::filesystem::path& base,
                                         const std::string& target,
                                         bool quoted) const;

    std::vector<std::filesystem::path> include_paths_;
};

}  // namespace kos::oilcpp
