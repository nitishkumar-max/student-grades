#include "Person.h"
#include "InputUtils.h"

#include <algorithm>
#include <iomanip>
#include <random>
#include <utility>

namespace {
std::mt19937& randomEngine() {
    static std::mt19937 engine{std::random_device{}()};
    return engine;
}

int randomGrade() {
    static std::uniform_int_distribution<int> dist(1, 10);
    return dist(randomEngine());
}
}  // namespace

OutputFormat Person::format;

Person::Person() : exam_(0), finalAvg_(0.0), finalMed_(0.0) {}

Person::Person(std::string name, std::string surname,
               std::vector<int> homework, int exam)
    : name_(std::move(name)), surname_(std::move(surname)),
      homework_(std::move(homework)), exam_(exam),
      finalAvg_(0.0), finalMed_(0.0) {}

Person::Person(const Person& other)
    : name_(other.name_), surname_(other.surname_),
      homework_(other.homework_), exam_(other.exam_),
      finalAvg_(other.finalAvg_), finalMed_(other.finalMed_) {}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        name_ = other.name_;
        surname_ = other.surname_;
        homework_ = other.homework_;
        exam_ = other.exam_;
        finalAvg_ = other.finalAvg_;
        finalMed_ = other.finalMed_;
    }
    return *this;
}

Person::~Person() {
    homework_.clear();
}

double Person::average(const std::vector<int>& values) {
    if (values.empty()) return 0.0;
    double sum = 0.0;
    for (int v : values) sum += v;
    return sum / static_cast<double>(values.size());
}

double Person::median(std::vector<int> values) {
    if (values.empty()) return 0.0;
    std::sort(values.begin(), values.end());
    std::size_t mid = values.size() / 2;
    if (values.size() % 2 == 0) return (values[mid - 1] + values[mid]) / 2.0;
    return values[mid];
}

double Person::calculateFinal(GradeMethod method) {
    if (method != GradeMethod::Median)
        finalAvg_ = 0.4 * average(homework_) + 0.6 * exam_;
    if (method != GradeMethod::Average)
        finalMed_ = 0.4 * median(homework_) + 0.6 * exam_;
    return method == GradeMethod::Median ? finalMed_ : finalAvg_;
}

void Person::randomizeGrades(std::size_t hwCount) {
    homework_.clear();
    homework_.reserve(hwCount);
    for (std::size_t i = 0; i < hwCount; ++i) homework_.push_back(randomGrade());
    exam_ = randomGrade();
}

// Input.
// Keyboard: the user is guided with prompts and types homework results one by
// one; an empty line ends the list, so their number is not known in advance.
// Other streams: one record "Name Surname hw1 hw2 ... hwn exam" (the last
// number is the exam result).
std::istream& operator>>(std::istream& is, Person& p) {
    std::string name, surname;
    std::vector<int> hw;
    int exam = 0;

    if (&is == &std::cin) {
        name = readWord(is, "  First name: ");
        surname = readWord(is, "  Surname: ");
        std::cout << "  Homework results (1-10), one per line. "
                     "Press Enter on an empty line to finish.\n";
        while (true) {
            std::string line = readLine(is, "    HW" + std::to_string(hw.size() + 1) + ": ");
            if (line.find_first_not_of(" \t") == std::string::npos) {
                if (!hw.empty()) break;
                std::cout << "    Enter at least one homework result.\n";
                continue;
            }
            int value = 0;
            if (parseInt(line, value) && value >= 1 && value <= 10)
                hw.push_back(value);
            else
                std::cout << "    Please enter a whole number from 1 to 10.\n";
        }
        exam = readInt(is, "  Exam result (1-10): ", 1, 10);
    } else {
        if (!(is >> name >> surname)) return is;
        int value;
        while (is >> value) hw.push_back(value);
        if (hw.empty()) {                // no numbers at all: invalid record
            is.setstate(std::ios::failbit);
            return is;
        }
        is.clear(is.rdstate() & ~std::ios::failbit);
        exam = hw.back();
        hw.pop_back();
    }

    p = Person(name, surname, hw, exam);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Person& p) {
    const OutputFormat& f = Person::format;
    os << std::left << std::setw(f.nameWidth) << p.name_
       << std::setw(f.surnameWidth) << p.surname_
       << std::right << std::fixed << std::setprecision(2);
    switch (f.method) {
        case GradeMethod::Average: os << std::setw(18) << p.finalAvg_; break;
        case GradeMethod::Median:  os << std::setw(18) << p.finalMed_; break;
        case GradeMethod::Both:
            os << std::setw(12) << p.finalAvg_ << " | " << std::setw(12) << p.finalMed_;
            break;
    }
    return os;
}
