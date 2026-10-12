#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "Person.h"
#include "StudentProcessor.h"

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

// Timings (in seconds) of one run of the whole pipeline.
struct PipelineResult {
    std::string file;
    std::size_t students = 0;
    std::size_t passed = 0;
    std::size_t failed = 0;
    std::size_t skippedLines = 0;
    double read = 0.0;    // read the file + calculate final grades
    double sort = 0.0;    // sort the students
    double split = 0.0;   // divide into "passed" and "failed"
    double write = 0.0;   // write the two output files
    double total = 0.0;   // everything above
};

class Benchmark {
public:
    // read file -> sort -> split into two groups -> write two files,
    // measuring every step. The output files are named after the input file:
    // "students1000.txt" -> "students1000_passed.txt" and "students1000_failed.txt".
    // Throws the exceptions of StudentFile::read / write.
    static PipelineResult run(const std::string& path, GradeMethod method, SortKey key);

    static void printDetails(std::ostream& os, const PipelineResult& r);
    // Markdown table, one row per result.
    static void printTable(std::ostream& os, const std::vector<PipelineResult>& results,
                           GradeMethod method, SortKey key);

    static std::string outputName(const std::string& inputPath, const std::string& suffix);
};

#endif
