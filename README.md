# CS 300: Data Structures and Algorithms

## Portfolio Artifacts

This repository contains two artifacts from my work in CS 300.

The first artifact is my Project One analysis, which evaluates the run-time efficiency and memory usage of vectors, hash tables, and binary search trees. The second artifact is my working C++ program from Project Two. The program loads course information from a CSV file, sorts the courses in alphanumeric order, displays the complete course list, and allows the user to search for individual courses and their prerequisites.

## Reflection

### What was the problem you were solving in the projects for this course?

The main problem I was solving was creating an advising assistance program for the Computer Science department at ABCU. Academic advisors needed a program that could load course information, display all courses in alphanumeric order, and show the course title and prerequisites for a selected course. Before writing the program, I analyzed several data structures to determine which one would be appropriate for storing, sorting, and searching the course information.

### How did you approach the problem? Consider why data structures are important to understand.

I approached the problem by first studying vectors, hash tables, and binary search trees. I compared their run-time performance and memory requirements for operations such as inserting, searching, and sorting data. This helped me understand that the structure used to store information can have a major effect on the performance and complexity of a program.

For the completed C++ application, I broke the problem into smaller tasks. These tasks included reading the CSV file, creating course objects, storing the course data, validating prerequisites, sorting the courses, and displaying the requested information. Understanding data structures allowed me to make better decisions about how the course information should be organized and accessed.

### How did you overcome any roadblocks you encountered while going through the activities or project?

One of the main roadblocks I encountered was loading and reading the original CSV file correctly. The original version of the program depended on a separate CSVParser header file, which caused compilation and file-loading problems. I overcame this problem by creating the required file-reading logic directly inside one C++ file. I also made sure that the program removed unnecessary spaces from the CSV fields and checked whether the file opened successfully.

I tested the program multiple times using the original course data. I checked the menu options, the alphanumeric course list, individual course searches, prerequisite information, invalid course numbers, and invalid menu selections. Testing each section separately helped me locate problems and correct them without changing parts of the program that were already working.

### How has your work on this project expanded your approach to designing software and developing programs?

This project expanded my approach by showing me the importance of planning before beginning the coding process. The pseudocode and run-time analysis from Project One gave me a clear plan for Project Two. Instead of immediately writing code, I first considered the program requirements, the type of data being stored, the operations the program needed to perform, and the advantages and disadvantages of each possible data structure.

I also learned to divide larger programs into smaller functions with specific responsibilities. This made it easier to develop, test, and troubleshoot the application. In future projects, I will continue analyzing requirements and designing the program structure before beginning the full implementation.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

My work in this course improved the way I organize and document my programs. I used meaningful names for variables, functions, and data structures so that the purpose of each part of the program would be understandable. I separated responsibilities into functions for loading data, sorting courses, printing course information, validating input, and displaying the menu.

I also included comments where they were useful and avoided repeating the same code in multiple places. Input validation and error messages made the program more reliable and easier for someone else to use. These practices make the code easier to maintain because individual parts can be updated without rewriting the entire application. They also make the program adaptable if new courses, prerequisites, or features need to be added later.

## AI Use Acknowledgment

I used OpenAI ChatGPT to help organize and revise the wording of this reflection. I reviewed and edited the final response to ensure that it accurately represents my project, learning experience, and completed work.
Every wording of this assignment reflects my understanding of the project and no SNHU owned Property was directly uploaded into ChatGPT.
