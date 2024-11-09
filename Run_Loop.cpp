#include "headers/Run_Loop.h"
#include "headers/Logger.h"
#include "headers/Language.h"
#include "headers/Tiny.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

void run_io_loop(Tiny& tiny, const string& command = "") {
    string user_command = tiny.sterilize_input(command);

    tiny.prompt_input();
    if (command.empty()) {
        std::getline(std::cin, user_command);
    } else {
        std::istringstream command_stream(command);
        std::getline(command_stream, user_command);
        std::cout << command << std::endl;
    }

    append_to_log(tiny.get_name() + " < " + command, log_file_path, tiny);

    // Process user input
    tiny.tokenize_input(user_command);

    // Generate output
    tiny.process_token_queue();

    const bool is_clean = Language::is_clean(user_command);
    const string output = is_clean ? "true" : "false";

    // Show whether input is clean (debug) TEMP
    tiny.return_output(output, OutputType::Output);

    append_to_log(tiny.get_name() + " > " + output, log_file_path, tiny);

    if (user_command == "exit") // TEMP
        tiny.is_running = false;
}

void run_file(const string& path, Tiny& tiny) {
    // Check if the path exists, is a regular file, and has the ".tiny" extension
    if (const fs::path file_path = path; exists(file_path) && is_regular_file(file_path) && file_path.extension() == ".tiny") {
        std::ifstream file(path);

        // Check if the file was opened successfully
        if (!file.is_open()) {
            tiny.return_output("File could not be opened.", OutputType::Error);
        }

        // Run run_io_loop for all lines
        string line;
        while (std::getline(file, line)) {
            run_io_loop(tiny, line);
        }

        file.close();
        tiny.return_output("File read complete!", OutputType::Info);
        tiny.return_output("File output: " + std::to_string(tiny.last), OutputType::Info);
    } else {
        tiny.return_output("File doesn't exist or invalid path.", OutputType::Error);
    }
}
