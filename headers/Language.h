#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>
using std::string;

class Language {
public:
    static const string valid_chars;

    static bool is_clean(const string& line);

    static bool is_valid_identifier(const string& name);
};

#endif