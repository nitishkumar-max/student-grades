#ifndef TABLE_PRINTER_H
#define TABLE_PRINTER_H

#include "Container.h"
#include "Person.h"

#include <ostream>

// Prints students as a formatted table (header, separator line, one row each).
class TablePrinter {
public:
    static void print(std::ostream& os, const StudentContainer& students, GradeMethod method);

private:
    static void fitColumns(const StudentContainer& students);
    static void printHeader(std::ostream& os);
};

#endif
