


#include <stdio.h>
#include <stdlib.h>
#include "reports.h"
#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "assets.h"

/* LOCAL INPUT HELPER*/


static int readInt(const char *prompt)
{
    char  buf[64];
    char *end;
    long  value;

    while (1) {
        printf("%s", prompt);

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            return 0;                       
        }

        value = strtol(buf, &end, 10);

        if (end == buf) {                   
            printf("  Error: please enter a whole number.\n");
            continue;
        }

        while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n') {
            end++;
        }
        if (*end != '\0') {                 
            printf("  Error: please enter a whole number.\n");
            continue;
        }

        return (int)value;
    }
}

/* BUDGET SUMMARY HELPERS (built on getBudget(i)) */


static double getTotalAllocated(void)
{
    int    i, n = budgetCount();
    double total = 0.0;

    for (i = 0; i < n; i++) {
        total += getBudget(i).allocatedBudget;
    }
    return total;
}


static double getTotalExpenditure(void)
{
    int    i, n = budgetCount();
    double total = 0.0;

    for (i = 0; i < n; i++) {
        total += getBudget(i).expenditure;
    }
    return total;
}


static double getTotalRemaining(void)
{
    return getTotalAllocated() - getTotalExpenditure();
}


static int getExceededBudgetCount(void)
{
    int i, n = budgetCount();
    int count = 0;

    for (i = 0; i < n; i++) {
        Budget b = getBudget(i);
        if (b.expenditure > b.allocatedBudget) {
            count++;
        }
    }
    return count;
}

/* Print every department that has overspent. */
static void displayExceededBudgets(void)
{
    int i, n = budgetCount();

    for (i = 0; i < n; i++) {
        Budget b = getBudget(i);
        if (b.expenditure > b.allocatedBudget) {
            printf("  - %s (over by N$%.2f)\n",
                   b.department,
                   b.expenditure - b.allocatedBudget);
        }
    }
}

/* ASSET SUMMARY HELPER (built on getAsset(i))*/

/* Sum of all purchase values in the asset register. */
static double getTotalAssetValue(void)
{
    int    i, n = assetCount();
    double total = 0.0;

    for (i = 0; i < n; i++) {
        total += getAsset(i)->purchaseValue;
    }
    return total;
}

/* EMPLOYEE REPORT*/
void employeeReport(void)
{
    int count = getEmployeeCount();

    printf("\n========================================\n");
    printf("           EMPLOYEE REPORT\n");
    printf("========================================\n");

    if (count == 0) {
        printf("No employees registered.\n");
        return;
    }

    printf("Total Employees : %d\n",     count);
    printf("Average Salary  : N$%.2f\n", getAverageSalary());
    printf("Highest Salary  : N$%.2f\n", getHighestSalary());
    printf("Lowest Salary   : N$%.2f\n", getLowestSalary());
    printf("----------------------------------------\n");

    displayEmployees();

    printf("========================================\n");
}

/* BUDGET REPORT*/
void budgetReport(void)
{
    int count = budgetCount();

    printf("\n========================================\n");
    printf("            BUDGET REPORT\n");
    printf("========================================\n");

    if (count == 0) {
        printf("No budgets registered.\n");
        return;
    }

    printf("Total Allocated Budget : N$%.2f\n", getTotalAllocated());
    printf("Total Expenditure      : N$%.2f\n", getTotalExpenditure());
    printf("Remaining Budget       : N$%.2f\n", getTotalRemaining());
    printf("----------------------------------------\n");

    printf("Departments exceeding budget:\n");
    if (getExceededBudgetCount() == 0) {
        printf("  None - all departments are within budget.\n");
    } else {
        displayExceededBudgets();
    }

    printf("----------------------------------------\n");

    displayBudgets();

    printf("========================================\n");
}

/* SUPPLIER REPORT*/
void supplierReport(void)
{
    int count = supplierCount();

    printf("\n========================================\n");
    printf("           SUPPLIER REPORT\n");
    printf("========================================\n");

    if (count == 0) {
        printf("No suppliers registered.\n");
        return;
    }

    printf("Total Suppliers : %d\n", count);
    printf("----------------------------------------\n");

    displaySuppliers();

    printf("========================================\n");
}

/* ASSET REPORT*/
void assetReport(void)
{
    int count = assetCount();

    printf("\n========================================\n");
    printf("             ASSET REPORT\n");
    printf("========================================\n");

    if (count == 0) {
        printf("No assets registered.\n");
        return;
    }

    printf("Total Assets         : %d\n",     count);
    printf("Total Purchase Value : N$%.2f\n", getTotalAssetValue());
    printf("----------------------------------------\n");

    displayAssets();

    printf("========================================\n");
}

/* FULL REPORT (all modules combined) */
void fullReport(void)
{
    printf("\n########################################\n");
    printf("#     MUNICIPAL FINANCIAL SUMMARY      #\n");
    printf("########################################\n");

    employeeReport();
    budgetReport();
    supplierReport();
    assetReport();

    printf("\n########################################\n");
    printf("#              END OF REPORT           #\n");
    printf("########################################\n");
}

/* REPORTS SUB-MENU */
void reportsMenu(void)
{
    int choice;

    do {
        printf("\n========================================\n");
        printf("               REPORTS\n");
        printf("========================================\n");
        printf("1. Employee Report\n");
        printf("2. Budget Report\n");
        printf("3. Supplier Report\n");
        printf("4. Asset Report\n");
        printf("5. Full Report (All Modules)\n");
        printf("6. Back to Main Menu\n");
        printf("========================================\n");

        choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1:
            employeeReport();
            break;
        case 2:
            budgetReport();
            break;
        case 3:
            supplierReport();
            break;
        case 4:
            assetReport();
            break;
        case 5:
            fullReport();
            break;
        case 6:
            printf("\nReturning to main menu...\n");
            break;
        default:
            printf("\nInvalid choice. Please enter a number from 1 to 6.\n");
        }

    } while (choice != 6);
}
