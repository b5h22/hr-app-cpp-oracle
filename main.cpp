#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <string>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

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

// Function Prototypes
int menu(void);
int findEmployee(Connection *conn, int employeeNumber, Employee *emp);
void displayEmployee(Connection *conn, struct Employee emp);
void displayAllEmployees(Connection *conn);
void getEmployee(struct Employee *emp);
void insertEmployee(Connection* conn, struct Employee emp);
void updateEmployee(Connection* conn, int employeeNumber);
void deleteEmployee(Connection* conn, int employeeNumber);

int main(void) {
    /* OCCI Variables */
    Environment* env = nullptr;
    Connection* conn = nullptr;
    /* Used Variables */
    string user = "dbs211_241ndd13";
    string pass = "dbs211";

    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";
    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(user, pass, constr);

        bool exit = false;
        while (!exit) {
            switch (menu()) {
            case 1: {
                int employeeNumber;
                cout << "Enter Employee Number: ";
                cin >> employeeNumber;

                Employee emp;
                int result = findEmployee(conn, employeeNumber, &emp);
                if (result == 0) {
                    cout << "Employee " << employeeNumber << " does not exist." << endl << endl;
                }
                else {
                    displayEmployee(conn, emp);
                }
                break;
            }
            case 2:
                displayAllEmployees(conn);
                break;
            case 3: {
                Employee emp;
                getEmployee(&emp);
                insertEmployee(conn, emp);
                break;
            }
            case 4: {
                int employeeNumber;
                cout << "Enter Employee Number: ";
                cin >> employeeNumber;

                Employee emp;
                int result = findEmployee(conn, employeeNumber, &emp);
                if (result == 0) {
                    cout << "The employee with ID " << employeeNumber << " does not exist." << endl << endl;
                }
                else {
                    updateEmployee(conn, employeeNumber);
                }
                break;
            }
            case 5: {
                int employeeNumber;
                cout << "Enter Employee Number: ";
                cin >> employeeNumber;

                Employee emp;
                int result = findEmployee(conn, employeeNumber, &emp);
                if (result == 0) {
                    cout << "The employee with ID " << employeeNumber << " does not exist." << endl << endl;
                }
                else {
                    deleteEmployee(conn, employeeNumber);
                }
                break;
            }
            case 0:
                exit = true;
                break;
            }
        }

        env->terminateConnection(conn);
        Environment::terminateEnvironment(env);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    return 0;
}

// Menu function
int menu(void) {
    int selection;
    cout << "********************* HR Menu *********************" << endl;
    cout << "1) Find Employee" << endl;
    cout << "2) Employees Report" << endl;
    cout << "3) Add Employee" << endl;
    cout << "4) Update Employee" << endl;
    cout << "5) Remove Employee" << endl;
    cout << "0) Exit" << endl;
    do {
        cout << "Enter an option (0-5): ";
        cin >> selection;
        if (selection < 0 || selection > 5) {
            cout << "Incorrect option, please try again." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (selection < 0 || selection > 5);
    return selection;
};

// Find Employee function for menu option 1
int findEmployee(Connection *conn, int employeeNumber, Employee *emp) {
    try {
        string sqlQuery =
        "SELECT e.employeeNumber, e.lastName, e.firstName, e.extension, e.email, "
        "e.officeCode, e.reportsTo, e.jobTitle "
        "FROM employees e "
        "WHERE e.employeeNumber = :1";

        Statement *stmt = conn->createStatement(sqlQuery);
        stmt->setInt(1, employeeNumber);

        ResultSet *rs = stmt->executeQuery();
        if (rs->next()) {
            emp->employeeNumber = rs->getInt(1);
            strncpy(emp->lastName, rs->getString(2).c_str(), sizeof(emp->lastName));
            strncpy(emp->firstName, rs->getString(3).c_str(), sizeof(emp->firstName));
            strncpy(emp->extension, rs->getString(4).c_str(), sizeof(emp->extension));
            strncpy(emp->email, rs->getString(5).c_str(), sizeof(emp->email));
            strncpy(emp->officeCode, rs->getString(6).c_str(), sizeof(emp->officeCode));
            emp->reportsTo = rs->getInt(7);
            strncpy(emp->jobTitle, rs->getString(8).c_str(), sizeof(emp->jobTitle));
            
            stmt->closeResultSet(rs);
            conn->terminateStatement(stmt);
            return 1;
        } else {
            stmt->closeResultSet(rs);
            conn->terminateStatement(stmt);
            return 0; 
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
        return 0;
    }
};

// Display Employee function for menu option 1
void displayEmployee(Connection *conn, struct Employee emp) {
    cout << endl << "-------------- Employee Information -------------" << endl;
    cout << "Employee Number: " << emp.employeeNumber << endl;
    cout << "Last Name: " << emp.lastName << endl;
    cout << "First Name: " << emp.firstName << endl;
    cout << "Extension: " << emp.extension << endl;
    cout << "Email: " << emp.email << endl;
    cout << "Office Code: " << emp.officeCode << endl;
    cout << "Manager ID: " << emp.reportsTo << endl;
    cout << "Job Title: " << emp.jobTitle << endl << endl;
};

// Display All Employees function for menu option 2
void displayAllEmployees(Connection *conn) {
    try {
        string sqlQuery =
        "SELECT e.employeeNumber, e.firstName || ' ' || e.lastName, e.email, o.phone, e.extension, m.firstName || ' ' || m.lastName "
        "FROM employees e "
        "INNER JOIN offices o ON e.officeCode = o.officeCode "
        "LEFT JOIN employees m ON e.reportsTo = m.employeeNumber "
        "ORDER BY e.employeeNumber";

        Statement *stmt = conn->createStatement(sqlQuery);
        ResultSet *rs = stmt->executeQuery();

        if (!rs->next()) {
            cout << "There is no employees’ information to be displayed." << endl;
        }

        // Print the header
        cout << endl;
        cout << "------   ---------------    ---------------------------------  ----------------   ---------   -----------------" << endl;
        cout << "ID       Employee Name      Email                              Phone              Extension   Manager Name" << endl;
        cout << "-----    ---------------    ---------------------------------  ----------------   ---------   -----------------" << endl;
        do {
            cout << left << setw(8) << rs->getInt(1) << " "
                 << left << setw(18) << rs->getString(2) << " "
                 << left << setw(34) << rs->getString(3) << " "
                 << left << setw(18) << rs->getString(4) << " "
                 << left << setw(11) << rs->getString(5) << " "
                 << rs->getString(6) << endl;
        } while (rs->next());
        cout << endl;

        stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

// Get Employee function for menu option 3
void getEmployee(Employee* emp) {
    if (emp == nullptr)
        return;

    cout << endl << "-------------- New Employee Information -------------" << endl;
    cout << "Employee Number: ";
    cin >> emp->employeeNumber;
    cin.ignore(10000, '\n');

    cout << "Last Name: ";
    cin.getline(emp->lastName, 50);

    cout << "First Name: ";
    cin.getline(emp->firstName, 50);

    cout << "Extension: ";
    cin.getline(emp->extension, 10);

    cout << "Email: ";
    cin.getline(emp->email, 100);

    cout << "Office Code: ";
    cin.getline(emp->officeCode, 10);

    cout << "Manager ID: ";
    cin >> emp->reportsTo;
    cin.ignore(10000, '\n');

    cout << "Job Title: ";
    cin.getline(emp->jobTitle, 50);
    cout << endl;
}

// Insert Employee function for menu option 3
void insertEmployee(Connection* conn, Employee emp) {
    if (findEmployee(conn, emp.employeeNumber, &emp)) {
        cout << "An employee with the same employee number exists." << endl << endl;
    } else {
        string sqlQuery =
            "INSERT INTO employees VALUES ("
            + to_string(emp.employeeNumber) + ", '"
            + emp.lastName + "', '"
            + emp.firstName + "', '"
            + emp.extension + "', '"
            + emp.email + "', '"
            + emp.officeCode + "', "
            + to_string(emp.reportsTo) + ", '"
            + emp.jobTitle + "')";

        try {
            Statement* stmt = conn->createStatement(sqlQuery);
            ResultSet* rs = stmt->executeQuery();
            stmt->closeResultSet(rs);
            conn->commit();
            cout << "The new employee is added successfully." << endl << endl;
            conn->terminateStatement(stmt);
        }
        catch (SQLException& e) {
            cout << "Error inserting employee: " << e.getMessage() << endl;
        }
    }
}

// Update Employee function for menu option 4
void updateEmployee(Connection* conn, int employeeNumber) {
    try {
        Employee emp;
        findEmployee(conn, employeeNumber, &emp);
        cout << "Last Name: " << emp.lastName << endl;
        cout << "First Name: " << emp.firstName << endl;
        cout << "Extension: ";
        cin >> emp.extension;
        cout << endl;
        
        string sqlQuery =
			"UPDATE employees "
			"SET extension = :1 "
			"WHERE employeeNumber = :2";
        
        Statement* stmt = conn->createStatement(sqlQuery);
        stmt->setString(1, emp.extension);
        stmt->setInt(2, employeeNumber);
        ResultSet* rs = stmt->executeQuery();

        cout << "The employee's extension is updated successfully." << endl << endl;

        stmt->closeResultSet(rs);
        conn->commit();
        conn->terminateStatement(stmt);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
}

// Delete Employee function for menu option 5
void deleteEmployee(Connection* conn, int employeeNumber) {
    try {
        string sqlQuery = 
            "DELETE FROM employees "
            "WHERE employeeNumber = :1";

        Statement* stmt = conn->createStatement(sqlQuery);
        stmt->setInt(1, employeeNumber);
        ResultSet* rs = stmt->executeQuery();

        cout << endl << "The employee with ID " << employeeNumber << " is deleted successfully." << endl << endl;

        stmt->closeResultSet(rs);
        conn->commit();
        conn->terminateStatement(stmt);
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage() << endl;
    }
}