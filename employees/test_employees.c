/*
 * test_employees.c
 * Stand-alone driver so you can test the employee module before
 * it is merged into the group's main.c. Do NOT commit this as main.c.
 *
 * Compile: gcc -std=c99 -Wall -Wextra test_employees.c employees.c -o test_employees
 * Run:     ./test_employees
 */

#include <stdio.h>
#include "employees.h"

int main(void)
{
    employeeMenu();

    printf("\nTotal employees : %d\n", getEmployeeCount());
    printf("Average salary  : N$%.2f\n", getAverageSalary());
    printf("Highest salary  : N$%.2f\n", getHighestSalary());
    printf("Lowest salary   : N$%.2f\n", getLowestSalary());
    return 0;
}
