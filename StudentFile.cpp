#include "StudentFile.h"
#include "Exceptions.h"
#include "TablePrinter.h"

#include <fstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
constexpr std::size_t kFileBufferSize = 1 << 20;   // 1 MB stream buffer
}

std::size_t StudentFile::read(const std::string& path, StudentContainer& students,
                              GradeMethod method) {
    // A big buffer makes reading large files noticeably faster. It is
    // declared before the stream, so it outlives the stream.
    std::vector<char> buffer(kFileBufferSize);
    std::ifstream in;
    in.rdbuf()->pubsetbuf(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    in.open(path);
    if (!in) throw FileOpenError(path);

    std::string line;
    std::size_t skipped = 0, valid = 0;
    bool first = true;
    while (std::getline(in, line)) {
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
        try {
            Person p = Person::parse(line);
            p.calculateFinal(method);
            students.push_back(std::move(p));
            ++valid;
        } catch (const std::logic_error&) {
            // invalid_argument (bad line) and domain_error (no homework)
            // both derive from logic_error. The first line is normally
            // the header, so it is not counted as an error.
            if (!first) ++skipped;
        }
        first = false;
    }
    if (in.bad()) throw FileIOError("reading", path);
    if (valid == 0) throw DataError(path);
    return skipped;
}

void StudentFile::write(const std::string& path, const StudentContainer& students,
                        GradeMethod method) {
    std::vector<char> buffer(kFileBufferSize);
    std::ofstream out;
    out.rdbuf()->pubsetbuf(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    out.open(path);
    if (!out) throw FileCreateError(path);

    TablePrinter::print(out, students, method);
    out.flush();
    if (!out) throw FileIOError("writing to", path);
}
