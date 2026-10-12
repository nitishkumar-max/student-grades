# Student Grade Calculator

A C++ console application that calculates students' final grades
(`Final = 0.4 * HW + 0.6 * Exam`, HW = average or median) and splits large
student lists into "passed" (final >= 5.0) and "failed" (final < 5.0) files,
measuring how long every step takes.

**v0.25** is the same program as v0.2, built with three different containers
to compare their speed: `std::vector`, `std::list` and `std::deque`.

## How the container is chosen

All the code uses only the type `StudentContainer`, defined in `Container.h`.
It is selected at compile time, so the three programs are built from exactly
the same sources:

| Program                 | Container      | Compiler flag                |
|-------------------------|----------------|------------------------------|
| `student_grades_vector` | `std::vector`  | (none)                       |
| `student_grades_list`   | `std::list`    | `-DSTUDENT_CONTAINER_LIST`   |
| `student_grades_deque`  | `std::deque`   | `-DSTUDENT_CONTAINER_DEQUE`  |

Differences that the change makes in `Container.h`:

- `std::list` has no random access, so it is sorted with its own member
  function `list::sort`; the other two use `std::sort`;
- only `std::vector` can `reserve` memory in advance.

Everything else (reading, splitting, writing, timing) is identical.

## Building

```
make                      # builds all three programs
make student_grades_list  # or just one of them
```

or with CMake:

```
cmake -S . -B build
cmake --build build --config Release
```

The program, its menu, the file format, the exception handling and the project
structure are described in the v0.2 README. Run the program, choose
**5** once to generate the five test files, then **7** to run the speed test.
Speed measurements must be made with an optimised (Release / `-O2`) build.

## Speed analysis

Computer: Intel Core Ultra 9 185H, test run in a Linux virtual machine with
2 CPU cores and 4 GB RAM, SSD; g++ `-O2`.  
Data: 5 homework results + exam per student. Final grade: average, sorted by:
final grade, times in seconds.

### std::vector

| Students   | Read  | Sort   | Split | Write  | Total  |
|-----------:|------:|-------:|------:|-------:|-------:|
| 1 000      | 0.003 | 0.000 | 0.000 | 0.001 | 0.004 |
| 10 000     | 0.005 | 0.004 | 0.000 | 0.002 | 0.011 |
| 100 000    | 0.026 | 0.069 | 0.009 | 0.019 | 0.122 |
| 1 000 000  | 0.249 | 0.843 | 0.103 | 0.226 | 1.420 |
| 10 000 000 | 3.106 | 11.563 | 1.001 | 2.295 | 17.966 |

### std::list

| Students   | Read  | Sort   | Split | Write  | Total  |
|-----------:|------:|-------:|------:|-------:|-------:|
| 1 000      | 0.001 | 0.000 | 0.000 | 0.001 | 0.001 |
| 10 000     | 0.002 | 0.002 | 0.001 | 0.002 | 0.008 |
| 100 000    | 0.022 | 0.056 | 0.036 | 0.025 | 0.140 |
| 1 000 000  | 0.281 | 1.513 | 0.688 | 0.254 | 2.735 |
| 10 000 000 | 2.682 | 25.902 | 8.390 | 10.508 | 47.483 |

### std::deque

| Students   | Read  | Sort   | Split | Write  | Total  |
|-----------:|------:|-------:|------:|-------:|-------:|
| 1 000      | 0.001 | 0.000 | 0.000 | 0.001 | 0.002 |
| 10 000     | 0.003 | 0.005 | 0.000 | 0.003 | 0.011 |
| 100 000    | 0.023 | 0.122 | 0.006 | 0.061 | 0.212 |
| 1 000 000  | 0.258 | 0.986 | 0.073 | 0.199 | 1.515 |
| 10 000 000 | 2.446 | 12.751 | 0.815 | 5.380 | 21.393 |

### Comparison for 10 000 000 students

| Step  | std::vector | std::list | std::deque |
|-------|------------:|----------:|-----------:|
| Read  | 3.106       | 2.682     | 2.446      |
| Sort  | 11.563      | 25.902    | 12.751     |
| Split | 1.001       | 8.390     | 0.815      |
| Write | 2.295       | 10.508    | 5.380      |
| Total | 17.966      | 47.483    | 21.393     |
| Peak memory | 2.8 GB | 3.1 GB  | 2.6 GB     |

### Conclusions

- **std::vector** is the fastest overall (18.0 s for 10 million students).
  Its elements lie in one continuous block of memory, which the processor
  cache handles best, and `std::sort` works directly on it.
- **std::deque** is almost as fast (21.4 s). It also has random-access
  iterators, so it uses `std::sort` too; its memory is split into blocks, so
  reading (no reallocation when it grows) and splitting were slightly faster,
  but sorting and writing a little slower than with a vector.
- **std::list** is clearly the slowest on large data (47.5 s, about 2.6 times
  slower than vector). Every student is a separate node somewhere in memory,
  so walking through the list causes many cache misses: sorting
  (`list::sort`) took twice as long, and splitting and writing were 5-8
  times slower. It also used the most memory (two extra pointers per student).
- With small files (up to 100 000 students) all three containers take well
  under a quarter of a second and the differences do not matter.
- Conclusion: for this task `std::vector` is the best choice.

## Versions

- **v0.1** - basic program (`Person`, `std::vector`).
- **v0.2** - several `.h`/`.cpp` files, exceptions, test file generation,
  passed/failed split, timing.
- **v0.25** - the v0.2 code built with `std::vector`, `std::list` and
  `std::deque`, with a speed comparison.
