#ifndef PERSON_H
#define PERSON_H

#include <cstddef>
#include <iostream>
#include <string>

// One student: name, surname, homework results, exam result and final grade.
// Homework results are kept in a dynamically allocated array, so the class
// implements the rule of three (copy constructor, copy assignment, destructor).
class Person {
public:
    Person();
    Person(const std::string& name, const std::string& surname,
           const int* homework, std::size_t hwCount, int exam);
    Person(const Person& other);              // rule of three: copy constructor
    Person& operator=(const Person& other);   // rule of three: copy assignment
    ~Person();                                // rule of three: destructor

    const std::string& name() const { return name_; }
    const std::string& surname() const { return surname_; }
    std::size_t homeworkCount() const { return hwCount_; }
    int exam() const { return exam_; }
    double finalGrade() const { return final_; }

    // Final = 0.4 * HW + 0.6 * Exam, HW = average or median of homework.
    double calculateFinal(bool useMedian);

    double homeworkAverage() const;
    double homeworkMedian() const;

    friend std::istream& operator>>(std::istream& is, Person& p);
    friend std::ostream& operator<<(std::ostream& os, const Person& p);

private:
    std::string name_;
    std::string surname_;
    int* homework_;
    std::size_t hwCount_;
    int exam_;
    double final_;
};

#endif
