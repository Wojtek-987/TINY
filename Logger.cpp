#include "headers/Logger.h"
#include <fstream>
#include <chrono>
#include <filesystem>
#include "headers/Tiny.h"
#include "types/OutputType.h"

namespace fs = std::filesystem;



std::string get_timestamp() {
    // Get current time
    const auto now = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // Format time as YYYYMMDD_HHMMSS
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

std::string create_log_file(Tiny& tiny) {
    // Create logs folder and file name with timestamp
    const std::string folder = "logs";
    const std::string filename = "tiny-" + get_timestamp() + ".logs.txt";

    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }

    const std::string log_file_path = folder + "/" + filename;

    // Create and open the file
    if (std::ofstream log_file(log_file_path); log_file.is_open()) {
        log_file << "[" + get_timestamp() + "]: " + "Log file created." << std::endl;
        log_file.close();
        tiny.return_output("Log file created: " + folder + "/" + filename, OutputType::Info);
    } else {
        tiny.return_output("Could not create log file " + filename, OutputType::Error);
    }

    return log_file_path;
}

void append_to_log(const std::string& line, const std::string& log_file_path, Tiny& tiny) {
    if (std::ofstream log_file(log_file_path, std::ios::app); log_file.is_open()) {
        log_file << "[" + get_timestamp() + "]: " + line << std::endl;
        log_file.close();
    } else {
        tiny.return_output("Could not append to log file.", OutputType::Error);
    }
}

void clear_log_file(const Tiny& tiny, const string& log_file) {
    if (std::ofstream file(log_file, std::ios::trunc); file.is_open()) {
        file << "[" + get_timestamp() + "]: " + "Log file initialized." << std::endl;
        file.close();
        tiny.return_output("Log file initialized.", OutputType::Info);
    }
}