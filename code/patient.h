#ifndef PATIENT_H
#define PATIENT_H

#include "utils.h"

// º¯ÊýÉùÃ÷
void patientManagement();
void addPatient();
void showSinglePatientDetail(const char *id);
void findPatient(const char *id);
void delPatient();
void modPatient();
void showAllPatients();
Patient* findPatientById(const char *id);
Patient* findPatientByPhone(const char *phone);
int patientLogin(char *patient_id, char *phone);
void addRegistrationForPatient(const char *patient_id, const char *phone);

#endif // PATIENT_H
