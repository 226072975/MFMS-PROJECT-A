/*
 * employees.c
 * Employee Management module - Municipal Financial Management System (MFMS)
 *
 * Data is stored in parallel arrays: index i in every array refers to the
 * same employee. They are "static" so only this file can touch them; other
 * modules (e.g. Reports) use the get...() functions at the bottom.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "employees.h"

/* ---------------------------------------------------------------
 * STORAGE
 * --------------------------------------------------------------- */
static char   empId[MAX_EMPLOYEES][ID_LEN];
static char   empName[MAX_EMPLOYEES][NAME_LEN];
static char   empDept[MAX_EMPLOYEES][DEPT_LEN];
static char   empPosition[MAX_EMPLOYEES][POS_LEN];
static double empBasic[MAX_EMPLOYEES];
static double empHousing[MAX_EMPLOYEES];
static double empTransport[MAX_EMPLOYEES];
static double empOther[MAX_EMPLOYEES];
static int    empCount = 0;

/* ---------------------------------------------------------------
 * INPUT HELPERS (validation lives here)
 * --------------------------------------------------------------- */

/* Read one line safely, strip the newline, discard any overflow. */
static void readLine(const char *prompt, char *buf, int size)
{
    printf("%s", prompt);

    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        int c;                                   /* line was too long: flush the rest */
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}

/* Returns 1 if the string is empty or only spaces/tabs. */
static int isBlank(const char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (!isspace((unsigned char)s[i])) {
            return 0;
        }
    }
    return 1;
}

/* Keep asking until the user types something that is not blank. */
static void readNonEmpty(const char *prompt, char *buf, int size)
{
    do {
        readLine(prompt, buf, size);
        if (isBlank(buf)) {
            printf("  Error: this field cannot be empty.\n");
        }
    } while (isBlank(buf));
}

/* Keep asking until the user types a valid number >= min. */
static double readDouble(const char *prompt, double min)
{
    char   temp[64];
    char  *end;
    double value;

    while (1) {
        readLine(prompt, temp, sizeof(temp));
        value = strtod(temp, &end);

        if (end == temp) {                        /* nothing was converted */
            printf("  Error: please enter a valid number.\n");
            continue;
        }
        while (isspace((unsigned char)*end)) {    /* allow trailing spaces */
            end++;
        }
        if (*end != '\0') {                       /* leftover characters, e.g. "12abc" */
            printf("  Error: please enter a valid number.\n");
            continue;
        }
        if (value < min) {
            printf("  Error: value cannot be less than %.2f.\n", min);
            continue;
        }
        return value;
    }
}

/* Keep asking until the user types a whole number. */
static int readInt(const char *prompt)
{
    char  temp[32];
    char *end;
    long  value;

    while (1) {
        readLine(prompt, temp, sizeof(temp));
        value = strtol(temp, &end, 10);

        if (end == temp) {
            printf("  Error: please enter a whole number.\n");
            continue;
        }
        while (isspace((unsigned char)*end)) {
            end++;
        }
        if (*end != '\0') {
            printf("  Error: please enter a whole number.\n");
            continue;
        }
        return (int)value;
    }
}

/* Copy src into dest in lower case (used for case-insensitive search). */
static void toLowerCopy(char *dest, const char *src)
{
    int i;
    for (i = 0; src[i] != '\0'; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

/* ---------------------------------------------------------------
 * INTERNAL HELPERS
 * --------------------------------------------------------------- */

/* Returns the array index of the employee with this ID, or -1 if not found. */
static int findEmployeeById(const char *id)
{
    int i;
    for (i = 0; i < empCount; i++) {
        if (strcmp(empId[i], id) == 0) {
            return i;
        }
    }
    return -1;
}

static void printEmployeeHeader(void)
{
    printf("\n%-8s %-22s %-16s %-18s %12s %12s\n",
           "ID", "Name", "Department", "Position", "Basic (N$)", "Gross (N$)");
    printf("---------------------------------------------------------------------------------------\n");
}

static void printEmployeeRow(int i)
{
    double gross = calculateGross(empBasic[i], empHousing[i], empTransport[i], empOther[i]);

    printf("%-8s %-22.22s %-16.16s %-18.18s %12.2f %12.2f\n",
           empId[i], empName[i], empDept[i], empPosition[i], empBasic[i], gross);
}

static void printPayslip(int i)
{
    double gross   = calculateGross(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
    double tax     = calculateTax(gross);
    double pension = calculatePension(empBasic[i]);
    double net     = calculateNet(gross, tax, pension);

    printf("\n========================================\n");
    printf("            SALARY INFORMATION\n");
    printf("========================================\n");
    printf("Employee ID       : %s\n", empId[i]);
    printf("Name              : %s\n", empName[i]);
    printf("Department        : %s\n", empDept[i]);
    printf("Position          : %s\n", empPosition[i]);
    printf("----------------------------------------\n");
    printf("Basic Salary      : N$%10.2f\n", empBasic[i]);
    printf("Housing Allowance : N$%10.2f\n", empHousing[i]);
    printf("Transport Allow.  : N$%10.2f\n", empTransport[i]);
    printf("Other Allowances  : N$%10.2f\n", empOther[i]);
    printf("Gross Salary      : N$%10.2f\n", gross);
    printf("----------------------------------------\n");
    printf("Income Tax        : N$%10.2f\n", tax);
    printf("Pension (5%% basic): N$%10.2f\n", pension);
    printf("NET SALARY        : N$%10.2f\n", net);
    printf("========================================\n");
}

/* ---------------------------------------------------------------
 * SALARY CALCULATIONS
 * NOTE: the tax brackets below are a SIMPLIFIED model for this
 * project, not the official PAYE tables. Change them if your
 * group decides on different rules.
 * --------------------------------------------------------------- */
double calculateGross(double basic, double housing, double transport, double other)
{
    return basic + housing + transport + other;
}

double calculateTax(double gross)
{
    double rate;

    if (gross <= 5000.0) {
        rate = 0.0;
    } else if (gross <= 15000.0) {
        rate = 0.10;
    } else if (gross <= 30000.0) {
        rate = 0.20;
    } else {
        rate = 0.30;
    }
    return gross * rate;
}

double calculatePension(double basic)
{
    return basic * 0.05;
}

double calculateNet(double gross, double tax, double pension)
{
    return gross - tax - pension;
}

/* ---------------------------------------------------------------
 * CORE FEATURES
 * --------------------------------------------------------------- */

void addEmployee(void)
{
    char id[ID_LEN];

    if (empCount >= MAX_EMPLOYEES) {
        printf("\nError: employee list is full (%d employees).\n", MAX_EMPLOYEES);
        return;
    }

    printf("\n--- ADD EMPLOYEE ---\n");

    /* Employee ID must be non-empty and unique */
    while (1) {
        readNonEmpty("Employee ID (e.g. E001): ", id, sizeof(id));
        if (findEmployeeById(id) != -1) {
            printf("  Error: an employee with ID '%s' already exists.\n", id);
        } else {
            break;
        }
    }
    strcpy(empId[empCount], id);

    readNonEmpty("Full name: ",  empName[empCount],     NAME_LEN);
    readNonEmpty("Department: ", empDept[empCount],     DEPT_LEN);
    readNonEmpty("Position: ",   empPosition[empCount], POS_LEN);

    empBasic[empCount]     = readDouble("Basic salary (N$): ",        0.0);
    empHousing[empCount]   = readDouble("Housing allowance (N$): ",   0.0);
    empTransport[empCount] = readDouble("Transport allowance (N$): ", 0.0);
    empOther[empCount]     = readDouble("Other allowances (N$): ",    0.0);

    empCount++;
    printf("\nEmployee added successfully. Total employees: %d\n", empCount);
}

void displayEmployees(void)
{
    int i;

    if (empCount == 0) {
        printf("\nNo employees have been added yet.\n");
        return;
    }

    printf("\n--- EMPLOYEE LIST (%d) ---", empCount);
    printEmployeeHeader();
    for (i = 0; i < empCount; i++) {
        printEmployeeRow(i);
    }
}

void searchEmployee(void)
{
    int  choice, i, found = 0;
    char query[NAME_LEN];
    char lowerQuery[NAME_LEN];
    char lowerName[NAME_LEN];

    if (empCount == 0) {
        printf("\nNo employees to search. Add an employee first.\n");
        return;
    }

    printf("\n--- SEARCH EMPLOYEE ---\n");
    printf("1. Search by Employee ID\n");
    printf("2. Search by Name (partial match allowed)\n");
    choice = readInt("Enter choice: ");

    switch (choice) {
    case 1:
        readNonEmpty("Enter Employee ID: ", query, sizeof(query));
        i = findEmployeeById(query);
        if (i == -1) {
            printf("\nNo employee found with ID '%s'.\n", query);
        } else {
            printEmployeeHeader();
            printEmployeeRow(i);
        }
        break;

    case 2:
        readNonEmpty("Enter name (or part of it): ", query, sizeof(query));
        toLowerCopy(lowerQuery, query);

        for (i = 0; i < empCount; i++) {
            toLowerCopy(lowerName, empName[i]);
            if (strstr(lowerName, lowerQuery) != NULL) {
                if (!found) {
                    printEmployeeHeader();
                }
                printEmployeeRow(i);
                found++;
            }
        }
        if (!found) {
            printf("\nNo employee found matching '%s'.\n", query);
        } else {
            printf("\n%d employee(s) found.\n", found);
        }
        break;

    default:
        printf("\nInvalid choice.\n");
    }
}

void calculateSalary(void)
{
    char id[ID_LEN];
    int  i;

    if (empCount == 0) {
        printf("\nNo employees available. Add an employee first.\n");
        return;
    }

    printf("\n--- CALCULATE SALARY ---\n");
    readNonEmpty("Enter Employee ID: ", id, sizeof(id));

    i = findEmployeeById(id);
    if (i == -1) {
        printf("\nNo employee found with ID '%s'.\n", id);
        return;
    }
    printPayslip(i);
}

/* ---------------------------------------------------------------
 * EMPLOYEE SUB-MENU
 * --------------------------------------------------------------- */
void employeeMenu(void)
{
    int choice;

    do {
        printf("\n========================================\n");
        printf("          EMPLOYEE MANAGEMENT\n");
        printf("========================================\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee\n");
        printf("4. Calculate Salary Information\n");
        printf("5. Back to Main Menu\n");
        choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1: addEmployee();      break;
        case 2: displayEmployees(); break;
        case 3: searchEmployee();   break;
        case 4: calculateSalary();  break;
        case 5: printf("\nReturning to main menu...\n"); break;
        default:
            printf("\nInvalid choice. Please enter a number from 1 to 5.\n");
        }
    } while (choice != 5);
}

/* ---------------------------------------------------------------
 * FUNCTIONS FOR THE REPORTS MODULE
 * --------------------------------------------------------------- */
int getEmployeeCount(void)
{
    return empCount;
}

double getAverageSalary(void)
{
    int    i;
    double total = 0.0;

    if (empCount == 0) {
        return 0.0;
    }
    for (i = 0; i < empCount; i++) {
        total += calculateGross(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
    }
    return total / empCount;
}

double getHighestSalary(void)
{
    int    i;
    double gross, highest;

    if (empCount == 0) {
        return 0.0;
    }
    highest = calculateGross(empBasic[0], empHousing[0], empTransport[0], empOther[0]);
    for (i = 1; i < empCount; i++) {
        gross = calculateGross(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
        if (gross > highest) {
            highest = gross;
        }
    }
    return highest;
}

double getLowestSalary(void)
{
    int    i;
    double gross, lowest;

    if (empCount == 0) {
        return 0.0;
    }
    lowest = calculateGross(empBasic[0], empHousing[0], empTransport[0], empOther[0]);
    for (i = 1; i < empCount; i++) {
        gross = calculateGross(empBasic[i], empHousing[i], empTransport[i], empOther[i]);
        if (gross < lowest) {
            lowest = gross;
        }
    }
    return lowest;
}
