#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <optional>

namespace oil {

enum class TokenKind {
    End,
    Identifier,
    String,
    Integer,
    True,
    False,
    // punctuation
    LBrace, RBrace, LParen, RParen, LBracket, RBracket,
    Comma, Semicolon, Colon, Equal
};

struct Token {
    TokenKind kind{TokenKind::End};
    std::string text; // for Identifier/String
    int64_t intValue{0};
    size_t line{1};
    size_t column{1};
};

class Lexer {
public:
    explicit Lexer(std::string input)
        : input_(std::move(input)) {}

    const Token &peek();
    Token next();

    static bool isKeyword(const std::string &id, const char* kw) {
        if (!kw) return false;
        size_t n = id.size();
        for (size_t i = 0; i < n; ++i) {
            char a = id[i];
            char b = kw[i];
            if (b == '\0') return false;
            if (a >= 'a' && a <= 'z') a = char(a - 'a' + 'A');
            if (b >= 'a' && b <= 'z') b = char(b - 'a' + 'A');
            if (a != b) return false;
        }
        return kw[n] == '\0';
    }
private:
    char curr() const { return pos_ < input_.size() ? input_[pos_] : '\0'; }
    char nextChar();
    void skipWhitespaceAndComments();
    Token lexIdentifierOrKeyword();
    Token lexNumber();
    Token lexString();

    std::string input_;
    size_t pos_{0};
    size_t line_{1};
    size_t col_{1};
    std::optional<Token> lookahead_{};
};

} // namespace oil
