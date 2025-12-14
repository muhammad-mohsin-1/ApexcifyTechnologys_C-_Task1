================================================================================
                  ACADEMIC MANAGEMENT SYSTEM (CGPA CALCULATOR)
================================================================================

Version:   5.0 (Professional Edition)
Language:  C++
Interface: Console (CLI)
Database:  CSV File (StudentData.csv)


[ PROJECT OVERVIEW ]
--------------------------------------------------------------------------------
This is a robust C++ Console Application designed to manage student academic
records. It allows users to register students, input course marks, automatically
calculate SGPA & CGPA, and store everything in a persistent Excel-compatible
CSV database.

Unlike simple calculators, this system features sound effects, input validation,
and a professional dashboard interface.


[ KEY FEATURES ]
--------------------------------------------------------------------------------
* Student Registration:
  Capture Roll No, Name, University, and Semester details.

* Automatic Grading:
  - Enter Marks (0-100) and the system automatically calculates Grade Points
    (4.0 Scale) and Letter Grades.
  - Computes SGPA (Semester GPA) and CGPA (Cumulative GPA) instantly.

* Persistent Database:
  All data is saved to "StudentData.csv". You can open this file directly in
  Microsoft Excel.

* Search & View:
  Search for students by Roll Number to view a detailed Official Transcript.

* Update Records:
  Functionality to edit a student's Name or University after registration.

* Robust Error Handling:
  Prevents crashes by validating all inputs (e.g., rejects text in numeric
  fields).

* Interactive UI:
  Includes sound effects for success, errors, and clicks.


[ HOW TO RUN ]
--------------------------------------------------------------------------------
1. Compiler:
   Use any standard C++ compiler (Dev-C++, Visual Studio, GCC).

2. Compile:
   Compile the "main.cpp" file.

3. Run:
   Execute the generated ".exe" file.

4. Database:
   The system will automatically create "StudentData.csv" in the same folder
   if it does not exist.


[ CONTROLS ]
--------------------------------------------------------------------------------
* Keyboard: Use Number keys (1, 2, 3) to navigate the menu.
* Input:    Follow the on-screen prompts.


[ FILE STRUCTURE ]
--------------------------------------------------------------------------------
* main.cpp:
  The core source code containing logic for GPA calculation and file handling.

* StudentData.csv:
  The database file storing student records in a structured format:
  Roll No, Name, University, Semester, SGPA, CGPA, Detailed Course Data...


[ REQUIREMENTS ]
--------------------------------------------------------------------------------
* Windows OS (required for Sound/Beep functions).
* Dev-C++ or TDM-GCC Compiler recommended.


================================================================================
Developed by [Muhammad Mohsin / Apexify Technologies]
================================================================================