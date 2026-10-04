# MFMS-PROJECT-A
# Municipal Financial Management System (MFMS) – Project A

**Course:** PAP521S – Programming in Practice  
**Project:** Project A – Foundation System  
**Language:** ANSI C (C99)

---

## Group Members

| # | Name | Student Number |
|---|------|----------------|
| 1 | Naric Abrahams | 225123355 |
| 2 | Valentino Joseph | 226072975 |
| 3 | Lowell Van Schalkwyk | 225118025 |
| 4 | Vaughan Lehmann | 225111578 |
| 5 | Meggin Charity Pieters | 225178028 |

---

## Project Description

The MFMS is a menu-driven console application written in C for a municipality. It lets a user manage employees, departmental budgets, suppliers and municipal assets, and produce summary reports from that information.

This is the foundation version of the system. It demonstrates the programming concepts from the first part of the course: input and output, variables, operators, decisions, loops, arrays, strings and functions. The code is split into separate modules so that Project B can extend and improve it without starting again.

---

## System Features

### Main Menu
A clear numbered menu with six options: Employee Management, Budget Management, Supplier Management, Asset Management, Reports and Exit. Invalid input (letters, blanks, numbers outside 1–6) is rejected with a message.

### Employee Management
- Add an employee (unique ID, name, department, position, basic salary, housing, transport and other allowances).
- Display all employees.
- Search by employee ID, or by name (partial match, not case-sensitive).
- Calculate salary information: gross salary, income tax, pension (5% of basic salary) and net salary.

### Budget Management
- Add a department budget with its allocated amount and expenditure.
- Display each department's allocated budget, expenditure, remaining budget and status (WITHIN BUDGET or EXCEEDED BUDGET).
- Calculate budget status and warn about departments that have exceeded their budget.

### Supplier Management
- Add a supplier (ID, name, email, telephone, town/location).
- Display all suppliers.
- Search by supplier ID, exact name or town.
- Compare two suppliers (shows both and whether they are in the same town).

### Asset Management
- Add an asset (ID, name, type, purchase value, department, condition).
- Display all assets.
- Search by asset ID, exact name or department.

### Reports
- **Employee report:** total employees, average, highest and lowest salary.
- **Budget report:** total allocated budget, total expenditure, remaining budget and the departments that exceeded their budget.
- **Supplier report:** total suppliers and the supplier list.
- **Asset report:** total assets, total purchase value and the asset list.
- **Full report:** all four reports together.

### Input Validation
- Negative salaries, budgets, expenditure and asset values are rejected.
- Empty names and fields are handled in the employee, supplier and asset modules.
- Invalid numbers (for example letters) and invalid menu choices are detected.
- Duplicate employee, supplier and asset IDs are rejected.

### Known Limitations
- Data is stored in arrays in memory, so it is **not saved** when the program closes.
- Storage is limited to 100 employees, 100 suppliers, 100 assets and 20 department budgets.
- Supplier and asset searches by name, town and department are exact and case-sensitive.
- The income tax rates are a simplified model for this project, not the official tax tables.

---

## Repository Structure

```
MFMS/
├── main.c              Main menu
├── employees/          employees.c, employees.h
├── budget/             budget.c, budget.h
├── suppliers/          suppliers.c, suppliers.h
├── assets/             assets.c, assets.h
├── reports/            reports.c, reports.h
├── tests/              Test plan and test files
├── docs/               Technical report and contribution records
└── README.md
```

---

## Compilation Instructions

**Requirements:** GCC (a C99-capable compiler). Visual Studio Code is the recommended editor.

1. Clone the repository and open a terminal in the main project folder (the folder that contains `main.c`).
2. Compile with this command:

```
gcc -std=c99 -Wall -Wextra -Iemployees -Ibudget -Isuppliers -Iassets -Ireports main.c employees/employees.c budget/budget.c suppliers/suppliers.c assets/assets.c reports/reports.c -lm -o mfms
```

The `-I` options tell the compiler where to find each module's header file. The command should finish with no errors or warnings.

> `tests/test_employees.c` has its own `main()` and must **not** be included in this command.

---

## How to Run the System

After compiling, start the program from the same folder:

- **Linux / macOS:** `./mfms`
- **Windows:** `mfms.exe`

Then type the number of a menu option and press Enter. Each module has its own sub-menu with a "Return to Main Menu" option. Choose **6** on the main menu to exit.

A quick way to see the system working:
1. Choose **1** and add an employee, then choose **4** to see the salary calculation.
2. Choose **2** and add a department budget.
3. Choose **3** and **4** to add a supplier and an asset.
4. Choose **5** to view the reports.

---

## Individual Responsibilities

| Name | Student Number | Responsibility |
|------|----------------|----------------|
| Naric Abrahams | 225123355 | Employee Management (`employees/`) |
| Valentino Joseph | 226072975 | Supplier Management and Asset Management (`suppliers/`, `assets/`) |
| Lowell Van Schalkwyk | 225118025 | Main Menu and Budget Management (`main.c`, `budget/`) |
| Vaughan Lehmann | 225111578 | Reports, functions and integration (`reports/`) |
| Meggin Charity Pieters | 225178028 | Testing, documentation and Git coordination (`tests/`, `docs/`, `README.md`) |

Each member's detailed contribution is recorded in the Individual Contribution Records in the `docs/` folder and in the commit history of this repository.
