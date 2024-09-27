#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>
using std::string;

class Language {
public:
    // Declare the static valid_chars constant
    static const string valid_chars;

    // Declare the is_clean function
    static bool is_clean(const string& line);
};

#endif