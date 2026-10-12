#ifndef CONTAINER_H
#define CONTAINER_H

// The one place that decides which container holds the students.
// Everything else in the program uses only StudentContainer.

#include "Person.h"

#include <algorithm>
#include <cstddef>
#include <vector>

using StudentContainer = std::vector<Person>;

inline constexpr const char* kContainerName = "std::vector";

inline void reserveStudents(StudentContainer& students, std::size_t count) {
    students.reserve(count);
}

template <typename Compare>
void sortStudents(StudentContainer& students, Compare comp) {
    std::sort(students.begin(), students.end(), comp);
}

// Frees the memory too (clear() alone keeps a vector's capacity).
inline void releaseStudents(StudentContainer& students) {
    StudentContainer().swap(students);
}

#endif
