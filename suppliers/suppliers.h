#ifndef SUPPLIERS_H
#define SUPPLIERS_H
#define MAX_SUPPLIERS 100
#define SUPPLIER_NAME_LEN 80
#define SUPPLIER_CONTACT_LEN 100
typedef struct {
 int id;
 char name[SUPPLIER_NAME_LEN];
 char email[SUPPLIER_CONTACT_LEN];
 char telephone[30];
 char town[60];
} Supplier;
void supplierMenu(void);
void addSupplier(void);
void displaySuppliers(void);
void searchSupplier(void);
void compareSuppliers(void);
int supplierCount(void);
const Supplier *getSupplier(int index);
#endif
