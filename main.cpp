#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "headers/Tiny.h"
#include "headers/Logger.h"
#include "headers/Run_Loop.h"


using std::string;
using std::cout;
using std::getline;
using std::endl;

namespace fs = std::filesystem;


// Init interpreter globally
const Tiny tiny;

string log_file_path = create_log_file(tiny);


int main(const int argc, char* argv[]) {

    // === FIX all "TEMP" comment occurrences by implementing the features correctly ===

    // Check for a file path
    string file_path;
    if (argc > 1) {
        file_path = argv[1];
    }

    // Run a file (optional)
    if (!file_path.empty()) {
        run_file(tiny, file_path);

        // If user wants to terminate the interpreter process after running a file
        if (argc > 2 && string(argv[2]) == "final")
            return 1;
    }

    // Run the interpreter loop
    while (tiny.is_running) {
        run_io_loop(tiny, "");
    }

    return 0;
}