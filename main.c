#include <stdio.h>
#include <string.h>

#include "employees.h"
#include "suppliers.h"
#include "assets.h"
#include "budget.h"
#include "reports.h"

/* =========================
   MAIN MENU
   ========================= */

void displayMenu(void)
{
    printf("\n");
    printf("=============================================\n");
    printf("   MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");
    printf("=============================================\n");
    printf("1. Employee Management\n");
    printf("2. Budget Management\n");
    printf("3. Supplier Management\n");
    printf("4. Asset Management\n");
    printf("5. Reports\n");
    printf("6. Exit\n");
    printf("=============================================\n");
}

int main(void)
{
    int choice;
    char input[20];

    do
    {
        displayMenu();

        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("\nInvalid choice. Please enter a number from 1-6.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                employeeMenu();
                break;

            case 2:
                budgetMenu();
                break;

            case 3:
                supplierMenu();
                break;

            case 4:
                assetMenu();
                break;

            case 5:
                displayReports();
                break;

            case 6:
                printf("\nExiting Municipal Financial Management System...\n");
                break;

            default:
                printf("\nInvalid choice. Please select 1-6.\n");
        }

    } while (choice != 6);

    return 0;
}
