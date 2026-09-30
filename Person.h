#ifndef PERSON_H
#define PERSON_H

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// How the homework part of the final grade is calculated / displayed.
enum class GradeMethod { Average, Median, Both };

// Column layout used by operator<< (shared by all Person objects).
struct OutputFormat {
    GradeMethod method = GradeMethod::Average;
    int nameWidth = 11;
    int surnameWidth = 14;
};

// One student: name, surname, homework results, exam result and final grade.
// Homework results are stored in a std::vector, so the number of results does
// not need to be known in advance. The rule of three is still written out
// explicitly as required by the task.
class Person {
public:
    Person();
    Person(std::string name, std::string surname,
           std::vector<int> homework, int exam);
    Person(const Person& other);              // rule of three: copy constructor
    Person& operator=(const Person& other);   // rule of three: copy assignment
    ~Person();                                // rule of three: destructor

    const std::string& name() const { return name_; }
    const std::string& surname() const { return surname_; }
    const std::vector<int>& homework() const { return homework_; }
    int exam() const { return exam_; }
    double finalAverage() const { return finalAvg_; }
    double finalMedian() const { return finalMed_; }

    void setName(const std::string& name) { name_ = name; }
    void setSurname(const std::string& surname) { surname_ = surname; }

    // Final = 0.4 * HW + 0.6 * Exam, HW = average or median of homework.
    // Average/Median calculate one value, Both calculates the two of them.
    double calculateFinal(GradeMethod method);

    // Fills homework (hwCount results) and exam with random grades 1..10.
    void randomizeGrades(std::size_t hwCount);

    static double average(const std::vector<int>& values);
    static double median(std::vector<int> values);

    static OutputFormat format;   // controls what operator<< prints

    friend std::istream& operator>>(std::istream& is, Person& p);
    friend std::ostream& operator<<(std::ostream& os, const Person& p);

private:
    std::string name_;
    std::string surname_;
    std::vector<int> homework_;
    int exam_;
    double finalAvg_;
    double finalMed_;
};

#endif
