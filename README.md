# HR Application using C++ and Oracle Database

## Project Description
This project is a simple HR application developed using the C++ programming language and Oracle database. It aims to provide a basic understanding of application development using C++ and Oracle database interactions. The application performs various operations such as finding, displaying, adding, updating, and removing employee records.

## Table of Contents
- [Features](#features)
- [Code Structure](#code-structure)
- [Database Schema](#database-schema)

## Features
1. Find Employee: Search for an employee by their employee number and display their details.
2. Employees Report: Display all employees' information in a formatted report.
3. Add Employee: Add a new employee to the database.
4. Update Employee: Update an existing employee's phone extension.
5. Remove Employee: Delete an employee from the database.

The application provides a simple menu interface to interact with the user. 
The user can select an option from the menu to perform the desired operation.
```cpp
********************* HR Menu *********************
1) Find Employee
2) Employees Report
3) Add Employee
4) Update Employee
5) Remove Employee
0) Exit
***************************************************
Enter an option (0-5):
```

## Code Structure
The application consists of several functions to handle different operations:
- menu(): Displays the menu and returns the user's selection.
- findEmployee(Connection *conn, int employeeNumber, Employee *emp): Searches for an employee by employee number.
- displayEmployee(Connection *conn, Employee emp): Displays the details of a specific employee.
- displayAllEmployees(Connection *conn): Displays a report of all employees.
- getEmployee(Employee *emp): Prompts the user to enter details for a new employee.
- insertEmployee(Connection *conn, Employee emp): Inserts a new employee into the database.
- updateEmployee(Connection *conn, int employeeNumber): Updates the phone extension of an existing employee.
- deleteEmployee(Connection *conn, int employeeNumber): Deletes an employee from the database.

The main function initializes the Oracle environment, connects to the database, and calls the menu function to interact with the user. Based on the user's selection, it calls the corresponding function to perform the operation.

## Database Schema
The application interacts with an Oracle database that contains a single table named "employees". The table has the following columns:
```cpp
struct Employee {
    int employeeNumber;
    char lastName[50];
    char firstName[50];
    char extension[10];
    char email[100];
    char officeCode[10];
    int reportsTo;
    char jobTitle[50];
};
```
The application uses Oracle's Pro*C/C++ precompiler to embed SQL statements within the C++ code. The SQL statements are used to perform database operations such as querying, inserting, updating, and deleting records.
