#include <stdio.h>
#include <string.h>

#include "budget.h"

static Budget budgets[MAX_DEPARTMENTS];
static int budgetTotal = 0;

/* Budget menu */

void budgetMenu(void)
{
    int choice;
    char input[20];

    do
    {
        printf("\n");
        printf("=========== BUDGET MANAGEMENT ===========\n");
        printf("1. Add Department Budget\n");
        printf("2. Display Budgets\n");
        printf("3. Calculate Budget Status\n");
        printf("4. Return to Main Menu\n");
        printf("=========================================\n");

        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            return;
        }

        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid choice. Please enter a number.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                addBudget();
                break;

            case 2:
                displayBudgets();
                break;

            case 3:
                calculateBudget();
                break;

            case 4:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 4);
}

/* Add budget */

void addBudget(void)
{
    if (budgetTotal >= MAX_DEPARTMENTS)
    {
        printf("\nBudget storage is full.\n");
        return;
    }

    printf("\nEnter Department Name: ");

    if (fgets(budgets[budgetTotal].department,
              sizeof(budgets[budgetTotal].department), stdin) == NULL)
    {
        return;
    }

    budgets[budgetTotal].department[
        strcspn(budgets[budgetTotal].department, "\n")
    ] = '\0';

    do
    {
        char input[50];

        printf("Enter Allocated Budget: N$ ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            return;
        }

        if (sscanf(input, "%lf", &budgets[budgetTotal].allocatedBudget) != 1)
        {
            budgets[budgetTotal].allocatedBudget = -1;
        }

        if (budgets[budgetTotal].allocatedBudget < 0)
        {
            printf("Budget cannot be negative.\n");
        }

    } while (budgets[budgetTotal].allocatedBudget < 0);

    do
    {
        char input[50];

        printf("Enter Expenditure: N$ ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            return;
        }

        if (sscanf(input, "%lf", &budgets[budgetTotal].expenditure) != 1)
        {
            budgets[budgetTotal].expenditure = -1;
        }

        if (budgets[budgetTotal].expenditure < 0)
        {
            printf("Expenditure cannot be negative.\n");
        }

    } while (budgets[budgetTotal].expenditure < 0);

    budgetTotal++;

    printf("\nBudget added successfully.\n");
}

/* Display budgets */

void displayBudgets(void)
{
    int i;
    double remaining;

    if (budgetTotal == 0)
    {
        printf("\nNo budgets have been registered.\n");
        return;
    }

    printf("\n=============== BUDGET REPORT ===============\n");

    for (i = 0; i < budgetTotal; i++)
    {
        remaining =
            budgets[i].allocatedBudget -
            budgets[i].expenditure;

        printf("\nDepartment: %s\n",
               budgets[i].department);

        printf("Allocated Budget: N$ %.2f\n",
               budgets[i].allocatedBudget);

        printf("Expenditure: N$ %.2f\n",
               budgets[i].expenditure);

        printf("Remaining Budget: N$ %.2f\n",
               remaining);

        if (budgets[i].expenditure <=
            budgets[i].allocatedBudget)
        {
            printf("Status: WITHIN BUDGET\n");
        }
        else
        {
            printf("Status: EXCEEDED BUDGET\n");
        }
    }
}

/* Calculate budget */

void calculateBudget(void)
{
    int i;
    double remaining;

    if (budgetTotal == 0)
    {
        printf("\nNo budgets available.\n");
        return;
    }

    for (i = 0; i < budgetTotal; i++)
    {
        remaining =
            budgets[i].allocatedBudget -
            budgets[i].expenditure;

        printf("\nDepartment: %s\n",
               budgets[i].department);

        printf("Remaining Budget: N$ %.2f\n",
               remaining);

        if (budgets[i].expenditure >
            budgets[i].allocatedBudget)
        {
            printf("WARNING: Department has exceeded its budget.\n");
        }
        else
        {
            printf("Department is within budget.\n");
        }
    }
}

/* Functions used by Reports */

int budgetCount(void)
{
    return budgetTotal;
}

Budget getBudget(int index)
{
    Budget empty = {"", 0.0, 0.0};

    if (index < 0 || index >= budgetTotal)
    {
        return empty;
    }

    return budgets[index];
}
