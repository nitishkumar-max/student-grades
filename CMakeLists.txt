cmake_minimum_required(VERSION 3.10)
project(StudentGrades CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(student_grades
    main.cpp Application.cpp Benchmark.cpp FileGenerator.cpp Person.cpp
    StringUtils.cpp StudentFile.cpp StudentProcessor.cpp TablePrinter.cpp)
