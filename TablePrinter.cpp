#include "TablePrinter.h"

#include <algorithm>
#include <string>

void TablePrinter::print(std::ostream& os, const StudentContainer& students, GradeMethod method) {
    Person::format.method = method;
    fitColumns(students);
    printHeader(os);
    for (const Person& p : students) os << p << '\n';
}

// Makes the name/surname columns wide enough for the longest value.
void TablePrinter::fitColumns(const StudentContainer& students) {
    std::size_t nameMax = 0, surnameMax = 0;
    for (const Person& p : students) {
        nameMax = std::max(nameMax, p.name().size());
        surnameMax = std::max(surnameMax, p.surname().size());
    }
    bool both = Person::format.method == GradeMethod::Both;
    Person::format.nameWidth = std::max<int>(both ? 10 : 11, static_cast<int>(nameMax) + 2);
    Person::format.surnameWidth = std::max<int>(both ? 15 : 14, static_cast<int>(surnameMax) + 2);
}

void TablePrinter::printHeader(std::ostream& os) {
    const OutputFormat& f = Person::format;
    std::string name = "Name", surname = "Surname";
    name.resize(f.nameWidth, ' ');
    surname.resize(f.surnameWidth, ' ');
    os << name << surname;
    switch (f.method) {
        case GradeMethod::Average: os << "Final_Point(Aver.)"; break;
        case GradeMethod::Median:  os << " Final_Point(Med.)"; break;
        case GradeMethod::Both:    os << "Final (Avg.) | Final (Med.)"; break;
    }
    int width = f.nameWidth + f.surnameWidth + (f.method == GradeMethod::Both ? 27 : 18);
    os << '\n' << std::string(width, '-') << '\n';
}
