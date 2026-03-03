#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "Generator.hpp"
#include "OilParser.hpp"
#include "Preprocessor.hpp"
#include "Version.hpp"

namespace fs = std::filesystem;

namespace {

struct CliOptions {
    std::vector<fs::path> include_paths;
    std::optional<fs::path> output_prefix;
    bool generate_orti{false};
    bool test_only{false};
    std::string target;
    bool show_help{false};
    bool show_version{false};
    fs::path input;
};

void printHelp() {
    std::cout << "kosgen_cpp - C++ OIL generator (prototype)\n"
              << "Usage: kosgen_cpp [options] <input.oil>\n"
              << "Options:\n"
              << "  -I, --include <dir>   Add include search path (KOS_INCLUDE is also used)\n"
              << "  -o, --output <path>   Output prefix (default: input path without extension)\n"
              << "  --target <name>       Target identifier for templating/build outputs\n"
              << "  --orti                Generate ORTI (.ort) alongside Os_Cfg files\n"
              << "  --test                Parse/validate only (no output)\n"
              << "  -h, --help            Show this help\n"
              << "  -V, --version         Show version\n";
}

CliOptions parseArgs(int argc, char** argv) {
    CliOptions opts;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            opts.show_help = true;
            return opts;
        } else if (arg == "-V" || arg == "--version") {
            opts.show_version = true;
            return opts;
        } else if (arg == "-I" || arg == "--include") {
            if (i + 1 >= argc) throw std::runtime_error("Missing path after " + arg);
            opts.include_paths.emplace_back(argv[++i]);
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 >= argc) throw std::runtime_error("Missing path after " + arg);
            opts.output_prefix = fs::path(argv[++i]);
        } else if (arg == "--target") {
            if (i + 1 >= argc) throw std::runtime_error("Missing name after --target");
            opts.target = argv[++i];
        } else if (arg == "--orti") {
            opts.generate_orti = true;
        } else if (arg == "--test") {
            opts.test_only = true;
        } else if (!arg.empty() && arg[0] == '-') {
            throw std::runtime_error("Unknown option: " + arg);
        } else {
            if (!opts.input.empty()) throw std::runtime_error("Multiple input files specified");
            opts.input = fs::path(arg);
        }
    }
    if (!opts.show_help && !opts.show_version && opts.input.empty()) {
        throw std::runtime_error("No input file specified");
    }
    return opts;
}

}  // namespace

int main(int argc, char** argv) {
    try {
        auto opts = parseArgs(argc, argv);
        if (opts.show_help) {
            printHelp();
            return 0;
        }
        if (opts.show_version) {
            std::cout << "kosgen_cpp " << kos::oilcpp::kVersion << "\n";
            return 0;
        }

        kos::oilcpp::Preprocessor pre(opts.include_paths);
        auto flattened = pre.process(opts.input);

        kos::oilcpp::OilParser parser;
        auto model = parser.parse(flattened, opts.input);

        kos::oilcpp::Generator gen;
        kos::oilcpp::Generator::Options gopts;
        gopts.generate_orti = opts.generate_orti;
        gopts.test_only = opts.test_only;
        gopts.output_prefix = opts.output_prefix.value_or(opts.input.replace_extension());
        gopts.target = opts.target;
        gen.generate(model, gopts);

        if (opts.test_only) {
            std::cout << "Preprocess + parse OK (test mode).\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
