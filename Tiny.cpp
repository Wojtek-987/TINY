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
#include "headers/VariableManager.h"

using std::string;
using std::cout;
using std::endl;




#include <charconv>

long long safe_stoll(const std::string& str, bool& success) {
    long long result = 0;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
    success = (ec == std::errc());
    return result;
}


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
    if (Tokenizer::queue.isEmpty()) {
        const string error = "Received empty token queue.";
        this->return_output(error, OutputType::Info);
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

    // === Deleting variables and constants ===
    if (token.type == TokenType::Keyword && token.value == "delete") {
        Tokenizer::queue.pop(); // Remove "delete"

        if (Tokenizer::queue.isEmpty() || Tokenizer::queue.peek().type != TokenType::Identifier) {
            const string error = "Error: Missing or invalid identifier to delete.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }

        if (string name = Tokenizer::queue.pop().value; variableManager.remove(name)) {
            this->return_output(name + " -> #", OutputType::Info);
            return name + " deleted";
        } else {
            const string error = "Error: Identifier '" + name + "' does not exist.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
    }

    // === Renaming variables and constants using 'rename' keyword ===
    if (token.type == TokenType::Keyword && token.value == "rename") {
        Tokenizer::queue.pop(); // Remove 'rename'

        // Get the old name
        if (Tokenizer::queue.isEmpty() || Tokenizer::queue.peek().type != TokenType::Identifier) {
            const string error = "Error: Missing or invalid identifier to rename.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
        string oldName = Tokenizer::queue.pop().value;

        // Check for '='
        if (Tokenizer::queue.isEmpty() || Tokenizer::queue.peek().value != "=") {
            const string error = "Error: Missing '=' after identifier.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
        Tokenizer::queue.pop(); // Remove '='

        // Get the new name
        if (Tokenizer::queue.isEmpty() || Tokenizer::queue.peek().type != TokenType::Identifier) {
            const string error = "Error: Missing or invalid new identifier.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
        string newName = Tokenizer::queue.pop().value;

        // Check for extra tokens
        if (!Tokenizer::queue.isEmpty()) {
            const string error = "Error: Unexpected input after renaming command.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }

        // Perform renaming
        if (!variableManager.exists(oldName)) {
            const string error = "Error: Identifier '" + oldName + "' does not exist.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
        if (variableManager.exists(newName)) {
            const string error = "Error: Identifier '" + newName + "' already exists.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
        if (variableManager.rename(oldName, newName)) {
            long long value;
            if (!variableManager.getValue(newName, value)) {
                const string error = "Error: Could not retrieve value for identifier '" + newName + "'.";
                this->return_output(error, OutputType::Error);
                Tokenizer::queue.clear();
                return error;
            }
            VariableType varType;
            if (!variableManager.getType(newName, varType)) {
                const string error = "Error: Could not retrieve type for identifier '" + newName + "'.";
                this->return_output(error, OutputType::Error);
                Tokenizer::queue.clear();
                return error;
            }

            string output = oldName + " -> " + newName + (varType == VariableType::Variable ? " = " : ": ") + std::to_string(value);
            this->return_output(output, OutputType::Info);
            return output;
        } else {
            const string error = "Error: Could not rename identifier '" + oldName + "' to '" + newName + "'.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
    }

    // === Variable and Constant Creation ===
    if (token.type == TokenType::Keyword && (token.value == "vint" || token.value == "cint")) {
        bool is_variable = (token.value == "vint");
        Tokenizer::queue.pop(); // Remove "vint" or "cint"

        // Validate identifier
        if (Tokenizer::queue.isEmpty() || Tokenizer::queue.peek().type != TokenType::Identifier) {
            const string error = "Error: Missing or invalid identifier.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
        string name = Tokenizer::queue.pop().value;

        if (!Language::is_valid_identifier(name)) {
            const string error = "Error: Invalid identifier '" + name + "'.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }

        // Check for '='
        if (Tokenizer::queue.isEmpty() || Tokenizer::queue.peek().value != "=") {
            const string error = "Error: Missing '=' after identifier.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
        Tokenizer::queue.pop(); // Remove '='

        // Validate value
        if (Tokenizer::queue.isEmpty()) {
            const string error = "Error: Missing value after '='.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }

        const Token valueToken = Tokenizer::queue.pop();
        long long value;

        // Assigning 'last'
        if (valueToken.type == TokenType::Keyword && valueToken.value == "last") {
            value = this->last;
        }
        // Assigning a literal value
        else if (valueToken.type == TokenType::Literal) {
            bool success;
            long long tempValue = safe_stoll(valueToken.value, success);
            if (!success) {
                const string error = "Error: Invalid numeric value '" + valueToken.value + "'.";
                this->return_output(error, OutputType::Error);
                return error;
            }
            if (tempValue < -2147483648LL || tempValue > 2147483647LL) {
                const string error = "Error: Value out of range for '" + valueToken.value + "'.";
                this->return_output(error, OutputType::Error);
                Tokenizer::queue.clear();
                return error;
            }
            value = static_cast<int>(tempValue);
        } else {
            const string error = "Error: Invalid value after '='.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }

        // Add variable or constant
        if (variableManager.add(name, is_variable ? VariableType::Variable : VariableType::Constant, value)) {
            string output = is_variable ? (name + " = " + std::to_string(value)) : (name + ": " + std::to_string(value));
            this->return_output(output, OutputType::Info);
            return output;
        } else {
            const string error = "Error: Could not create " + string(is_variable ? "variable" : "constant") + " '" + name + "'.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
    }

    // === Output variable/constant value when only its name is entered ===
    if (token.type == TokenType::Identifier && Tokenizer::queue.size() == 1) {
        string name = token.value;
        Tokenizer::queue.pop(); // Remove the identifier

        if (long long value; variableManager.getValue(name, value)) {
            if (VariableType varType; variableManager.getType(name, varType)) {
                string output = name + (varType == VariableType::Variable ? " = " : ": ") + std::to_string(value);
                this->return_output(output, OutputType::Info);
                this->last = value;
                return output;
            } else {
                const string error = "Error: Could not retrieve type for identifier '" + name + "'.";
                this->return_output(error, OutputType::Error);
                Tokenizer::queue.clear();
                return error;
            }
        } else {
            const string error = "Error: Identifier '" + name + "' does not exist.";
            this->return_output(error, OutputType::Error);
            Tokenizer::queue.clear();
            return error;
        }
    }

    // === Output 'last' value when only 'last' is entered ===
    if (token.type == TokenType::Keyword && token.value == "last" && Tokenizer::queue.size() == 1) {
        Tokenizer::queue.pop();
        this->return_output(std::to_string(this->last), OutputType::Output);
        return std::to_string(this->last);
    }

    // === Handle potential variable assignment or math expression starting with identifier ===
    if (token.type == TokenType::Identifier) {
        string identifier = token.value;
        Tokenizer::queue.pop(); // Remove the identifier

        if (Tokenizer::queue.isEmpty()) {
            // Only identifier provided; output its value
            if (long long value; variableManager.getValue(identifier, value)) {
                if (VariableType varType; variableManager.getType(identifier, varType)) {
                    string output = identifier + (varType == VariableType::Variable ? " = " : ": ") + std::to_string(value);
                    this->return_output(output, OutputType::Info);
                    return output;
                } else {
                    const string error = "Error: Could not retrieve type for identifier '" + identifier + "'.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
            } else {
                const string error = "Error: Identifier '" + identifier + "' does not exist.";
                this->return_output(error, OutputType::Error);
                return error;
            }
        }

        // Peek at the next token to decide what to do
        const Token nextToken = Tokenizer::queue.peek();

        if (nextToken.value == "=") {
            // === Variable Assignment ===
            Tokenizer::queue.pop(); // Remove '='

            if (Tokenizer::queue.isEmpty()) {
                const string error = "Error: Missing value after '='.";
                this->return_output(error, OutputType::Error);
                return error;
            }

            const Token valueToken = Tokenizer::queue.pop();

            long long value;

            // Assigning 'last' to variables
            if (valueToken.type == TokenType::Keyword && valueToken.value == "last") {
                value = this->last;
            }
            // Assigning from another variable/constant
            else if (valueToken.type == TokenType::Identifier) {
                long long sourceValue;
                if (!variableManager.getValue(valueToken.value, sourceValue)) {
                    const string error = "Error: Identifier '" + valueToken.value + "' does not exist.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
                value = sourceValue;
            }
            // Assigning a literal value
            else if (valueToken.type == TokenType::Literal) {
                bool success;
                long long tempValue = safe_stoll(valueToken.value, success);
                if (!success) {
                    const string error = "Error: Invalid numeric value '" + valueToken.value + "'.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
                if (tempValue < -2147483648LL || tempValue > 2147483647LL) {
                    const string error = "Error: Value out of range for '" + valueToken.value + "'.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
                value = static_cast<int>(tempValue);
            } else {
                const string error = "Error: Invalid value after '='.";
                this->return_output(error, OutputType::Error);
                return error;
            }

            // Perform variable assignment
            if (variableManager.exists(identifier)) {
                VariableType targetType;
                if (!variableManager.getType(identifier, targetType)) {
                    const string error = "Error: Could not retrieve type for identifier '" + identifier + "'.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
                if (targetType == VariableType::Constant) {
                    const string error = "Error: Cannot modify constant '" + identifier + "'.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }

                if (variableManager.set(identifier, value)) {
                    string output = identifier + " = " + std::to_string(value);
                    this->return_output(output, OutputType::Info);
                    return output;
                } else {
                    const string error = "Error: Failed to update variable '" + identifier + "'.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
            } else {
                // Variable does not exist, create it
                if (variableManager.add(identifier, VariableType::Variable, value)) {
                    string output = identifier + " = " + std::to_string(value);
                    this->return_output(output, OutputType::Info);
                    return output;
                } else {
                    const string error = "Error: Could not create variable '" + identifier + "'.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
            }
        } else if (nextToken.type == TokenType::Operator) {
            // === Math Expression Starting with Identifier ===
            // Push back the identifier to the queue
            Tokenizer::queue.push_front(token);
            // Proceed to math expression handling
        } else {
            const string error = "Error: Unexpected token '" + nextToken.value + "'.";
            this->return_output(error, OutputType::Error);
            return error;
        }
    }

    // === Add Basic Math functionality ===
    if (Tokenizer::queue.size() >= 1) {
        Token firstToken = Tokenizer::queue.peek();

        if (firstToken.type == TokenType::Literal || firstToken.type == TokenType::Keyword || firstToken.type == TokenType::Identifier) {
            // Proceed with math expression handling as before
            // === Math Expression Handling ===
            if (Tokenizer::queue.size() >= 3) {
                // Get the first operand
                Token operand1Token = Tokenizer::queue.pop();
                long long num1;
                if (operand1Token.type == TokenType::Keyword && operand1Token.value == "last") {
                    num1 = this->last;
                } else if (operand1Token.type == TokenType::Identifier) {
                    long long value;
                    if (!variableManager.getValue(operand1Token.value, value)) {
                        const string error = "Error: Identifier '" + operand1Token.value + "' does not exist.";
                        this->return_output(error, OutputType::Error);
                        return error;
                    }
                    num1 = value;
                } else {
                    bool success;
                    num1 = safe_stoll(operand1Token.value, success);
                    if (!success) {
                        const string error = "Error: Invalid numeric value '" + operand1Token.value + "'.";
                        this->return_output(error, OutputType::Error);
                        return error;
                    }
                }

                // Get the operator
                Token operatorToken = Tokenizer::queue.pop();
                if (operatorToken.type != TokenType::Operator) {
                    const string error = "Error: Missing operator!";
                    this->return_output(error, OutputType::Error);
                    return error;
                }
                string operator_symbol = operatorToken.value;

                // Get the second operand
                Token operand2Token = Tokenizer::queue.pop();
                long long num2;
                if (operand2Token.type == TokenType::Keyword && operand2Token.value == "last") {
                    num2 = this->last;
                } else if (operand2Token.type == TokenType::Identifier) {
                    long long value;
                    if (!variableManager.getValue(operand2Token.value, value)) {
                        const string error = "Error: Identifier '" + operand2Token.value + "' does not exist.";
                        this->return_output(error, OutputType::Error);
                        return error;
                    }
                    num2 = value;
                } else {
                    bool success;
                    num2 = safe_stoll(operand2Token.value, success);
                    if (!success) {
                        const string error = "Error: Invalid numeric value '" + operand2Token.value + "'.";
                        this->return_output(error, OutputType::Error);
                        return error;
                    }
                }

                // Perform the operation
                long long result = 0;
                if (operator_symbol == "+") {
                    result = num1 + num2;
                } else if (operator_symbol == "-") {
                    result = num1 - num2;
                } else if (operator_symbol == "*") {
                    result = num1 * num2;
                } else if (operator_symbol == "/") {
                    if (num2 == 0) {
                        const string error = "Error: Division by zero!";
                        this->return_output(error, OutputType::Error);
                        return error;
                    }
                    result = num1 / num2; // Integer division
                } else {
                    const string error = "Error: Invalid Operator!";
                    this->return_output(error, OutputType::Error);
                    return error;
                }

                // Check for overflow/underflow
                if (result < -2147483648LL || result > 2147483647LL) {
                    const string error = "Error: Overflow or underflow detected!";
                    this->return_output(error, OutputType::Error);
                    return error;
                }

                // Update 'last' and output the result
                this->last = static_cast<int>(result);
                this->return_output(std::to_string(this->last), OutputType::Output);

                // Check for unexpected extra tokens after the operation
                if (!Tokenizer::queue.isEmpty()) {
                    const string error = "Error: Unexpected input after math operation.";
                    this->return_output(error, OutputType::Error);
                    return error;
                }

                return std::to_string(this->last);
            }

        }
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
                    const string error = "Error: Couldn't clear logs!";
                    this->return_output(error, OutputType::Error);
                    Tokenizer::queue.clear();
                    return error;
                }

                // Clear all variables and constants
                variableManager.clear();

                this->return_output("Cleared state!", OutputType::Info);
                return "Cleared";
            }
        }

        // Implement the exit command
        if (token.value == "exit") {
            Tokenizer::queue.pop();
            if (Tokenizer::queue.isEmpty()) {
                // Set is_running to false to exit the interpreter
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
                    string command = "start " + url;
                #elif __APPLE__
                    string command = "open " + url;
                #elif __linux__
                    string command = "xdg-open " + url;
                #else
                    #error "Unsupported platform"
                #endif
                std::system(command.c_str());

                return "Online help";
            }
        }
    }

    // === Unexpected token ===
    if (!Tokenizer::queue.isEmpty() && !Tokenizer::queue.peek().value.empty()) {
        const string error = "Unexpected token: " + Tokenizer::queue.peek().value;
        this->return_output(error, OutputType::Error);
        Tokenizer::queue.clear();
        return error;
    }

    // TEMP - debug
    this->return_output("Uncaught exception [Debug]", OutputType::Info);
    Tokenizer::queue.clear();
    return "Uncaught exception [TEMP Debug Info]";
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