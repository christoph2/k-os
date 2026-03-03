#pragma once
#include "OilLexer.hpp"
#include "OilAst.hpp"
#include <stdexcept>
#include <string>

namespace oil {

struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Parser {
public:
    Parser(Lexer lex, Arena& arena) : lex_(std::move(lex)), arena_(arena) {}

    OilFile parseOilFile();

private:
    bool accept(TokenKind k);
    void expect(TokenKind k, const char* what);

    std::string expectIdentifier();

    Value* parseValue();
    Value* parseList();

    // High-level constructs
    std::string parseOilVersion();
    Node* parseImplementationDefinition();
    Node* parseApplicationDefinition();

    // Generic object and lists
    Node* parseObjectWithBody(const std::string& kind);
    void parseItemList(Node* parent);

    // attribute: IDENT '=' value ';'
    bool tryParseAttribute(Node* into);

    // optional description: ':' STRING?
    void parseOptionalDescription(Node* n);

    Lexer lex_;
    Arena& arena_;
};

} // namespace oil
