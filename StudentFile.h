#ifndef STUDENT_FILE_H
#define STUDENT_FILE_H

#include "Container.h"
#include "Person.h"

#include <string>

// Reading students from, and writing them to, text files.
class StudentFile {
public:
    // Reads a file whose lines look like "Name Surname hw1 ... hwn exam" and
    // appends the students to `students` (final grades are calculated with
    // `method`). Lines that do not match (e.g. the header) are skipped;
    // returns how many invalid lines were skipped after the first line.
    // Throws FileOpenError, FileIOError or DataError.
    static std::size_t read(const std::string& path, StudentContainer& students,
                            GradeMethod method);

    // Writes the students as a formatted table.
    // Throws FileCreateError or FileIOError.
    static void write(const std::string& path, const StudentContainer& students,
                      GradeMethod method);
};

#endif
