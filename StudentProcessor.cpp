#include "StudentProcessor.h"
#include "StringUtils.h"

#include <utility>

const char* toString(SortKey key) {
    switch (key) {
        case SortKey::Name:    return "name";
        case SortKey::Surname: return "surname";
        default:               return "final grade";
    }
}

const char* toString(GradeMethod method) {
    switch (method) {
        case GradeMethod::Average: return "average";
        case GradeMethod::Median:  return "median";
        default:                   return "average and median";
    }
}

namespace {
bool nameThenSurname(const Person& x, const Person& y) {
    if (x.name() != y.name()) return naturalLess(x.name(), y.name());
    return naturalLess(x.surname(), y.surname());
}

bool surnameThenName(const Person& x, const Person& y) {
    if (x.surname() != y.surname()) return naturalLess(x.surname(), y.surname());
    return naturalLess(x.name(), y.name());
}
}  // namespace

void StudentProcessor::sort(StudentContainer& students, SortKey key, GradeMethod method) {
    switch (key) {
        case SortKey::Name:
            sortStudents(students, nameThenSurname);
            break;
        case SortKey::Surname:
            sortStudents(students, surnameThenName);
            break;
        case SortKey::FinalGrade:
            sortStudents(students, [method](const Person& x, const Person& y) {
                double fx = x.finalGrade(method), fy = y.finalGrade(method);
                if (fx != fy) return fx < fy;
                return surnameThenName(x, y);
            });
            break;
    }
}

void StudentProcessor::split(StudentContainer& all, StudentContainer& passed,
                             StudentContainer& failed, GradeMethod method) {
    for (Person& p : all) {
        if (isPassed(p, method)) passed.push_back(std::move(p));
        else                     failed.push_back(std::move(p));
    }
    releaseStudents(all);
}
