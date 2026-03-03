#pragma once

#include <filesystem>

#include "OilAst.hpp"

namespace kos::oilcpp {

class OilParser {
public:
    OilParser() = default;

    /// Parse preprocessed OIL text into an OilModel (hand-rolled, no ANTLR).
    OilModel parse(const std::string& preprocessed, const std::filesystem::path& origin) const;

private:
    std::vector<ImplementationSpec> parseImplementation(const std::string& text) const;
};

}  // namespace kos::oilcpp
