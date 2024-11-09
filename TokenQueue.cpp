#include "headers/TokenQueue.h"

void TokenQueue::push(const Token& token) {
    tokens.push(token);
}

Token TokenQueue::pop() {
    if (tokens.empty()) {
        return Token{TokenType::Literal, ""};
    }
    Token frontToken = tokens.front();
    tokens.pop();
    return frontToken;
}

Token TokenQueue::peek() const {
    if (tokens.empty()) {
        return Token{TokenType::Literal, ""};
    }
    return tokens.front();
}

bool TokenQueue::isEmpty() const {
    return tokens.empty();
}

void TokenQueue::clear() {
    while (!tokens.empty()) {
        tokens.pop();
    }
}