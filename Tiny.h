#ifndef TINY_H
#define TINY_H

#include "OutputType.h"
using std::string;

class Tiny {
private:
    string name;

    void change_name(const string& new_name);

    static void greet();

    static string sterilize_input(const string& input_string);

    static void tokenize_input();

    static void process_token_stack();

public:
    bool is_active;

    explicit Tiny(const string& interpreter_name = "tiny");

    ~Tiny();

    void prompt_input();

    void return_output(const string& value, OutputType type);
};

#endif