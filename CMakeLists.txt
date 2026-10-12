cmake_minimum_required(VERSION 3.10)
project(StudentGrades CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(SOURCES
    main.cpp Application.cpp Benchmark.cpp FileGenerator.cpp Person.cpp
    StringUtils.cpp StudentFile.cpp StudentProcessor.cpp TablePrinter.cpp)

# The same sources, built three times with a different container.
add_executable(student_grades_vector ${SOURCES})

add_executable(student_grades_list ${SOURCES})
target_compile_definitions(student_grades_list PRIVATE STUDENT_CONTAINER_LIST)

add_executable(student_grades_deque ${SOURCES})
target_compile_definitions(student_grades_deque PRIVATE STUDENT_CONTAINER_DEQUE)
