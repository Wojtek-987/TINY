#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include "Tiny.h"

std::string get_timestamp();
std::string create_log_file(Tiny& tiny);
void append_to_log(const std::string& line, const std::string& log_file_path, Tiny& tiny);
void clear_log_file(Tiny& tiny, const string& log_file);

#endif // LOGGER_H