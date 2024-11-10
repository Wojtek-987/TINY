#ifndef TINY_H
#define TINY_H

#include "../types/OutputType.h"
#include "TokenQueue.h"


using std::string;


class Tiny {
private:
    string name;

    mutable TokenQueue queue;

    static void greet();

    void change_name(const string& new_name);


public:
    mutable bool is_running;
    mutable long long int last;

    explicit Tiny(const string& interpreter_name = "tiny");

    string get_name() const;

    void prompt_input() const;

    [[nodiscard]] string sterilize_input(const string& input_string) const;

    void tokenize_input(const string& input) const;

    string process_token_queue() const;

    void return_output(const string& value, OutputType type) const;

    ~Tiny();
};

#endif