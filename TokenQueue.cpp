#include "headers/TokenQueue.h"

void TokenQueue::push(const Token& token) {
    tokens.push_back(token);
}

Token TokenQueue::pop() {
    if (tokens.empty()) {
        return Token{TokenType::Literal, ""};
    }
    Token frontToken = tokens.front();
    tokens.pop_front();
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

size_t TokenQueue::size() const {
    return tokens.size();
}

void TokenQueue::push_front(const Token& token) {
    tokens.push_front(token);
}

void TokenQueue::clear() {
    while (!tokens.empty()) {
        tokens.pop_front();
    }
}