#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "TokenQueue.h"

class Tokenizer {
public:
    static TokenQueue queue;
    static void tokenize(const std::string& line) ;

private:
    static TokenType identifyTokenType(const std::string& token);
};

#endif //TOKENIZER_H
