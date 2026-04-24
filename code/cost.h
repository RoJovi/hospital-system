#ifndef COST_H
#define COST_H

#include "utils.h"

// º¯ÊýÉùÃ÷
void costManagement();
Cost* findCostById(const char *cost_id);
Cost* findCostByPatientId(const char *patient_id);
void addCost();
void autoAddCost(const char *patient_id, const char *item_name, double amount);
void showSingleCostDetail(const char *cost_id);
void showAllCosts();
void modifyCost();
void deleteCost();
void findCostByPatient(const char *id);
void updatePatientTotalCost(const char *patient_id, double amount_change);

#endif // COST_H
