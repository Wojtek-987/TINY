#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <utility>

enum class TokenType {
    Keyword, // vint...
    SystemKeyword,
    Identifier, // var_name...
    Operator, // +...
    Literal, // 7...
    Invalid // aB-...
};

struct Token {
    TokenType type;
    std::string value;

    Token(const TokenType t, std::string v) : type(t), value(std::move(v)) {}
};

#endif //TOKEN_H
