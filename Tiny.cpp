#include <iostream>
#include <string>
#include "types/OutputType.h"
#include "types/Colour.h"
#include "headers/Language.h"
#include "headers/Tiny.h"
#include "headers/Tokenizer.h"

using std::string;
using std::cout;
using std::endl;



class ColourText {
public:
    static string open(const Colour& colour) {
        if (colour == Colour::Blue) {
            return "\033[1m\033[34m";
        }
        if (colour == Colour::Magenta) {
            return "\033[35m";
        }
        if (colour == Colour::Red) {
            return "\033[31m";
        }
        if (colour == Colour::Cyan) {
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
    cout << "Welcome to " << ColourText::open(Colour::Blue) << " TINY " << ColourText::close() << " interpreter! For more info type 'help'" << endl;
}

Tiny::Tiny(const string& interpreter_name) {
    name = interpreter_name;
    is_running = true;
    last = 0;
    greet();
}

string Tiny::get_name() const {
    return this->name;
}

void Tiny::prompt_input() const {
    cout << ColourText::open(Colour::Blue) << this->name << ColourText::close() << " < ";
}

string Tiny::sterilize_input(const string& input_string) const {
    const unsigned long long length = input_string.length();

    if(!Language::is_clean(input_string)) {
        this->return_output("Invalid character", OutputType::Error);
        return "Error";
    }

    string sterilized_input;

    // Clear multiple spaces
    for (unsigned long long i = 0; i < length - 1; i++) {

        if (input_string[i] == ' ') {
            sterilized_input += ' ';
            while (input_string[i] == ' ') {
                i++;
            }
        }
    }

    if (!sterilized_input.empty()) {
        // Trim spaces from the beginning
        unsigned long long int start = 0;
        while (start < sterilized_input.length() && sterilized_input[start] == ' ') {
            ++start;
        }
        sterilized_input = sterilized_input.substr(start);

        // Trim spaces from the end
        unsigned long long int end = sterilized_input.length();
        while (end > 0 && sterilized_input[end - 1] == ' ') {
            --end;
        }
        sterilized_input = sterilized_input.substr(0, end);

    }

    return sterilized_input;
}

void Tiny::tokenize_input(const string& input) const {
    // Sterilize input
    const string sterilized_input = this->sterilize_input(input);
    if (sterilized_input == "Error")
        return;

    // Tokenize
    const Tokenizer tokenizer(queue);
    tokenizer.tokenize(sterilized_input);
}

void Tiny::process_token_queue() {
    // interpret the token queue
}

void Tiny::return_output(const string& value, const OutputType type) const {

    Colour colour;

    switch (type) {
        case OutputType::Output:
            colour = Colour::Magenta;
        break;
        case OutputType::Error:
            colour = Colour::Red;
        break;
        case OutputType::Info:
            colour = Colour::Cyan;
        break;
    }

    cout << ColourText::open(Colour::Blue) << this->name << ColourText::close() << " > " << ColourText::open(colour) << value << ColourText::close() << endl;
}

Tiny::~Tiny() {
    is_running = false;
    cout << "Goodbye!" << endl;
}