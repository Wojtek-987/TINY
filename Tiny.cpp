#include <iostream>
#include <string>
#include <cstdlib>
#include "types/OutputType.h"
#include "types/Colour.h"
#include "headers/Language.h"
#include "headers/Tiny.h"

#include "headers/Logger.h"
#include "headers/Run_Loop.h"
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
    if(input_string.empty()) return "";
    const unsigned long long length = input_string.length();

    if(!Language::is_clean(input_string)) {
        this->return_output("Invalid character", OutputType::Error);
        return "";
    }

    string sterilized_input;

    // Clear multiple spaces
    bool in_space_sequence = false;
    for (unsigned long long i = 0; i < length; i++) {
        if (input_string[i] == ' ') {
            if (!in_space_sequence) {
                sterilized_input += ' ';
                in_space_sequence = true;
            }
        } else {
            sterilized_input += input_string[i];
            in_space_sequence = false;
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
    if (sterilized_input.empty())
        return;

    // Tokenize
    Tokenizer::tokenize(sterilized_input);
}

string Tiny::process_token_queue() const {

    if(Tokenizer::queue.isEmpty()) {
        this->return_output("Received empty token queue.", OutputType::Info);
        return "";
    }

    const Token token = Tokenizer::queue.peek();



    // === Add Comment functionality ===
    if (token.type == TokenType::Keyword && token.value == "comment") {
        Tokenizer::queue.pop(); // Remove "comment" token from the queue

        // Output the remaining tokens as the comment text
        string comment_text;
        while (!Tokenizer::queue.isEmpty()) {
            comment_text += Tokenizer::queue.pop().value + " ";
        }

        this->return_output("comment: " + comment_text, OutputType::Info);
        return comment_text;
    }



    // === Add System Command functionality ===
    if (token.type == TokenType::SystemKeyword) {
        // Implement the hello command
        if (token.value == "hello") {
            Tokenizer::queue.pop();
            if (Tokenizer::queue.isEmpty()) {
                this->return_output("hello world!", OutputType::Output);
                return "hello world!";
            }
        }



        // Implement the clear command
        if (token.value == "clear") {
            Tokenizer::queue.pop();
            if (Tokenizer::queue.isEmpty()) {

                this->last = 0;

                if (const bool is_cleared = clear_log_file(*this, log_file_path); !is_cleared) {
                    this-> return_output("Counldn't clear logs!", OutputType::Error);
                }

                // TEMP clear all variables and constants

                this->return_output("Cleared state!", OutputType::Info);
                return "Cleared";
            }
        }



        // Implement the exit command
        if (token.value == "exit") {
            Tokenizer::queue.pop();
            if (Tokenizer::queue.isEmpty()) {
                this->is_running = false;
                return "Terminated";
            }
        }



        // Implement the help command
        if (token.value == "help") {
            Tokenizer::queue.pop();
            if (Tokenizer::queue.isEmpty()) {
                // Open browser link
                const string url = "https://github.com/Wojtek-987/TINY"; // TEMP
                #ifdef _WIN32
                    std::string command = "start " + url;
                #elif __APPLE__
                    std::string command = "open " + url;
                #elif __linux__
                    std::string command = "xdg-open " + url;
                #else
                    #error "Unsupported platform"
                #endif
                std::system(command.c_str());

                return "Online help";
            }
        }


        // === Unexpected token ===
        string error = "Unexpected token: " + Tokenizer::queue.peek().value;
        this->return_output(error, OutputType::Error);
        Tokenizer::queue.clear();
        return error;
    }



    // === REMINDER: Update the tiny.last variable to the last VALID int output ===


    // Add Basic Math functionality (accounting for overflow as stated in docs)

    // Add constant and variable creation based on docs
    // + Assure inline value assignment on creation works ex. "vint x = 5"

    // Add deletion, renaming, and value changing to variables and constants (+ error handling for constants)

    // Add operations between variables and constants (by value substitution, similar to basic math functionality but with preemptive error handling)

    // Check all this works for *.tiny files and the logs.


    // TEMP - debug
    this->return_output("other", OutputType::Info);
    Tokenizer::queue.clear();
    return "other";
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