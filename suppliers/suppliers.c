
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "suppliers.h"

static Supplier suppliers[MAX_SUPPLIERS];
static int count = 0;

/* Read a complete line and reject empty text. */
static void readText(const char *prompt, char *out, size_t size) {
    for (;;) {
        int c;
        size_t len;

        printf("%s", prompt);

        if (!fgets(out, (int)size, stdin)) {
            puts("Input ended.");
            exit(0);
        }

        len = strlen(out);

        if (len && out[len - 1] == '\n')
            out[--len] = '\0';
        else
            while ((c = getchar()) != '\n' && c != EOF) {}

        if (len && strspn(out, " \t\r") != len)
            return;

        puts("This field cannot be empty.");
    }
}

/* Read a valid whole number. */
static int readInt(const char *prompt, int min) {
    char buf[100], extra;
    int value;

    for (;;) {
        readText(prompt, buf, sizeof buf);

        if (sscanf(buf, " %d %c", &value, &extra) == 1 &&
            value >= min)
            return value;

        printf("Enter a valid whole number (minimum %d).\n", min);
    }
}

/* Find a supplier using their ID. */
static int findSupplier(int id) {
    int i;

    for (i = 0; i < count; i++) {
        if (suppliers[i].id == id)
            return i;
    }

    return -1;
}

/* Display one supplier. */
static void printSupplier(const Supplier *s) {
    printf("ID: %d | Name: %s | Email: %s | Phone: %s | Town: %s\n",
           s->id, s->name, s->email, s->telephone, s->town);
}

/* Add a new supplier. */
void addSupplier(void) {
    Supplier s;

    if (count >= MAX_SUPPLIERS) {
        puts("Supplier register is full.");
        return;
    }

    s.id = readInt("Supplier ID: ", 1);

    if (findSupplier(s.id) >= 0) {
        puts("This supplier ID already exists.");
        return;
    }

    readText("Supplier name: ", s.name, sizeof s.name);
    readText("Email: ", s.email, sizeof s.email);
    readText("Telephone: ", s.telephone, sizeof s.telephone);
    readText("Town/Location: ", s.town, sizeof s.town);

    suppliers[count++] = s;

    puts("Supplier added successfully.");
}

/* Display all registered suppliers. */
void displaySuppliers(void) {
    int i;

    if (!count) {
        puts("No suppliers registered.");
        return;
    }

    for (i = 0; i < count; i++)
        printSupplier(&suppliers[i]);
}

/* Search suppliers by ID, name or town. */
void searchSupplier(void) {
    int i, choice;
    char query[SUPPLIER_NAME_LEN];

    puts("1. Search by ID");
    puts("2. Search by exact name");
    puts("3. Search by town");

    choice = readInt("Choice: ", 1);

    if (choice == 1) {
        i = findSupplier(readInt("Supplier ID: ", 1));

        if (i < 0)
            puts("Supplier not found.");
        else
            printSupplier(&suppliers[i]);

    } else if (choice == 2 || choice == 3) {
        int found = 0;

        readText(choice == 2 ? "Exact name: " : "Town: ",
                 query, sizeof query);

        for (i = 0; i < count; i++) {
            if (strcmp(choice == 2 ? suppliers[i].name :
                       suppliers[i].town, query) == 0) {
                printSupplier(&suppliers[i]);
                found = 1;
            }
        }

        if (!found)
            puts("No matching suppliers.");

    } else {
        puts("Invalid search option.");
    }
}

/* Compare two suppliers. */
void compareSuppliers(void) {
    int a = findSupplier(readInt("First supplier ID: ", 1));
    int b = findSupplier(readInt("Second supplier ID: ", 1));

    if (a < 0 || b < 0) {
        puts("One or both suppliers were not found.");
        return;
    }

    puts("First supplier:");
    printSupplier(&suppliers[a]);

    puts("Second supplier:");
    printSupplier(&suppliers[b]);

    puts(strcmp(suppliers[a].town, suppliers[b].town) == 0 ?
         "Both suppliers are in the same town." :
         "Suppliers are in different towns.");
}

/* Functions for the Reports module. */
int supplierCount(void) {
    return count;
}

const Supplier *getSupplier(int index) {
    return index >= 0 && index < count ?
           &suppliers[index] : NULL;
}

/* Supplier Management menu. */
void supplierMenu(void) {
    int choice;

    do {
        puts("\n=== SUPPLIER MANAGEMENT ===");
        puts("1. Add supplier");
        puts("2. Display suppliers");
        puts("3. Search suppliers");
        puts("4. Compare suppliers");
        puts("5. Return");

        choice = readInt("Choice: ", 1);

        switch (choice) {
            case 1:
                addSupplier();
                break;
            case 2:
                displaySuppliers();
                break;
            case 3:
                searchSupplier();
                break;
            case 4:
                compareSuppliers();
                break;
            case 5:
                break;
            default:
                puts("Invalid menu choice.");
        }

    } while (choice != 5);
}
