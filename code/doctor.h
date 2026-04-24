#ifndef DOCTOR_H
#define DOCTOR_H

#include "utils.h"

// º¯ÊýÉùÃ÷
void doctorManagement();
void addDoctor();
void showAllDoctors(int m);
Doctor* findDoctorById(const char *id);
void showSingleDoctorDetail(const char *id);
void findDoctor();
void delDoctor();
void modDoctor();
Doctor*selectDoctorFromList ();

#endif // DOCTOR_H
