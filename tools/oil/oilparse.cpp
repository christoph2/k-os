#include "OilParser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace oil;

static void dumpValue(const Value* v, std::ostream& os, int indent);
static void dumpNode(const Node* n, std::ostream& os, int indent);

static void pad(std::ostream& os, int n) { for (int i=0;i<n;++i) os << ' '; }

static void dumpValue(const Value* v, std::ostream& os, int indent) {
    std::visit([&](auto&& arg){
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, ScalarString>) {
            os << '"' << arg.v << '"';
        } else if constexpr (std::is_same_v<T, ScalarIdent>) {
            os << "{\"ident\":\"" << arg.v << "\"}";
        } else if constexpr (std::is_same_v<T, ScalarInt>) {
            os << arg.v;
        } else if constexpr (std::is_same_v<T, ScalarBool>) {
            os << (arg.v ? "true" : "false");
        } else if constexpr (std::is_same_v<T, ListValue>) {
            os << "[";
            for (size_t i=0;i<arg.elements.size();++i) {
                if (i) os << ", ";
                dumpValue(arg.elements[i], os, indent);
            }
            os << "]";
        }
    }, v->data);
}

static void dumpNode(const Node* n, std::ostream& os, int indent) {
    pad(os, indent); os << "{\n";
    pad(os, indent+2); os << "\"kind\": \"" << n->kind << "\",\n";
    pad(os, indent+2); os << "\"name\": \"" << n->name << "\",\n";
    if (n->description) { pad(os, indent+2); os << "\"description\": \"" << *n->description << "\",\n"; }
    pad(os, indent+2); os << "\"attributes\": [\n";
    for (size_t i=0;i<n->attributes.size();++i) {
        const auto &a = n->attributes[i];
        pad(os, indent+4); os << "{ \"name\": \"" << a.name << "\", \"value\": ";
        dumpValue(a.value, os, indent+4); os << " }";
        if (i+1<n->attributes.size()) os << ",";
        os << "\n";
    }
    pad(os, indent+2); os << "],\n";
    pad(os, indent+2); os << "\"children\": [\n";
    for (size_t i=0;i<n->children.size();++i) {
        dumpNode(n->children[i], os, indent+4);
        if (i+1<n->children.size()) os << ",";
        os << "\n";
    }
    pad(os, indent+2); os << "]\n";
    pad(os, indent); os << "}";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: oilparse <file.oil>\n";
        return 2;
    }
    std::ifstream ifs(argv[1], std::ios::binary);
    if (!ifs) {
        std::cerr << "Cannot open: " << argv[1] << "\n";
        return 2;
    }
    std::ostringstream oss; oss << ifs.rdbuf();
    std::string input = oss.str();
    Arena arena;
    try {
        Parser p(Lexer(std::move(input)), arena);
        OilFile f = p.parseOilFile();
        std::cout << "{\n  \"OIL_VERSION\": \"" << f.oilVersion << "\",\n  \"IMPLEMENTATION\": ";
        dumpNode(f.implementation, std::cout, 2);
        std::cout << ",\n  \"APPLICATION\": ";
        dumpNode(f.application, std::cout, 2);
        std::cout << "\n}\n";
    } catch (const ParseError& e) {
        std::cerr << "ParseError: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
