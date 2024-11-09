#ifndef TOKENQUEUE_H
#define TOKENQUEUE_H

#include "../types/Token.h"
#include <queue>

class TokenQueue {
public:
    void push(const Token& token);

    Token pop();

    [[nodiscard]] Token peek() const;

    [[nodiscard]] bool isEmpty() const;

    void clear();

private:
    std::queue<Token> tokens;
};

#endif //TOKENQUEUE_H
