#include "OilParser.hpp"
#include <sstream>

namespace oil {

bool Parser::accept(TokenKind k) {
    if (lex_.peek().kind == k) { lex_.next(); return true; }
    return false;
}

void Parser::expect(TokenKind k, const char* what) {
    const auto &t = lex_.peek();
    if (t.kind != k) {
        std::ostringstream oss;
        oss << "Parse error at " << t.line << ":" << t.column << ", expected " << what;
        throw ParseError(oss.str());
    }
    lex_.next();
}

std::string Parser::expectIdentifier() {
    const auto &t = lex_.peek();
    if (t.kind != TokenKind::Identifier) {
        std::ostringstream oss; oss << "expected identifier at " << t.line << ":" << t.column;
        throw ParseError(oss.str());
    }
    std::string s = t.text;
    lex_.next();
    return s;
}

Value* Parser::parseList() {
    // list: '(' value (',' value)* ')'
    expect(TokenKind::LParen, "'('");
    auto* val = arena_.make<Value>();
    val->data = ListValue{};
    auto &lst = std::get<ListValue>(val->data);
    if (lex_.peek().kind != TokenKind::RParen) {
        lst.elements.push_back(parseValue());
        while (accept(TokenKind::Comma)) {
            lst.elements.push_back(parseValue());
        }
    }
    expect(TokenKind::RParen, "')'");
    return val;
}

Value* Parser::parseValue() {
    const auto &t = lex_.peek();
    auto* v = arena_.make<Value>();
    v->loc = {t.line, t.column};
    switch (t.kind) {
        case TokenKind::String: v->data = ScalarString{t.text}; lex_.next(); break;
        case TokenKind::Identifier: v->data = ScalarIdent{t.text}; lex_.next(); break;
        case TokenKind::Integer: v->data = ScalarInt{t.intValue}; lex_.next(); break;
        case TokenKind::True: v->data = ScalarBool{true}; lex_.next(); break;
        case TokenKind::False: v->data = ScalarBool{false}; lex_.next(); break;
        case TokenKind::LParen: return parseList();
        default: {
            std::ostringstream oss; oss << "expected value at " << t.line << ":" << t.column; throw ParseError(oss.str());
        }
    }
    return v;
}

void Parser::parseOptionalDescription(Node* n) {
    if (accept(TokenKind::Colon)) {
        const auto &t = lex_.peek();
        if (t.kind == TokenKind::String) { n->description = t.text; lex_.next(); }
    }
}

bool Parser::tryParseAttribute(Node* into) {
    // IDENT '=' value ';'
    if (lex_.peek().kind != TokenKind::Identifier) return false;
    // lookahead to see if equals appears
    Token tId = lex_.next();
    if (!accept(TokenKind::Equal)) {
        // it was not an attribute; treat as unexpected
        std::ostringstream oss; oss << "expected '=' after attribute name '" << tId.text << "'";
        throw ParseError(oss.str());
    }
    Value* val = parseValue();
    expect(TokenKind::Semicolon, "';'");
    Attribute attr; attr.name = tId.text; attr.value = val; attr.loc = {tId.line, tId.column};
    into->attributes.push_back(attr);
    return true;
}

void Parser::parseItemList(Node* parent) {
    // A body contains either nested objects: <OIL_OBJECT> IDENT '{' ... '}' description? ';'
    // or attributes: IDENT '=' value ';'
    while (true) {
        const auto &t = lex_.peek();
        if (t.kind == TokenKind::RBrace) { break; }
        if (t.kind == TokenKind::End) { break; }
        if (t.kind == TokenKind::Identifier) {
            // Could be an attribute or a nested object (keyword like TASK/OS/...)
            // We'll treat known keywords as object kinds.
            std::string ident = t.text;
            if (Lexer::isKeyword(ident, "OS") || Lexer::isKeyword(ident, "TASK") || Lexer::isKeyword(ident, "COUNTER") ||
                Lexer::isKeyword(ident, "ALARM") || Lexer::isKeyword(ident, "RESOURCE") || Lexer::isKeyword(ident, "EVENT") ||
                Lexer::isKeyword(ident, "ISR") || Lexer::isKeyword(ident, "MESSAGE") || Lexer::isKeyword(ident, "COM") ||
                Lexer::isKeyword(ident, "NM") || Lexer::isKeyword(ident, "APPMODE") || Lexer::isKeyword(ident, "SCHEDULETABLE")) {
                lex_.next();
                Node* child = parseObjectWithBody(ident);
                parent->children.push_back(child);
                continue;
            } else {
                // attribute
                tryParseAttribute(parent);
                continue;
            }
        } else {
            // Unexpected token, attempt to recover by consuming
            lex_.next();
        }
    }
}

Node* Parser::parseObjectWithBody(const std::string& kind) {
    Node* n = arena_.make<Node>();
    n->kind = kind; n->loc = {lex_.peek().line, lex_.peek().column};
    n->name = expectIdentifier();
    expect(TokenKind::LBrace, "'{' ");
    parseItemList(n);
    expect(TokenKind::RBrace, "'}'");
    parseOptionalDescription(n);
    expect(TokenKind::Semicolon, "';'");
    return n;
}

std::string Parser::parseOilVersion() {
    // OIL_VERSION '=' string ';'
    std::string kw = expectIdentifier();
    if (!Lexer::isKeyword(kw, "OIL_VERSION")) {
        // tolerate unexpected token by not failing hard (some OIL dialects differ)
    }
    expect(TokenKind::Equal, "'='");
    const auto &t = lex_.peek();
    if (t.kind != TokenKind::String) throw ParseError("expected string for OIL_VERSION");
    std::string ver = t.text; lex_.next();
    // optional description
    if (accept(TokenKind::Colon)) {
        if (lex_.peek().kind == TokenKind::String) lex_.next();
    }
    expect(TokenKind::Semicolon, "';'");
    return ver;
}

Node* Parser::parseImplementationDefinition() {
    // IMPLEMENTATION ident '{' ... '}' description? ';'
    std::string kw = expectIdentifier();
    if (!Lexer::isKeyword(kw, "IMPLEMENTATION")) {
        throw ParseError("expected IMPLEMENTATION");
    }
    Node* impl = arena_.make<Node>();
    impl->kind = "IMPLEMENTATION";
    impl->name = expectIdentifier();
    expect(TokenKind::LBrace, "'{' ");
    parseItemList(impl);
    expect(TokenKind::RBrace, "'}'");
    parseOptionalDescription(impl);
    expect(TokenKind::Semicolon, "';'");
    return impl;
}

Node* Parser::parseApplicationDefinition() {
    // APPLICATION ident '{' ... '}' description? ';'
    std::string kw = expectIdentifier();
    if (!Lexer::isKeyword(kw, "APPLICATION")) {
        throw ParseError("expected APPLICATION");
    }
    Node* app = arena_.make<Node>();
    app->kind = "APPLICATION";
    app->name = expectIdentifier();
    expect(TokenKind::LBrace, "'{' ");
    parseItemList(app);
    expect(TokenKind::RBrace, "'}'");
    parseOptionalDescription(app);
    expect(TokenKind::Semicolon, "';'");
    return app;
}

OilFile Parser::parseOilFile() {
    OilFile f;
    // oilfile: oilVersion implementationDefinition applicationDefinition
    f.oilVersion = parseOilVersion();
    f.implementation = parseImplementationDefinition();
    f.application = parseApplicationDefinition();
    return f;
}

} // namespace oil
