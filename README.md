# Quick Sort Record Organizer in C

A menu-driven C program that manages student records and sorts them using the **Quick Sort algorithm**. Users can sort records by student ID, name, or marks in ascending or descending order, choose a pivot strategy, and analyze sorting performance using counters.

## Features

- **Add Student Records** — Store student ID, name, and marks.
- **Display Records** — View all stored student records in a tabular format.
- **Multiple Sorting Fields** — Sort by:
  - Student ID
  - Student Name
  - Student Marks
- **Sorting Order** — Choose ascending or descending order.
- **Pivot Selection** — Select one of three pivot strategies:
  - First Element
  - Middle Element
  - Last Element
- **Performance Analysis** — Track the number of comparisons, swaps, recursive calls, and maximum recursion depth.
- **Menu-Driven Interface** — Perform operations through an interactive console menu.
- **Fixed Record Capacity** — Supports up to 100 student records.

## Technologies Used

- **Programming Language:** C
- **Standard Libraries:** `stdio.h`, `string.h`
- **Algorithm:** Quick Sort
- **Data Structure:** Array of Structures

## Student Record Structure

Each student record contains three fields:

| Field | Data Type | Description |
|---|---|---|
| `id` | `int` | Unique or identifying student number |
| `name` | `char[30]` | Student name |
| `marks` | `float` | Student marks |

The records are stored in an array of structures with a maximum capacity of 100.

## Menu Options

| Option | Operation | Description |
|---|---|---|
| 1 | Add Records | Enter student details |
| 2 | Display Records | Display all stored records |
| 3 | Select Sorting Field | Choose ID, name, or marks |
| 4 | Select Sorting Order | Choose ascending or descending |
| 5 | Select Pivot | Choose first, middle, or last element |
| 6 | Sort Records | Sort records and display performance statistics |
| 7 | Exit | Terminate the program |

## Algorithm: Quick Sort

Quick Sort is a divide-and-conquer sorting algorithm that partitions an array around a pivot element.

### Working Principle

1. Select a pivot according to the user's choice.
2. Move the pivot to the end of the current partition.
3. Compare the remaining records with the pivot.
4. Place records that belong before the pivot on its left side.
5. Place the pivot in its final sorted position.
6. Recursively sort the left and right partitions.
7. Repeat until the partitions contain zero or one record.

The program uses a Lomuto-style partitioning approach and supports sorting in both ascending and descending order through a configurable comparison function.

## Performance Analysis

The program tracks four performance metrics:

- **Comparisons:** Number of times two student records are compared during partitioning.
- **Swaps:** Number of actual swap operations performed.
- **Recursive Calls:** Number of non-base-case calls to the Quick Sort function.
- **Maximum Depth:** Maximum depth value recorded for non-base-case calls.

### Time Complexity

| Case | Time Complexity |
|---|---|
| Best Case | O(n log n) |
| Average Case | O(n log n) |
| Worst Case | O(n²) |

### Space Complexity

- **Average auxiliary space:** O(log n) due to recursion.
- **Worst-case auxiliary space:** O(n) due to an unbalanced recursion tree.

Pivot selection can influence performance significantly. Choosing the first or last element as the pivot can lead to poor performance on certain input arrangements. Selecting the middle position may help for some inputs, but it does not guarantee balanced partitions.

## Requirements

- A C compiler, such as GCC.
- A terminal or command prompt.
- Windows, Linux, or macOS.

## How to Compile and Run

### 1. Save the Source Code

Save the program in a file named `quicksort.c`.

### 2. Compile the Program

Using GCC:

```bash
gcc quicksort.c -o quicksort
```

### 3. Run the Program

**Windows:**

```bash
.\quicksort.exe
```

**Linux or macOS:**

```bash
./quicksort
```

## Sample Output

```text
===== QUICK SORT RECORD ORGANIZER =====
1. Add Records
2. Display Records
3. Select Sorting Field
4. Select Sorting Order
5. Select Pivot
6. Sort Records
7. Exit

Enter choice: 1

How many students? 3

Student 1
ID: 103
Name: Rahul
Marks: 78.5

Student 2
ID: 101
Name: Priya
Marks: 92

Student 3
ID: 102
Name: Amit
Marks: 85

Enter choice: 3

1. ID
2. Name
3. Marks
Choose field: 1

Enter choice: 4

1. Ascending
2. Descending
Choose order: 1

Enter choice: 5

1. First Element
2. Middle Element
3. Last Element
Choose pivot: 2

Enter choice: 6

Records sorted successfully.

ID       NAME                 MARKS
101      Priya                92.00
102      Amit                 85.00
103      Rahul                78.50

--- Quick Sort Analysis ---
Comparisons       : [runtime-dependent]
Swaps             : [runtime-dependent]
Recursive Calls   : [runtime-dependent]
Maximum Depth     : [runtime-dependent]
```

*Note: The performance statistics depend on the input records and selected pivot strategy. The values shown as placeholders are calculated by the program at runtime.*

## Limitations

- The program supports a maximum of 100 records.
- Student records are stored in memory and are not saved to a file.
- Names are limited to 29 characters plus the terminating null character.
- The program assumes valid numeric input from the user.
- The program does not enforce uniqueness of student IDs.

## Learning Outcomes

This project demonstrates:

- Implementation of Quick Sort in C.
- Use of structures and arrays.
- Functions, pointers, and structure swapping.
- String comparison using `strcmp()`.
- Ascending and descending sorting.
- Pivot selection and partitioning.
- Basic algorithm performance analysis.

## Future Enhancements

- Add file handling to save and load student records.
- Validate user input and handle invalid data safely.
- Support searching, editing, and deleting records.
- Add randomized pivot selection.
- Compare performance across pivot strategies and input sizes.

## Author

Developed as a C programming project to demonstrate sorting algorithms, record management, and performance analysis.
