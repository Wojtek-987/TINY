#ifndef RUN_LOOP_H
#define RUN_LOOP_H

#include <string>
#include "Tiny.h"

class Tiny;

void run_io_loop(const Tiny& tiny, const std::string& command);
void run_file(const Tiny& tiny, const std::string& path);

extern std::string log_file_path;
void append_to_log(const Tiny& tiny, const std::string& line, const std::string& log_file_path);

#endif //RUN_LOOP_H
