#include "InputUtils.h"
#include "Person.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

void printResults(const std::vector<Person>& students, GradeMethod method) {
    std::cout << "\n"
              << "Name       Surname       "
              << (method == GradeMethod::Median ? " Final_Point(Med.)" : "Final_Point(Aver.)")
              << "\n" << std::string(43, '-') << "\n";
    for (const Person& p : students) std::cout << p << "\n";
}

}  // namespace

int main() {
    std::cout << "Student final grade calculator v0.1\n"
              << "Final = 0.4 * homework (average or median) + 0.6 * exam\n\n"
              << "How do you want to enter the data?\n"
              << "  1 - Type names and all results by hand\n"
              << "  2 - Type names, generate results randomly\n"
              << "  3 - Generate names and results randomly\n"
              << "  0 - Exit\n";
    int mode = readInt(std::cin, "Your choice: ", 0, 3);
    if (mode == 0) return 0;

    std::cout << "\nCalculate the final grade by:\n"
              << "  1 - Average of homework\n"
              << "  2 - Median of homework\n";
    GradeMethod method = readInt(std::cin, "Your choice: ", 1, 2) == 2
                             ? GradeMethod::Median : GradeMethod::Average;

    std::vector<Person> students;

    if (mode == 3) {
        int count = readInt(std::cin, "\nHow many students to generate? ", 1, 1000000);
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
    } else {
        int hwCount = 0;
        if (mode == 2)
            hwCount = readInt(std::cin, "\nHow many homework results per student? ", 1, 100);
        std::cout << "\nEnter students. Leave the first name empty when you are done.\n";
        while (true) {
            std::cout << "\nStudent " << students.size() + 1 << ":\n";
            if (mode == 1) {
                std::string more = readLine(std::cin, "  Add this student? (Enter = yes, n = finish): ");
                if (more == "n" || more == "N") break;
                Person p;
                std::cin >> p;
                p.calculateFinal(method);
                students.push_back(p);
            } else {
                std::string name = readLine(std::cin, "  First name (empty = finish): ");
                if (name.find_first_not_of(" \t") == std::string::npos) break;
                Person p;
                p.setName(name.substr(name.find_first_not_of(" \t")));
                p.setSurname(readWord(std::cin, "  Surname: "));
                p.randomizeGrades(hwCount);
                p.calculateFinal(method);
                students.push_back(p);
            }
        }
    }

    if (students.empty()) {
        std::cout << "No students entered.\n";
        return 0;
    }
    printResults(students, method);
    return 0;
}
