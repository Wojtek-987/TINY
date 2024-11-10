#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "Tiny.h"

std::string get_timestamp();
std::string create_log_file(const Tiny& tiny);
void append_to_log(const Tiny& tiny, const std::string& line, const std::string& log_file_path);
bool clear_log_file(const Tiny& tiny, const string& log_file);

#endif // LOGGER_H