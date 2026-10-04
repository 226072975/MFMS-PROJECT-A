#ifndef EMPLOYEES_H
#define EMPLOYEES_H


#define MAX_EMPLOYEES 100
#define ID_LEN        15
#define NAME_LEN      50
#define DEPT_LEN      30
#define POS_LEN       30


void employeeMenu(void);

void addEmployee(void);
void displayEmployees(void);
void searchEmployee(void);
void calculateSalary(void);


double calculateGross(double basic, double housing, double transport, double other);
double calculateTax(double gross);
double calculatePension(double basic);
double calculateNet(double gross, double tax, double pension);

int    getEmployeeCount(void);
double getAverageSalary(void);  
double getHighestSalary(void);  
double getLowestSalary(void);    
#endif
