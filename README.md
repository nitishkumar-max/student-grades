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
