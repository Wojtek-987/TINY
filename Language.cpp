#include "headers/Language.h"

const string Language::valid_chars = "abcdefghijklmnopqrstuvwxyz0123456789+-*/= ";

bool Language::is_clean(const string& line) {
    const unsigned long long line_length = line.length();
    const int valid_chars_length = valid_chars.length();

    // Check if all characters are valid
    for (unsigned long long i = 0; i < line_length; i++) {
        bool is_found = false;
        for (int j = 0; j < valid_chars_length; j++) {
            if (line[i] == valid_chars[j]) {
                is_found = true;
                break;
            }
        }

        if (!is_found) {
            // Found an invalid character
            return false;
        }
    }

    return true;
}