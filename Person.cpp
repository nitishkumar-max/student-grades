#include "Person.h"
#include "InputUtils.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <iomanip>
#include <random>
#include <stdexcept>
#include <utility>

namespace {
std::mt19937& randomEngine() {
    static std::mt19937 engine{std::random_device{}()};
    return engine;
}

int randomGrade() {
    static std::uniform_int_distribution<int> dist(Person::kMinGrade, Person::kMaxGrade);
    return dist(randomEngine());
}

bool validGrade(int g) { return g >= Person::kMinGrade && g <= Person::kMaxGrade; }
}  // namespace

OutputFormat Person::format;

Person::Person() : exam_(0), finalAvg_(0.0), finalMed_(0.0) {}

Person::Person(std::string name, std::string surname,
               std::vector<int> homework, int exam)
    : name_(std::move(name)), surname_(std::move(surname)),
      homework_(std::move(homework)), exam_(exam),
      finalAvg_(0.0), finalMed_(0.0) {
    if (!validGrade(exam_))
        throw std::invalid_argument("exam result must be from 1 to 10");
    if (!std::all_of(homework_.begin(), homework_.end(), validGrade))
        throw std::invalid_argument("homework results must be from 1 to 10");
}

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

Person::Person(Person&& other) noexcept
    : name_(std::move(other.name_)), surname_(std::move(other.surname_)),
      homework_(std::move(other.homework_)), exam_(other.exam_),
      finalAvg_(other.finalAvg_), finalMed_(other.finalMed_) {}

Person& Person::operator=(Person&& other) noexcept {
    if (this != &other) {
        name_ = std::move(other.name_);
        surname_ = std::move(other.surname_);
        homework_ = std::move(other.homework_);
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
    if (values.empty())
        throw std::domain_error("cannot calculate the average of no homework results");
    double sum = 0.0;
    for (int v : values) sum += v;
    return sum / static_cast<double>(values.size());
}

double Person::median(std::vector<int> values) {
    if (values.empty())
        throw std::domain_error("cannot calculate the median of no homework results");
    std::sort(values.begin(), values.end());
    std::size_t mid = values.size() / 2;
    // at() checks the index and throws std::out_of_range instead of reading
    // memory that does not belong to the vector.
    if (values.size() % 2 == 0) return (values.at(mid - 1) + values.at(mid)) / 2.0;
    return values.at(mid);
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

// Fast line parser used for files (no stringstream per line).
Person Person::parse(const std::string& line) {
    const char* p = line.data();
    const char* end = p + line.size();
    auto isSpace = [](char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; };
    auto skipSpaces = [&]() { while (p < end && isSpace(*p)) ++p; };
    auto word = [&]() {
        skipSpaces();
        const char* start = p;
        while (p < end && !isSpace(*p)) ++p;
        return std::string(start, p);
    };

    std::string name = word();
    std::string surname = word();
    if (name.empty() || surname.empty())
        throw std::invalid_argument("missing name or surname");

    // Reused between calls so that no vector is allocated per line; the
    // student's own vector is then built with exactly the right size.
    static thread_local std::vector<int> numbers;
    numbers.clear();
    while (true) {
        skipSpaces();
        if (p == end) break;
        int value = 0;
        auto [ptr, ec] = std::from_chars(p, end, value);
        if (ec != std::errc() || (ptr != end && !isSpace(*ptr)))
            throw std::invalid_argument("results must be whole numbers");
        numbers.push_back(value);
        p = ptr;
    }
    if (numbers.size() < 2)
        throw std::invalid_argument("a student needs homework results and an exam result");

    int exam = numbers.back();
    std::vector<int> homework(numbers.begin(), numbers.end() - 1);
    return Person(std::move(name), std::move(surname), std::move(homework), exam);
}

// Input.
// Keyboard: the user is guided with prompts and types homework results one by
// one; an empty line ends the list, so their number is not known in advance.
// Other streams: one record per line, "Name Surname hw1 hw2 ... hwn exam"
// (the last number is the exam result). A malformed line sets failbit.
std::istream& operator>>(std::istream& is, Person& p) {
    if (&is == &std::cin) {
        std::string name = readWord(is, "  First name: ");
        std::string surname = readWord(is, "  Surname: ");
        std::vector<int> hw;
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
            if (parseInt(line, value) && value >= Person::kMinGrade && value <= Person::kMaxGrade)
                hw.push_back(value);
            else
                std::cout << "    Please enter a whole number from 1 to 10.\n";
        }
        int exam = readInt(is, "  Exam result (1-10): ", Person::kMinGrade, Person::kMaxGrade);
        p = Person(std::move(name), std::move(surname), std::move(hw), exam);
        return is;
    }

    std::string line;
    if (!std::getline(is, line)) return is;
    try {
        p = Person::parse(line);
    } catch (const std::invalid_argument&) {
        is.setstate(std::ios::failbit);
    }
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
