#include "InputUtils.h"
#include "Person.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "Student final grade calculator\n"
              << "Final = 0.4 * homework average + 0.6 * exam\n\n";

    int count = readInt(std::cin, "How many students? ", 1, 1000);

    std::vector<Person> students;
    students.reserve(count);
    for (int i = 0; i < count; ++i) {
        std::cout << "\nStudent " << i + 1 << ":\n";
        Person p;
        std::cin >> p;
        p.calculateFinal(false);  // average
        students.push_back(p);
    }

    std::cout << "\n"
              << "Name       Surname       Final_Point(Aver.)\n"
              << std::string(43, '-') << "\n";
    for (const Person& p : students) std::cout << p << "\n";
    return 0;
}
