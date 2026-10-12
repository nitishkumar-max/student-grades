#ifndef CONTAINER_H
#define CONTAINER_H

// The one place that decides which container holds the students.
// Everything else in the program uses only StudentContainer.
//
// Chosen at compile time:
//   (nothing)                     -> std::vector
//   -DSTUDENT_CONTAINER_LIST      -> std::list
//   -DSTUDENT_CONTAINER_DEQUE     -> std::deque

#include "Person.h"

#include <algorithm>
#include <cstddef>

#if defined(STUDENT_CONTAINER_LIST)
  #include <list>
  using StudentContainer = std::list<Person>;
  inline constexpr const char* kContainerName = "std::list";
#elif defined(STUDENT_CONTAINER_DEQUE)
  #include <deque>
  using StudentContainer = std::deque<Person>;
  inline constexpr const char* kContainerName = "std::deque";
#else
  #include <vector>
  using StudentContainer = std::vector<Person>;
  inline constexpr const char* kContainerName = "std::vector";
#endif

// Only std::vector can reserve memory in advance.
inline void reserveStudents(StudentContainer& students, std::size_t count) {
#if defined(STUDENT_CONTAINER_LIST) || defined(STUDENT_CONTAINER_DEQUE)
    (void)students;
    (void)count;
#else
    students.reserve(count);
#endif
}

// std::sort needs random-access iterators, which std::list does not have:
// it has its own member function sort().
template <typename Compare>
void sortStudents(StudentContainer& students, Compare comp) {
#if defined(STUDENT_CONTAINER_LIST)
    students.sort(comp);
#else
    std::sort(students.begin(), students.end(), comp);
#endif
}

// Frees the memory too (clear() alone keeps a vector's capacity).
inline void releaseStudents(StudentContainer& students) {
    StudentContainer().swap(students);
}

#endif
