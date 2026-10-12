#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include "Exceptions.h"

#include <iostream>
#include <sstream>
#include <string>

// Small helpers for safe console input.

// Reads one full line from `is`, shows `prompt` first (when not empty) and
// strips a trailing '\r' (files saved on Windows).
// Throws InputEnded when there is nothing more to read.
inline std::string readLine(std::istream& is, const std::string& prompt = "") {
    if (!prompt.empty()) std::cout << prompt;
    std::string line;
    if (!std::getline(is, line)) throw InputEnded();
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return line;
}

// Tries to read exactly one integer from `text`.
inline bool parseInt(const std::string& text, int& value) {
    std::istringstream ss(text);
    std::string extra;
    return static_cast<bool>(ss >> value) && !(ss >> extra);
}

// Keeps asking until the user types a whole number in [lo, hi].
inline int readInt(std::istream& is, const std::string& prompt, int lo, int hi) {
    while (true) {
        int value = 0;
        if (parseInt(readLine(is, prompt), value) && value >= lo && value <= hi)
            return value;
        std::cout << "  Please enter a whole number from " << lo << " to " << hi << ".\n";
    }
}

// Keeps asking until the user types a non-empty single word.
inline std::string readWord(std::istream& is, const std::string& prompt) {
    while (true) {
        std::istringstream ss(readLine(is, prompt));
        std::string word, extra;
        if (ss >> word && !(ss >> extra)) return word;
        std::cout << "  Please enter one word (no spaces).\n";
    }
}

#endif
