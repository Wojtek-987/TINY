#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "headers/Language.h"
#include "headers/Tiny.h"
#include "types/OutputType.h"
#include "headers/Logger.h"
#include "headers/Run_Loop.h"


using std::string;
using std::cout;
using std::getline;
using std::endl;

namespace fs = std::filesystem;


// Init interpreter globally
Tiny tiny;

string log_file_path = create_log_file(tiny);



void run_io_loop(const string& command = "") {

    string user_command = command;

    tiny.prompt_input();
    if (command.empty()) {
        getline(std::cin, user_command);
    } else {
        std::istringstream command_stream(command);
        std::getline(command_stream, user_command);
        cout << command << endl;
    }

    append_to_log("tiny < " + command, log_file_path, tiny);


    // process user input
    // generate output
    const bool is_clean = Language::is_clean(user_command);
    const string output = is_clean ? "true" : "false";

    // Show whether input is clean (debug)
    tiny.return_output(output, OutputType::Output);

    append_to_log("tiny > " + output, log_file_path, tiny);

    if (user_command == "exit")
        tiny.is_running = false;
}


int main(const int argc, char* argv[]) {

    // Check for a file path
    string file_path;
    if (argc > 1) {
        file_path = argv[1];
    }

    // Run a file (optional)
    if (!file_path.empty()) {
        run_file(file_path, tiny);

        // If user wants to terminate the interpreter process after running a file
        if (argc > 2 && string(argv[2]) == "final")
            return 1;
    }

    // Run the interpreter loop
    while (tiny.is_running) {
        run_io_loop();
    }

    return 0;
}