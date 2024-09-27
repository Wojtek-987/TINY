#include <iostream>
#include <string>
#include "types/OutputType.h"
#include "types/Colour.h"
#include "headers/Language.h"
#include "headers/Tiny.h"

using std::string;
using std::cout;
using std::endl;



class ColourText {
public:
    static string open(const Colour& colour) {
        if (colour == Colour::Blue) {
            return "\033[1m\033[34m";
        } else if (colour == Colour::Magenta) {
            return "\033[35m";
        } else if (colour == Colour::Red) {
            return "\033[31m";
        } else if (colour == Colour::Cyan) {
            return "\033[36m";
        }

        return "";
    }

    static string close() {
        return "\033[0m";
    }
};

void Tiny::change_name(const string& new_name) {
    name = new_name;
}

void Tiny::greet() {
    cout << "Welcome to " << ColourText::open(Colour::Blue) << " TINY " << ColourText::close() << " interpreter! For a list of commands type 'help'" << endl;
}

string Tiny::sterilize_input(const string& input_string) {
    const unsigned long long length = input_string.length();

    if(!Language::is_clean(input_string)) {
        // tiny.throw_error("invalid character")
    }

    string sterilized_input;

    // Clear multiple spaces
    for(unsigned long long i = 0; i < length - 1; i++) {

        if(input_string[i] == ' ') {
            sterilized_input += ' ';
            while(input_string[i] == ' ') {
                i++;
            }
        }
    }

    if(!sterilized_input.empty()) {
        // Trim spaces from the beginning and end
        if(sterilized_input[0] == ' ') {
            // dropLeft(1)
        }
        if(sterilized_input[sterilized_input.length() - 1] == ' ') {
            // dropRight(1)
        }
    }

    return sterilized_input;
}

void Tiny::tokenize_input() {
    // sterilize input
    // tokenize
}

void Tiny::process_token_stack() {
    // interpret the tokens
}

Tiny::Tiny(const string& interpreter_name) {
    name = interpreter_name;
    is_active = true;
    greet();
}

Tiny::~Tiny() {
    is_active = false;
    cout << "Goodbye!";
}

void Tiny::prompt_input() {
    cout << ColourText::open(Colour::Blue) << this->name << ColourText::close() << " < ";
}

void Tiny::return_output(const string& value, OutputType type) {
    cout << ColourText::open(Colour::Blue) << this->name << ColourText::close() << " > " << ColourText::open(Colour::Magenta) << value << ColourText::close() << endl;
    // \033[31m red for errors
    // \033[36m cyan for info
}