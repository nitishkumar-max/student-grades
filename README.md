# Student Grade Calculator

A C++ console application that collects students' homework and exam results,
calculates each student's **final grade**, and can split large student lists
into "passed" and "failed" files while measuring how long every step takes.

## Final grade formula

```
Final = 0.4 * HW + 0.6 * Exam
```

`HW` is the average or the median of the homework results (all results are on
a 1 to 10 scale). A student with a final grade **below 5.0 is "failed"**,
otherwise "passed".

## Project structure (v0.2)

| File                         | What it contains                                                         |
|------------------------------|--------------------------------------------------------------------------|
| `Person.h/.cpp`              | `Person` class (rule of five, `>>` / `<<`, final grade, line parser)      |
| `Container.h`                | `StudentContainer` (here `std::vector<Person>`) and its helper functions  |
| `StudentProcessor.h/.cpp`    | `StudentProcessor` class: sorting and splitting into passed / failed      |
| `StudentFile.h/.cpp`         | `StudentFile` class: reading and writing student files                    |
| `FileGenerator.h/.cpp`       | `StudentFileGenerator` class: random test files                           |
| `Benchmark.h/.cpp`           | `Benchmark` class: timed pipeline read -> sort -> split -> write          |
| `TablePrinter.h/.cpp`        | `TablePrinter` class: formatted table output                              |
| `Application.h/.cpp`         | `Application` class: menu and user dialogue                               |
| `Exceptions.h`               | program-specific exception classes                                        |
| `InputUtils.h`, `StringUtils.h/.cpp`, `Timer.h`, `Version.h` | small helpers (safe input, natural string order, stopwatch) |
| `main.cpp`                   | creates `Application` and handles fatal errors                            |

## Exception handling

| Situation                                              | Exception                                   | Handled                                             |
|--------------------------------------------------------|---------------------------------------------|-----------------------------------------------------|
| input file does not exist / cannot be opened           | `FileOpenError`                             | the program asks for another file name              |
| output file cannot be created                          | `FileCreateError`                           | the program asks for another file name              |
| read/write fails half-way (e.g. disk full)             | `FileIOError`                               | message, back to the menu                           |
| file has no valid student records                      | `DataError`                                 | message, back to the menu                           |
| a line is malformed or a grade is outside 1..10        | `std::invalid_argument` (`Person`)          | the line is skipped and counted                     |
| average / median of an empty homework list             | `std::domain_error`                         | the line is skipped                                 |
| index outside a vector in the median calculation       | `std::out_of_range` (from `vector::at`)     | message, back to the menu                           |
| not enough memory for a huge data set                  | `std::bad_alloc`                            | message, back to the menu (that file is skipped in the speed test) |
| keyboard input ends (Ctrl+D / Ctrl+Z)                  | `InputEnded`                                | the program exits cleanly                           |

## Menu

```
1 - Type names and all results by hand
2 - Type names, generate results randomly
3 - Generate names and results randomly
4 - Read from a file (e.g. Students.txt), sort and show
5 - Generate the five test files (1 000 ... 10 000 000 students)
6 - Read a file, sort, split into passed/failed, save two files (timed)
7 - Speed test: option 6 on all five test files
0 - Exit
```

Options 1-4 work as in v0.1. Option 5 creates `students1000.txt`,
`students10000.txt`, `students100000.txt`, `students1000000.txt` and
`students10000000.txt` (`Name1 Surname1 ...`, random grades 1..10).
Option 6 reads one file, sorts it (by name, surname or final grade), splits it
and writes `<file>_passed.txt` and `<file>_failed.txt`. Option 7 repeats this
for all five test files and prints a Markdown table.

## Input file format

The first line is a header. Every other line has a name, a surname, any
number of homework results and the exam result as the last number:

```
Name        Surname            HW1   HW2   HW3   HW4   HW5   Exam
Jonas       Jonaitis             8     9    10     6    10      9
```

## Building

With g++ (Linux, macOS, or MinGW on Windows):

```
make
./student_grades
```

With CMake (also works with Visual Studio):

```
cmake -S . -B build
cmake --build build --config Release
```

Standard C++17 only. Speed measurements must be made with an optimised
(Release / `-O2`) build.

## Speed analysis (v0.2, `std::vector`)

Computer: Intel Core Ultra 9 185H, test run in a Linux virtual machine with
2 CPU cores and 4 GB RAM, SSD; g++ `-O2`.  
Data: 5 homework results + exam per student. Final grade: average, sorted by:
final grade, times in seconds.

| File                 | Students   | Passed    | Failed    | Read  | Sort   | Split | Write | Total  |
|----------------------|-----------:|----------:|----------:|------:|-------:|------:|------:|-------:|
| students1000.txt     | 1 000      | 579       | 421       | 0.001 | 0.000  | 0.000 | 0.001 | 0.001  |
| students10000.txt    | 10 000     | 5 821     | 4 179     | 0.003 | 0.004  | 0.000 | 0.002 | 0.009  |
| students100000.txt   | 100 000    | 58 660    | 41 340    | 0.023 | 0.069  | 0.008 | 0.018 | 0.119  |
| students1000000.txt  | 1 000 000  | 586 396   | 413 604   | 0.286 | 0.974  | 0.174 | 0.232 | 1.665  |
| students10000000.txt | 10 000 000 | 5 867 285 | 4 132 715 | 3.264 | 11.790 | 1.096 | 1.832 | 17.981 |

Generating the five files took 0.001, 0.002, 0.015, 0.141 and 1.504 s.
Sorting is the slowest step; every step grows roughly linearly with the
number of students (sorting slightly faster, as n log n).

The generated data files are not stored in the repository (they are large).

## Versions

- **v0.1** - `Person` class, `>>`/`<<`, average/median final grade, `std::vector`,
  random data, reading from a file, sorted and formatted output.
- **v0.2** - code split into several `.h`/`.cpp` files and classes, exception
  handling, generation of five test files, division into passed/failed files,
  timing of every step.
