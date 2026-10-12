#include "Benchmark.h"
#include "Container.h"
#include "StudentFile.h"
#include "Timer.h"

#include <iomanip>

std::string Benchmark::outputName(const std::string& inputPath, const std::string& suffix) {
    std::size_t slash = inputPath.find_last_of("/\\");
    std::size_t dot = inputPath.find_last_of('.');
    bool hasExtension = dot != std::string::npos && (slash == std::string::npos || dot > slash);
    std::string stem = hasExtension ? inputPath.substr(0, dot) : inputPath;
    return stem + "_" + suffix + ".txt";
}

PipelineResult Benchmark::run(const std::string& path, GradeMethod method, SortKey key) {
    PipelineResult r;
    r.file = path;

    Timer total;
    Timer step;

    StudentContainer all;
    r.skippedLines = StudentFile::read(path, all, method);
    r.students = all.size();
    r.read = step.elapsed();

    step.restart();
    StudentProcessor::sort(all, key, method);
    r.sort = step.elapsed();

    step.restart();
    StudentContainer passed, failed;
    StudentProcessor::split(all, passed, failed, method);
    r.split = step.elapsed();
    r.passed = passed.size();
    r.failed = failed.size();

    step.restart();
    StudentFile::write(outputName(path, "passed"), passed, method);
    StudentFile::write(outputName(path, "failed"), failed, method);
    r.write = step.elapsed();

    r.total = total.elapsed();
    return r;
}

void Benchmark::printDetails(std::ostream& os, const PipelineResult& r) {
    os << std::fixed << std::setprecision(3)
       << "\nFile: " << r.file << "  (container: " << kContainerName << ")\n"
       << "  Students: " << r.students << "  (passed " << r.passed
       << ", failed " << r.failed;
    if (r.skippedLines) os << ", " << r.skippedLines << " invalid lines skipped";
    os << ")\n"
       << "  Read file:        " << r.read << " s\n"
       << "  Sort:             " << r.sort << " s\n"
       << "  Split in two:     " << r.split << " s\n"
       << "  Write two files:  " << r.write << " s\n"
       << "  Total:            " << r.total << " s\n"
       << "Output: " << outputName(r.file, "passed") << ", "
       << outputName(r.file, "failed") << "\n";
}

void Benchmark::printTable(std::ostream& os, const std::vector<PipelineResult>& results,
                           GradeMethod method, SortKey key) {
    os << "\nContainer: " << kContainerName << ", final grade: " << toString(method)
       << ", sorted by: " << toString(key) << " (times in seconds)\n\n"
       << "| File | Students | Passed | Failed | Read | Sort | Split | Write | Total |\n"
       << "|------|---------:|-------:|-------:|-----:|-----:|------:|------:|------:|\n"
       << std::fixed << std::setprecision(3);
    for (const PipelineResult& r : results) {
        os << "| " << r.file << " | " << r.students << " | " << r.passed << " | " << r.failed
           << " | " << r.read << " | " << r.sort << " | " << r.split << " | " << r.write
           << " | " << r.total << " |\n";
    }
}
