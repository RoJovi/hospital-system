#ifndef MEDICINE_H
#define MEDICINE_H

#include "utils.h"

// º¯ÊýÉùÃ÷
void medicineManagement();
void addMedicine();
void showAllMedicines();
Medicine* findMedicineById(const char *id);
void showSingleMedicineDetail(const char *id);
void findMedicine();
void delMedicine();
void modMedicine();
void sortMedicinesByStock();
void swapMedicineData(Medicine *a, Medicine *b);

#endif // MEDICINE_H
