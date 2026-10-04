
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "assets.h"

static Asset assets[MAX_ASSETS];
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

/* Read a positive whole number. */
static int readInt(const char *prompt) {
    char buf[100], extra;
    int value;

    for (;;) {
        readText(prompt, buf, sizeof buf);

        if (sscanf(buf, " %d %c", &value, &extra) == 1 &&
            value > 0)
            return value;

        puts("Enter a positive whole number.");
    }
}

/* Read a valid purchase value. */
static double readValue(void) {
    char buf[100], extra;
    double value;

    for (;;) {
        readText("Purchase value (N$): ", buf, sizeof buf);

        if (sscanf(buf, " %lf %c", &value, &extra) == 1 &&
            isfinite(value) && value >= 0)
            return value;

        puts("Enter a valid non-negative purchase value.");
    }
}

/* Find an asset by ID. */
static int findAsset(int id) {
    int i;

    for (i = 0; i < count; i++) {
        if (assets[i].id == id)
            return i;
    }

    return -1;
}

/* Display one asset. */
static void printAsset(const Asset *a) {
    printf("ID: %d | Name: %s | Type: %s | Value: N$%.2f | Department: %s | Condition: %s\n",
           a->id, a->name, a->type, a->purchaseValue,
           a->department, a->condition);
}

/* Add a new asset. */
void addAsset(void) {
    Asset a;

    if (count >= MAX_ASSETS) {
        puts("Asset register is full.");
        return;
    }

    a.id = readInt("Asset ID: ");

    if (findAsset(a.id) >= 0) {
        puts("This asset ID already exists.");
        return;
    }

    readText("Asset name: ", a.name, sizeof a.name);
    readText("Asset type: ", a.type, sizeof a.type);

    a.purchaseValue = readValue();

    readText("Department: ", a.department,
             sizeof a.department);
    readText("Condition: ", a.condition,
             sizeof a.condition);

    assets[count++] = a;

    puts("Asset added successfully.");
}

/* Display all assets. */
void displayAssets(void) {
    int i;

    if (!count) {
        puts("No assets registered.");
        return;
    }

    for (i = 0; i < count; i++)
        printAsset(&assets[i]);
}

/* Search assets by ID, name or department. */
void searchAsset(void) {
    int choice, i, found = 0;
    char query[80];

    puts("1. Search by ID");
    puts("2. Search by exact name");
    puts("3. Search by department");

    choice = readInt("Choice: ");

    if (choice == 1) {
        i = findAsset(readInt("Asset ID: "));

        if (i < 0)
            puts("Asset not found.");
        else
            printAsset(&assets[i]);

    } else if (choice == 2 || choice == 3) {
        readText(choice == 2 ? "Exact name: " :
                 "Department: ", query, sizeof query);

        for (i = 0; i < count; i++) {
            if (strcmp(choice == 2 ? assets[i].name :
                       assets[i].department, query) == 0) {
                printAsset(&assets[i]);
                found = 1;
            }
        }

        if (!found)
            puts("No matching assets.");

    } else {
        puts("Invalid search option.");
    }
}

/* Functions for the Reports module. */
int assetCount(void) {
    return count;
}

const Asset *getAsset(int index) {
    return index >= 0 && index < count ?
           &assets[index] : NULL;
}

/* Asset Management menu. */
void assetMenu(void) {
    int choice;

    do {
        puts("\n=== ASSET MANAGEMENT ===");
        puts("1. Add asset");
        puts("2. Display assets");
        puts("3. Search assets");
        puts("4. Return");

        choice = readInt("Choice: ");

        switch (choice) {
            case 1:
                addAsset();
                break;
            case 2:
                displayAssets();
                break;
            case 3:
                searchAsset();
                break;
            case 4:
                break;
            default:
                puts("Invalid menu choice.");
        }

    } while (choice != 4);
}
