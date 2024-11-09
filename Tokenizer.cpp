#include "headers/Tokenizer.h"
#include "types/Token.h"
#include <sstream>
#include <unordered_set>
#include <cctype>

using std::string;

Tokenizer::Tokenizer(TokenQueue& queue) : queue(queue) {}

void Tokenizer::tokenize(const string& line) const {
    std::istringstream stream(line);
    string token;

    // Split the line by spaces
    while (stream >> token) {
        const TokenType type = identifyTokenType(token);
        queue.push(Token(type, token));
    }
}

TokenType Tokenizer::identifyTokenType(const string& token) {
    // Define sets of keywords and operators
    static const std::unordered_set<std::string> system_keywords = {"hello", "clear", "exit" };
    static const std::unordered_set<std::string> keywords = {"vint", "cint", "last", "rename", "delete", "comment"};
    static const std::unordered_set<std::string> operators = {"+", "-", "*", "/", "="};

    // Helper lambda to check membership in a set
    auto is_in_set = [](const std::unordered_set<string>& set, const string& value) {
        return set.contains(value);
    };

    // Check if the token is a keyword
    if (is_in_set(keywords, token)) {
        return TokenType::Keyword;
    }

    // Check if the token is a system keyword
    if (is_in_set(system_keywords, token)) {
        return TokenType::SystemKeyword;
    }

    // Check if the token is an operator
    if (is_in_set(operators, token)) {
        return TokenType::Operator;
    }

    // Check if the token is a literal (a valid integer format with optional negative sign)
    if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1 && std::isdigit(token[1]))) {
        bool is_valid_literal = true;
        for (unsigned long long int i = (token[0] == '-' ? 1 : 0); i < token.size(); ++i) {
            if (!std::isdigit(token[i])) {
                is_valid_literal = false;
                break;
            }
        }
        if (is_valid_literal) {
            return TokenType::Literal;
        }
    }

    // Check if the token is a valid identifier
    bool is_valid_identifier = true;
    for (const char ch : token) {
        if (!std::islower(ch)) {
            is_valid_identifier = false;
            break;
        }
    }
    if (is_valid_identifier) {
        return TokenType::Identifier;
    }

    // If none of the above, it's an invalid token type
    return TokenType::Invalid;
}