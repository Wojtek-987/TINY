#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "TokenQueue.h"
#include <string>

class Tokenizer {
public:
    explicit Tokenizer(TokenQueue& queue);
    void tokenize(const std::string& line) const;

private:
    TokenQueue& queue;
    static TokenType identifyTokenType(const std::string& token);
};

#endif //TOKENIZER_H
