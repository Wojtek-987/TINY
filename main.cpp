#include <iostream>
#include <string>
#include "headers/Language.h"
#include "headers/Tiny.h"
#include "types/OutputType.h"


using std::string;
using std::cout;
using std::cin;
using std::endl;

// add argument for *.tiny file line-by-line reading
int main() {
    // Init interpreter
    Tiny tiny;

    // Run the interpreter loop
    while(tiny.is_active) {
        string user_command;

        tiny.prompt_input();

        std::getline(std::cin, user_command);

        // process user input
        // generate output

        bool is_clean = Language::is_clean(user_command);
        string output = is_clean ? "true" : "false";

        tiny.return_output(output, OutputType::Output);

        // save logs to file

        if(user_command == "exit")
            tiny.is_active = false;
    }

    return 0;
}