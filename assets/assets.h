#ifndef ASSETS_H
#define ASSETS_H
#define MAX_ASSETS 100
typedef struct {
    int id;
    char name[80];
    char type[60];
    double purchaseValue;
    char department[60];
    char condition[40];
} Asset;
void assetMenu(void);
void addAsset(void);
void displayAssets(void);
void searchAsset(void);
int assetCount(void);
const Asset *getAsset(int index);
#endif