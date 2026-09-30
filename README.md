# Student Grade Calculator

A small C++ console application that collects students' homework and exam
results and calculates each student's **final grade**.

The data can be typed in by hand, generated randomly, or read from a text
file. The results are shown as a neatly formatted table.

## Final grade formula

```
Final = 0.4 * HW + 0.6 * Exam
```

where `HW` is the homework score, calculated in one of two ways
(the user chooses which one):

- **Average:** `HW = (hw1 + hw2 + ... + hwn) / n`
- **Median:** the middle value of the sorted homework results
  (for an even `n`, the average of the two middle values)

All results (homework and exam) are on a 1 to 10 scale.

## Features (v0.1)

- `Person` class holding name, surname, homework results, exam result and
  final grade, with:
  - constructors, **copy constructor, copy assignment operator and
    destructor** (the rule of three);
  - overloaded `operator>>` (data input) and `operator<<` (data output);
  - `calculateFinal()` for the final grade by average, median or both.
- Homework results are stored in a `std::vector`, so their number does not
  need to be known in advance: when typing by hand, press Enter on an empty
  line to finish the list.
- Four ways to get the data:
  1. type names and all results by hand;
  2. type names and generate the results randomly;
  3. generate names and results randomly;
  4. read them from a file (for example `Students.txt`).
- Results are sorted by first name or surname ("natural" order, so
  `Name2` comes before `Name10`) and printed with two decimal places.
- For large data sets (more than 100 students) the table can be saved to a
  file instead of the screen.

## Input file format

The first line is a header. Every other line has a name, a surname, any
number of homework results and the exam result as the last number:

```
Name        Surname            HW1   HW2   HW3   HW4   HW5   Exam
Jonas       Jonaitis             8     9    10     6    10      9
Ana         Petraite             7    10     8     5     4      6
```

## Output example

```
Name      Surname         Final (Avg.) | Final (Med.)
-----------------------------------------------------
Ana       Petraite                6.32 |         6.40
Jonas     Jonaitis                8.84 |         9.00
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

The code uses standard C++17 only.

## Performance (release build, read + sort + save to file)

| File                | Students  | Read   | Sort   | Save   |
|---------------------|-----------|--------|--------|--------|
| students10000.txt   | 10 000    | 0.06 s | 0.01 s | 0.01 s |
| students100000.txt  | 100 000   | 0.67 s | 0.17 s | 0.09 s |
| students1000000.txt | 1 000 000 | 3.38 s | 2.14 s | 0.93 s |

The large test files are not stored in the repository (GitHub does not accept
files over 100 MB).

## Versions

- **v0.1** - `Person` class with the rule of three, `>>`/`<<` operators,
  average/median final grade, `std::vector` storage, random data generation,
  reading from a file, sorted and formatted output.
