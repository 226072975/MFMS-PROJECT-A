#ifndef BUDGET_H
#define BUDGET_H

#define MAX_DEPARTMENTS 20

typedef struct
{
    char department[50];
    double allocatedBudget;
    double expenditure;
} Budget;

void budgetMenu(void);
void addBudget(void);
void displayBudgets(void);
void calculateBudget(void);

/* Functions used by Reports */
int budgetCount(void);
Budget getBudget(int index);

#endif
