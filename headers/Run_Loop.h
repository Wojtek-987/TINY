#ifndef RUN_LOOP_H
#define RUN_LOOP_H

#include <string>
#include "Tiny.h"

class Tiny;

void run_io_loop(Tiny& tiny, const std::string& command);
void run_file(const std::string& path, Tiny& tiny);

extern std::string log_file_path;
void append_to_log(const std::string& line, const std::string& log_file_path, Tiny& tiny);

#endif //RUN_LOOP_H
