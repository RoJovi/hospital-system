#ifndef DATA_H
#define DATA_H

#include "utils.h"

// 前向声明
struct Patient;
struct Doctor;
struct Medicine;
struct Registration;
struct Cost;
struct Hospitalization;

// 数据管理函数
void saveAllData();
void savePatients(struct Patient *head);
struct Patient* loadPatients();
void saveDoctors(struct Doctor *head);
struct Doctor* loadDoctors();
void saveMedicines(struct Medicine *head);
struct Medicine* loadMedicines();
void saveRegistrations();
void loadRegistrations();
void saveCosts();
void loadCosts();
void saveHospitalizations();
void loadHospitalizations();

#endif // DATA_H
