#pragma once

#include <filesystem>
#include <string>

#include "OilAst.hpp"

namespace kos::oilcpp {

class Generator {
public:
    struct Options {
        bool generate_orti{false};
        bool test_only{false};
        std::filesystem::path output_prefix;
        std::string target;
        bool generate_main_stub{true};
    };

    void generate(const OilModel& model, const Options& opts) const;

private:
    void writeOsCfgH(const OilModel& model, const Options& opts) const;
    void writeOsCfgC(const OilModel& model, const Options& opts) const;
    void writeOrti(const OilModel& model, const Options& opts) const;
    void writeTaskIsrStubs(const OilModel& model, const Options& opts) const;
    void writeMainStub(const OilModel& model, const Options& opts) const;
    void writeTargetTemplate(const Options& opts) const;
};

}  // namespace kos::oilcpp
