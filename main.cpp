#include "InputUtils.h"
#include "Person.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

double secondsSince(Clock::time_point start) {
    return std::chrono::duration<double>(Clock::now() - start).count();
}

// "Natural" string order: digits are compared as numbers, so Name2 comes
// before Name10. Letters are compared without regard to case.
bool naturalLess(const std::string& a, const std::string& b) {
    std::size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        unsigned char ca = a[i], cb = b[j];
        if (std::isdigit(ca) && std::isdigit(cb)) {
            std::size_t ie = i, je = j;
            while (ie < a.size() && std::isdigit(static_cast<unsigned char>(a[ie]))) ++ie;
            while (je < b.size() && std::isdigit(static_cast<unsigned char>(b[je]))) ++je;
            std::size_t is = i, js = j;                     // skip leading zeros
            while (is + 1 < ie && a[is] == '0') ++is;
            while (js + 1 < je && b[js] == '0') ++js;
            if (ie - is != je - js) return ie - is < je - js;
            int cmp = a.compare(is, ie - is, b, js, je - js);
            if (cmp != 0) return cmp < 0;
            i = ie;
            j = je;
        } else {
            int la = std::tolower(ca), lb = std::tolower(cb);
            if (la != lb) return la < lb;
            ++i;
            ++j;
        }
    }
    return a.size() - i < b.size() - j;
}

void sortStudents(std::vector<Person>& students, bool bySurname) {
    std::sort(students.begin(), students.end(),
              [bySurname](const Person& x, const Person& y) {
                  const std::string& x1 = bySurname ? x.surname() : x.name();
                  const std::string& y1 = bySurname ? y.surname() : y.name();
                  if (x1 != y1) return naturalLess(x1, y1);
                  const std::string& x2 = bySurname ? x.name() : x.surname();
                  const std::string& y2 = bySurname ? y.name() : y.surname();
                  return naturalLess(x2, y2);
              });
}

// Reads a file whose lines look like "Name Surname hw1 ... hwn exam".
// Lines that do not match (e.g. the header line) are skipped.
bool readFromFile(const std::string& path, std::vector<Person>& students,
                  std::size_t& skipped) {
    std::ifstream in(path);
    if (!in) return false;

    // Read the whole file at once: much faster for large files.
    std::stringstream buffer;
    buffer << in.rdbuf();

    std::string line;
    skipped = 0;
    bool first = true;
    while (std::getline(buffer, line)) {
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
        std::istringstream ls(line);
        Person p;
        if (ls >> p) {
            p.calculateFinal(GradeMethod::Both);
            students.push_back(p);
        } else if (!first) {
            ++skipped;   // the first line is normally the header
        }
        first = false;
    }
    return true;
}

void printHeader(std::ostream& os) {
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
    os << "\n" << std::string(width, '-') << "\n";
}

void printTable(std::ostream& os, const std::vector<Person>& students) {
    printHeader(os);
    for (const Person& p : students) os << p << "\n";
}

// Makes the name/surname columns wide enough for the longest value.
void fitColumns(const std::vector<Person>& students) {
    std::size_t nameMax = 0, surnameMax = 0;
    for (const Person& p : students) {
        nameMax = std::max(nameMax, p.name().size());
        surnameMax = std::max(surnameMax, p.surname().size());
    }
    bool both = Person::format.method == GradeMethod::Both;
    Person::format.nameWidth = std::max<int>(both ? 10 : 11, static_cast<int>(nameMax) + 2);
    Person::format.surnameWidth = std::max<int>(both ? 15 : 14, static_cast<int>(surnameMax) + 2);
}

GradeMethod askMethod() {
    std::cout << "\nCalculate the final grade by:\n"
              << "  1 - Average of homework\n"
              << "  2 - Median of homework\n"
              << "  3 - Both (two columns)\n";
    switch (readInt(std::cin, "Your choice: ", 1, 3)) {
        case 1: return GradeMethod::Average;
        case 2: return GradeMethod::Median;
        default: return GradeMethod::Both;
    }
}

void enterByHand(std::vector<Person>& students, GradeMethod method, bool randomGrades) {
    int hwCount = 0;
    if (randomGrades)
        hwCount = readInt(std::cin, "\nHow many homework results per student? ", 1, 100);
    std::cout << "\nEnter students. Leave the first name empty when you are done.\n";
    while (true) {
        std::cout << "\nStudent " << students.size() + 1 << ":\n";
        Person p;
        if (!randomGrades) {
            std::string more = readLine(std::cin, "  Add this student? (Enter = yes, n = finish): ");
            if (more == "n" || more == "N") break;
            std::cin >> p;
        } else {
            std::string name = readLine(std::cin, "  First name (empty = finish): ");
            std::istringstream ss(name);
            std::string word;
            if (!(ss >> word)) break;
            p.setName(word);
            p.setSurname(readWord(std::cin, "  Surname: "));
            p.randomizeGrades(hwCount);
        }
        p.calculateFinal(method);
        students.push_back(p);
    }
}

void generateAll(std::vector<Person>& students, GradeMethod method) {
    int count = readInt(std::cin, "\nHow many students to generate? ", 1, 10000000);
    int hwCount = readInt(std::cin, "How many homework results per student? ", 1, 100);
    students.reserve(count);
    for (int i = 1; i <= count; ++i) {
        Person p;
        p.setName("Name" + std::to_string(i));
        p.setSurname("Surname" + std::to_string(i));
        p.randomizeGrades(hwCount);
        p.calculateFinal(method);
        students.push_back(p);
    }
}

}  // namespace

int main() {
    std::cout << "Student final grade calculator v0.1\n"
              << "Final = 0.4 * homework (average or median) + 0.6 * exam\n\n"
              << "How do you want to enter the data?\n"
              << "  1 - Type names and all results by hand\n"
              << "  2 - Type names, generate results randomly\n"
              << "  3 - Generate names and results randomly\n"
              << "  4 - Read from a file (e.g. Students.txt)\n"
              << "  0 - Exit\n";
    int mode = readInt(std::cin, "Your choice: ", 0, 4);
    if (mode == 0) return 0;

    std::vector<Person> students;
    GradeMethod method = GradeMethod::Both;

    if (mode == 4) {
        std::string path = readLine(std::cin, "\nFile name [Students.txt]: ");
        if (path.find_first_not_of(" \t") == std::string::npos) path = "Students.txt";
        auto start = Clock::now();
        std::size_t skipped = 0;
        if (!readFromFile(path, students, skipped)) {
            std::cout << "Could not open file \"" << path << "\".\n";
            return 1;
        }
        std::cout << "Read " << students.size() << " students in "
                  << secondsSince(start) << " s";
        if (skipped) std::cout << " (" << skipped << " invalid lines skipped)";
        std::cout << ".\n";
    } else {
        method = askMethod();
        if (mode == 3) generateAll(students, method);
        else enterByHand(students, method, mode == 2);
    }

    if (students.empty()) {
        std::cout << "No students to show.\n";
        return 0;
    }

    std::cout << "\nSort students by:\n  1 - First name\n  2 - Surname\n";
    bool bySurname = readInt(std::cin, "Your choice: ", 1, 2) == 2;
    auto start = Clock::now();
    sortStudents(students, bySurname);
    if (students.size() > 1000)
        std::cout << "Sorted in " << secondsSince(start) << " s.\n";

    Person::format.method = method;
    fitColumns(students);

    bool toFile = false;
    if (students.size() > 100) {
        std::cout << "\nThere are " << students.size() << " students. Show results:\n"
                  << "  1 - On the screen\n  2 - Save to a file\n";
        toFile = readInt(std::cin, "Your choice: ", 1, 2) == 2;
    }

    if (toFile) {
        std::string out = readLine(std::cin, "Output file name [results.txt]: ");
        if (out.find_first_not_of(" \t") == std::string::npos) out = "results.txt";
        std::ofstream file(out);
        if (!file) {
            std::cout << "Could not create file \"" << out << "\".\n";
            return 1;
        }
        start = Clock::now();
        printTable(file, students);
        std::cout << "Results saved to " << out << " in " << secondsSince(start) << " s.\n";
    } else {
        std::cout << "\n";
        printTable(std::cout, students);
    }
    return 0;
}
