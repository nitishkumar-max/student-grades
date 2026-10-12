#ifndef STUDENT_PROCESSOR_H
#define STUDENT_PROCESSOR_H

#include "Container.h"
#include "Person.h"

enum class SortKey { Name, Surname, FinalGrade };

const char* toString(SortKey key);
const char* toString(GradeMethod method);

// Sorting and splitting of a student container.
class StudentProcessor {
public:
    // A student with a final grade below this is "failed", otherwise "passed".
    static constexpr double kPassMark = 5.0;

    static bool isPassed(const Person& p, GradeMethod method) {
        return p.finalGrade(method) >= kPassMark;
    }

    static void sort(StudentContainer& students, SortKey key, GradeMethod method);

    // Moves every student of `all` into `passed` or `failed` (keeping the
    // order) and releases `all`.
    static void split(StudentContainer& all, StudentContainer& passed,
                      StudentContainer& failed, GradeMethod method);
};

#endif
