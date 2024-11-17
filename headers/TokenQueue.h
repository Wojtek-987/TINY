#ifndef TOKENQUEUE_H
#define TOKENQUEUE_H

#include "../types/Token.h"
#include <deque>

class TokenQueue {
public:
    void push(const Token& token);

    Token pop();

    [[nodiscard]] Token peek() const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] size_t size() const;

    void push_front(const Token& token);

    void clear();

private:
    std::deque<Token> tokens;
};

#endif //TOKENQUEUE_H
