#include "Application.h"
#include "Benchmark.h"
#include "Container.h"
#include "Exceptions.h"
#include "FileGenerator.h"
#include "InputUtils.h"
#include "Person.h"
#include "StudentFile.h"
#include "StudentProcessor.h"
#include "TablePrinter.h"
#include "Timer.h"
#include "Version.h"

#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include <vector>

namespace {

const std::vector<std::size_t> kTestSizes{1000, 10000, 100000, 1000000, 10000000};

bool isBlank(const std::string& s) {
    return s.find_first_not_of(" \t") == std::string::npos;
}

std::string askFileName(const std::string& prompt, const std::string& fallback) {
    std::string name = readLine(std::cin, prompt);
    return isBlank(name) ? fallback : name;
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

// For the passed/failed split one final grade is needed, so no "Both".
GradeMethod askSingleMethod() {
    std::cout << "\nFinal grade (used to decide passed / failed):\n"
              << "  1 - Average of homework\n"
              << "  2 - Median of homework\n";
    return readInt(std::cin, "Your choice: ", 1, 2) == 1 ? GradeMethod::Average
                                                          : GradeMethod::Median;
}

SortKey askSortKey() {
    std::cout << "\nSort students by:\n  1 - First name\n  2 - Surname\n  3 - Final grade\n";
    switch (readInt(std::cin, "Your choice: ", 1, 3)) {
        case 1: return SortKey::Name;
        case 2: return SortKey::Surname;
        default: return SortKey::FinalGrade;
    }
}

void enterByHand(StudentContainer& students, GradeMethod method, bool randomGrades) {
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
        students.push_back(std::move(p));
    }
}

void generateAll(StudentContainer& students, GradeMethod method) {
    int count = readInt(std::cin, "\nHow many students to generate? ", 1, 10000000);
    int hwCount = readInt(std::cin, "How many homework results per student? ", 1, 100);
    reserveStudents(students, static_cast<std::size_t>(count));
    for (int i = 1; i <= count; ++i) {
        Person p;
        p.setName("Name" + std::to_string(i));
        p.setSurname("Surname" + std::to_string(i));
        p.randomizeGrades(static_cast<std::size_t>(hwCount));
        p.calculateFinal(method);
        students.push_back(std::move(p));
    }
}

}  // namespace

void Application::printMenu() const {
    std::cout << "\nHow do you want to proceed?\n"
              << "  1 - Type names and all results by hand\n"
              << "  2 - Type names, generate results randomly\n"
              << "  3 - Generate names and results randomly\n"
              << "  4 - Read from a file (e.g. Students.txt), sort and show\n"
              << "  5 - Generate the five test files (1 000 ... 10 000 000 students)\n"
              << "  6 - Read a file, sort, split into passed/failed, save two files (timed)\n"
              << "  7 - Speed test: option 6 on all five test files\n"
              << "  0 - Exit\n";
}

int Application::run() {
    std::cout << "Student final grade calculator " << kVersion
              << " (container: " << kContainerName << ")\n"
              << "Final = 0.4 * homework (average or median) + 0.6 * exam\n";
    while (true) {
        printMenu();
        int mode = readInt(std::cin, "Your choice: ", 0, 7);
        if (mode == 0) return 0;
        try {
            if (mode <= 4)      classicFlow(mode);
            else if (mode == 5) generateTestFiles();
            else if (mode == 6) processFile();
            else                speedTest();
        } catch (const InputEnded&) {
            throw;                                  // handled in main()
        } catch (const std::bad_alloc&) {
            std::cout << "\nError: not enough memory for this many students.\n";
        } catch (const std::exception& e) {
            std::cout << "\nError: " << e.what() << "\n";
        }
    }
}

// Modes 1-4: same behaviour as v0.1.
void Application::classicFlow(int mode) {
    StudentContainer students;
    GradeMethod method = GradeMethod::Both;

    if (mode == 4) {
        while (true) {
            std::string path = askFileName("\nFile name [Students.txt] (0 = back to menu): ",
                                           "Students.txt");
            if (path == "0") return;
            Timer timer;
            try {
                std::size_t skipped = StudentFile::read(path, students, method);
                std::cout << "Read " << students.size() << " students in "
                          << timer.elapsed() << " s";
                if (skipped) std::cout << " (" << skipped << " invalid lines skipped)";
                std::cout << ".\n";
                break;
            } catch (const FileOpenError& e) {
                std::cout << e.what() << "\n";      // ask for another file name
            }
        }
    } else {
        method = askMethod();
        if (mode == 3) generateAll(students, method);
        else           enterByHand(students, method, mode == 2);
    }

    if (students.empty()) {
        std::cout << "No students to show.\n";
        return;
    }

    std::cout << "\nSort students by:\n  1 - First name\n  2 - Surname\n";
    SortKey key = readInt(std::cin, "Your choice: ", 1, 2) == 2 ? SortKey::Surname : SortKey::Name;
    Timer timer;
    StudentProcessor::sort(students, key, method);
    if (students.size() > 1000)
        std::cout << "Sorted in " << timer.elapsed() << " s.\n";

    bool toFile = false;
    if (students.size() > 100) {
        std::cout << "\nThere are " << students.size() << " students. Show results:\n"
                  << "  1 - On the screen\n  2 - Save to a file\n";
        toFile = readInt(std::cin, "Your choice: ", 1, 2) == 2;
    }

    if (!toFile) {
        std::cout << "\n";
        TablePrinter::print(std::cout, students, method);
        return;
    }
    while (true) {
        std::string out = askFileName("Output file name [results.txt] (0 = skip saving): ",
                                      "results.txt");
        if (out == "0") return;
        timer.restart();
        try {
            StudentFile::write(out, students, method);
            std::cout << "Results saved to " << out << " in " << timer.elapsed() << " s.\n";
            return;
        } catch (const FileCreateError& e) {
            std::cout << e.what() << "\n";          // ask for another file name
        }
    }
}

void Application::generateTestFiles() {
    int hwCount = readInt(std::cin, "\nHow many homework results per student? [1-100]: ", 1, 100);
    StudentFileGenerator generator(static_cast<std::size_t>(hwCount));
    for (std::size_t count : kTestSizes) {
        std::string path = StudentFileGenerator::fileNameFor(count);
        std::cout << "Generating " << path << " ... " << std::flush;
        Timer timer;
        generator.generate(path, count);
        std::cout << "done in " << timer.elapsed() << " s.\n";
    }
}

void Application::processFile() {
    GradeMethod method = askSingleMethod();
    SortKey key = askSortKey();
    while (true) {
        std::string path = askFileName("\nFile name [students1000.txt] (0 = back to menu): ",
                                       StudentFileGenerator::fileNameFor(1000));
        if (path == "0") return;
        try {
            Benchmark::printDetails(std::cout, Benchmark::run(path, method, key));
            return;
        } catch (const FileOpenError& e) {
            std::cout << e.what() << "\n";          // ask for another file name
        }
    }
}

void Application::speedTest() {
    GradeMethod method = askSingleMethod();
    SortKey key = askSortKey();
    std::vector<PipelineResult> results;
    for (std::size_t count : kTestSizes) {
        std::string path = StudentFileGenerator::fileNameFor(count);
        std::cout << "\nProcessing " << path << " ..." << std::endl;
        try {
            results.push_back(Benchmark::run(path, method, key));
            Benchmark::printDetails(std::cout, results.back());
        } catch (const FileOpenError&) {
            std::cout << "  File not found - generate the test files first (menu option 5).\n";
        } catch (const std::bad_alloc&) {
            std::cout << "  Not enough memory for this file - skipped.\n";
        }
    }
    if (!results.empty()) Benchmark::printTable(std::cout, results, method, key);
}
