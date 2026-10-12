#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

// All program-specific errors derive from StudentException, so one
// catch (const StudentException&) handles every one of them, while the
// more specific types can still be caught separately (e.g. to ask the user
// for another file name).
class StudentException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// The file does not exist or cannot be opened for reading.
class FileOpenError : public StudentException {
public:
    explicit FileOpenError(const std::string& path)
        : StudentException("Could not open file \"" + path + "\" (does it exist?).") {}
};

// The output file cannot be created (bad path, no permission, ...).
class FileCreateError : public StudentException {
public:
    explicit FileCreateError(const std::string& path)
        : StudentException("Could not create file \"" + path + "\".") {}
};

// Reading or writing failed half-way (disk full, file removed, ...).
class FileIOError : public StudentException {
public:
    FileIOError(const std::string& action, const std::string& path)
        : StudentException("Error while " + action + " file \"" + path + "\".") {}
};

// The file was opened but does not contain a single valid student record.
class DataError : public StudentException {
public:
    explicit DataError(const std::string& path)
        : StudentException("File \"" + path + "\" contains no valid student records.") {}
};

// The keyboard input ended (Ctrl+D / Ctrl+Z, closed pipe).
class InputEnded : public StudentException {
public:
    InputEnded() : StudentException("Input ended.") {}
};

#endif
