#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include <cstddef>
#include <random>
#include <string>

// Generates test files in the format the program reads:
//   Name Surname HW1 ... HWn Exam
//   Name1 Surname1 8 9 10 6 10 9
//   Name2 Surname2 ...
class StudentFileGenerator {
public:
    explicit StudentFileGenerator(std::size_t homeworkCount = 5);

    // Creates a file with `count` random students.
    // Throws std::invalid_argument, FileCreateError or FileIOError.
    void generate(const std::string& path, std::size_t count);

    // "students10000.txt" for 10000.
    static std::string fileNameFor(std::size_t count);

private:
    std::size_t hwCount_;
    std::mt19937 engine_;
};

#endif
