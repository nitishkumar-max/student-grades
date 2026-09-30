#include "Person.h"
#include "InputUtils.h"

#include <algorithm>
#include <iomanip>
#include <vector>

Person::Person()
    : homework_(nullptr), hwCount_(0), exam_(0), final_(0.0) {}

Person::Person(const std::string& name, const std::string& surname,
               const int* homework, std::size_t hwCount, int exam)
    : name_(name), surname_(surname),
      homework_(hwCount ? new int[hwCount] : nullptr), hwCount_(hwCount),
      exam_(exam), final_(0.0) {
    std::copy(homework, homework + hwCount, homework_);
}

Person::Person(const Person& other)
    : name_(other.name_), surname_(other.surname_),
      homework_(other.hwCount_ ? new int[other.hwCount_] : nullptr),
      hwCount_(other.hwCount_), exam_(other.exam_), final_(other.final_) {
    std::copy(other.homework_, other.homework_ + other.hwCount_, homework_);
}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        int* copy = other.hwCount_ ? new int[other.hwCount_] : nullptr;
        std::copy(other.homework_, other.homework_ + other.hwCount_, copy);
        delete[] homework_;
        homework_ = copy;
        hwCount_ = other.hwCount_;
        name_ = other.name_;
        surname_ = other.surname_;
        exam_ = other.exam_;
        final_ = other.final_;
    }
    return *this;
}

Person::~Person() {
    delete[] homework_;
}

double Person::homeworkAverage() const {
    if (hwCount_ == 0) return 0.0;
    double sum = 0.0;
    for (std::size_t i = 0; i < hwCount_; ++i) sum += homework_[i];
    return sum / static_cast<double>(hwCount_);
}

double Person::homeworkMedian() const {
    if (hwCount_ == 0) return 0.0;
    std::vector<int> sorted(homework_, homework_ + hwCount_);
    std::sort(sorted.begin(), sorted.end());
    std::size_t mid = hwCount_ / 2;
    if (hwCount_ % 2 == 0) return (sorted[mid - 1] + sorted[mid]) / 2.0;
    return sorted[mid];
}

double Person::calculateFinal(bool useMedian) {
    double hw = useMedian ? homeworkMedian() : homeworkAverage();
    final_ = 0.4 * hw + 0.6 * exam_;
    return final_;
}

// Input: when reading from the keyboard the user is guided with prompts;
// from any other stream the format is: Name Surname n hw1 ... hwn exam
std::istream& operator>>(std::istream& is, Person& p) {
    std::string name, surname;
    int n = 0, exam = 0;
    int* hw = nullptr;

    if (&is == &std::cin) {
        name = readWord(is, "  First name: ");
        surname = readWord(is, "  Surname: ");
        n = readInt(is, "  Number of homework results: ", 1, 100);
        hw = new int[n];
        for (int i = 0; i < n; ++i)
            hw[i] = readInt(is, "    HW" + std::to_string(i + 1) + " (1-10): ", 1, 10);
        exam = readInt(is, "  Exam result (1-10): ", 1, 10);
    } else {
        if (!(is >> name >> surname >> n) || n < 0) {
            is.setstate(std::ios::failbit);
            return is;
        }
        hw = new int[n > 0 ? n : 1];
        for (int i = 0; i < n; ++i) is >> hw[i];
        is >> exam;
        if (!is) { delete[] hw; return is; }
    }

    p = Person(name, surname, hw, static_cast<std::size_t>(n), exam);
    delete[] hw;
    return is;
}

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << std::left << std::setw(11) << p.name_
       << std::setw(14) << p.surname_
       << std::right << std::setw(18) << std::fixed << std::setprecision(2)
       << p.final_;
    return os;
}
