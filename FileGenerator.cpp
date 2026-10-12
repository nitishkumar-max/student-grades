#include "FileGenerator.h"
#include "Exceptions.h"

#include <charconv>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace {
constexpr std::size_t kFlushSize = 1 << 16;

void appendNumber(std::string& out, std::size_t value) {
    char digits[24];
    auto result = std::to_chars(digits, digits + sizeof digits, value);
    out.append(digits, result.ptr);
}
}  // namespace

StudentFileGenerator::StudentFileGenerator(std::size_t homeworkCount)
    : hwCount_(homeworkCount), engine_(std::random_device{}()) {
    if (hwCount_ == 0)
        throw std::invalid_argument("at least one homework result per student is needed");
}

std::string StudentFileGenerator::fileNameFor(std::size_t count) {
    return "students" + std::to_string(count) + ".txt";
}

void StudentFileGenerator::generate(const std::string& path, std::size_t count) {
    std::vector<char> buffer(1 << 20);
    std::ofstream out;
    out.rdbuf()->pubsetbuf(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    out.open(path);
    if (!out) throw FileCreateError(path);

    std::uniform_int_distribution<int> grade(1, 10);
    std::string chunk;
    chunk.reserve(kFlushSize + 512);

    chunk = "Name Surname";
    for (std::size_t h = 1; h <= hwCount_; ++h) {
        chunk += " HW";
        appendNumber(chunk, h);
    }
    chunk += " Exam\n";

    for (std::size_t i = 1; i <= count; ++i) {
        chunk += "Name";
        appendNumber(chunk, i);
        chunk += " Surname";
        appendNumber(chunk, i);
        for (std::size_t h = 0; h <= hwCount_; ++h) {   // homework + exam
            chunk += ' ';
            appendNumber(chunk, static_cast<std::size_t>(grade(engine_)));
        }
        chunk += '\n';
        if (chunk.size() >= kFlushSize) {
            out.write(chunk.data(), static_cast<std::streamsize>(chunk.size()));
            chunk.clear();
        }
    }
    out.write(chunk.data(), static_cast<std::streamsize>(chunk.size()));
    out.flush();
    if (!out) throw FileIOError("writing to", path);
}
