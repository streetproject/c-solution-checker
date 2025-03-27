# C Exam System
A C project that implements an exam system with multiple exercises. The user can progress through exercises, submit solutions, and view progress.

## Features
- Start, resume, or reset exercises.
- View progress and reset progress.
- Auto-checking of solutions.
- Configurable settings.

## Installation
Installation Using Make
- Open a terminal and navigate to the solutionchecker directory.
Run the following command to compile the project:
    make
This will compile your project based on the Makefile.

Manual Compilation
If you prefer to compile manually, use the following command:

    cc main.c functions.c printheader.c -o solutionchecker
    
-o solutionchecker specifies the output executable name.

### How to Use
- If you compiled using make, the program will run automatically.
- If you compiled manually using cc, run the program with:
    ./solutionchecker

### How It Works
1. You receive a task starting from 'ex00'.
2. The instructions for each task are provided in the 'subject.txt' file.
3. You must write your solution in the corresponding 'exXX.c' file (e.g., 'ex00.c', 'ex01.c', etc.).
4. The program will check your output against the expected output.
5. If your output is correct, you proceed to the next exercise.
6. Continue writing your solutions in 'ex01.c', 'ex02.c', and so on.

### Add or change exercises
To add or modify exercises, it’s important to determine whether the task requires a function only or a full program (including main() function).

1. Program Task (with main() function)
If the task requires creating a full program, your solution should be placed in:

solutions/solution_exXX.c

You do not need to include a main.c file in the directory.
The structure for a program task should look like this:
ex00/
└── solutions/

    └── solution_ex00.c

2. Function Task (function only, with main() provided)
If the task is only to create a function, write your solution in:

solutions/solution_exXX.c

Additionally, you must include a main.c file that tests the function implemented by the user and verifies its output.

The structure for a function-only task should look like this:

ex00/
└── solutions/
    ├── main.c
    └── solution_ex00.c

