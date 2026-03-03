#include "OilLexer.hpp"
#include <cctype>

namespace oil {

static bool isIdentStart(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '$';
}
static bool isIdentChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '$' || c == '-';
}

char Lexer::nextChar() {
    char c = curr();
    if (c == '\0') return c;
    ++pos_;
    if (c == '\n') { ++line_; col_ = 1; } else { ++col_; }
    return curr();
}

void Lexer::skipWhitespaceAndComments() {
    for (;;) {
        char c = curr();
        // whitespace
        while (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' || c == '\v') {
            nextChar();
            c = curr();
        }
        // comments: // line, /* block */
        if (c == '/') {
            char n = (pos_ + 1 < input_.size()) ? input_[pos_ + 1] : '\0';
            if (n == '/') {
                // line comment
                while (curr() && curr() != '\n') nextChar();
                continue;
            } else if (n == '*') {
                // block comment
                pos_ += 2; col_ += 2;
                while (curr()) {
                    if (curr() == '*' && (pos_ + 1 < input_.size()) && input_[pos_ + 1] == '/') { pos_ += 2; col_ += 2; break; }
                    nextChar();
                }
                continue;
            }
        }
        break;
    }
}

Token Lexer::lexIdentifierOrKeyword() {
    Token t; t.line = line_; t.column = col_;
    std::string s;
    s.push_back(curr());
    nextChar();
    while (isIdentChar(curr())) { s.push_back(curr()); nextChar(); }
    // Keywords true/false
    if (Lexer::isKeyword(s, "TRUE")) { t.kind = TokenKind::True; return t; }
    if (Lexer::isKeyword(s, "FALSE")) { t.kind = TokenKind::False; return t; }
    t.kind = TokenKind::Identifier; t.text = std::move(s);
    return t;
}

Token Lexer::lexNumber() {
    Token t; t.line = line_; t.column = col_;
    int64_t v = 0;
    bool neg = false;
    if (curr() == '+') { nextChar(); }
    else if (curr() == '-') { neg = true; nextChar(); }

    if (curr() == '0' && (pos_ + 1 < input_.size()) && (input_[pos_ + 1] == 'x' || input_[pos_ + 1] == 'X')) {
        pos_ += 2; col_ += 2; // consume 0x
        while (std::isxdigit(static_cast<unsigned char>(curr()))) {
            char c = curr();
            v *= 16;
            if (c >= '0' && c <= '9') v += c - '0';
            else if (c >= 'a' && c <= 'f') v += 10 + (c - 'a');
            else if (c >= 'A' && c <= 'F') v += 10 + (c - 'A');
            nextChar();
        }
    } else {
        while (std::isdigit(static_cast<unsigned char>(curr()))) { v = v * 10 + (curr() - '0'); nextChar(); }
    }
    if (neg) v = -v;
    t.kind = TokenKind::Integer;
    t.intValue = v;
    return t;
}

Token Lexer::lexString() {
    Token t; t.line = line_; t.column = col_;
    char quote = curr();
    nextChar();
    std::string s;
    while (curr() && curr() != quote) {
        char c = curr();
        if (c == '\\') { // escape
            nextChar();
            char e = curr();
            switch (e) {
                case 'n': s.push_back('\n'); break;
                case 'r': s.push_back('\r'); break;
                case 't': s.push_back('\t'); break;
                case '\\': s.push_back('\\'); break;
                case '"': s.push_back('"'); break;
                case '\'': s.push_back('\''); break;
                default: s.push_back(e); break;
            }
            nextChar();
        } else {
            s.push_back(c); nextChar();
        }
    }
    if (curr() == quote) nextChar();
    t.kind = TokenKind::String; t.text = std::move(s);
    return t;
}

const Token &Lexer::peek() {
    if (!lookahead_) {
        skipWhitespaceAndComments();
        Token t; t.line = line_; t.column = col_;
        char c = curr();
        if (c == '\0') { t.kind = TokenKind::End; lookahead_ = t; return *lookahead_; }
        if (isIdentStart(c)) { lookahead_ = lexIdentifierOrKeyword(); return *lookahead_; }
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '+' || c == '-' ) { lookahead_ = lexNumber(); return *lookahead_; }
        switch (c) {
            case '{': t.kind = TokenKind::LBrace; nextChar(); break;
            case '}': t.kind = TokenKind::RBrace; nextChar(); break;
            case '(': t.kind = TokenKind::LParen; nextChar(); break;
            case ')': t.kind = TokenKind::RParen; nextChar(); break;
            case '[': t.kind = TokenKind::LBracket; nextChar(); break;
            case ']': t.kind = TokenKind::RBracket; nextChar(); break;
            case ',': t.kind = TokenKind::Comma; nextChar(); break;
            case ';': t.kind = TokenKind::Semicolon; nextChar(); break;
            case ':': t.kind = TokenKind::Colon; nextChar(); break;
            case '=': t.kind = TokenKind::Equal; nextChar(); break;
            case '"': case '\'': lookahead_ = lexString(); return *lookahead_;
            default:
                // treat other punctuation as identifier separators; consume and continue
                nextChar();
                return peek();
        }
        lookahead_ = t;
    }
    return *lookahead_;
}

Token Lexer::next() {
    const Token &t = peek();
    Token out = t;
    lookahead_.reset();
    return out;
}

} // namespace oil
