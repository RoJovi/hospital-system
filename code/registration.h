#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "utils.h"


// º¯ÊýÉùÃ÷
void registrationManagement();
Registration* findRegistrationById(const char *reg_id);
Registration* findRegistrationByPatientId(const char *patient_id);
void addRegistration();
void addRegistrationForPatient(const char *patient_id, const char *phone);
void cancelRegistration();
void showSingleRegistrationDetail(const char *reg_id);
void showAllRegistrations();
void modifyRegistrationStatus();
void findRegistrationByPatient(const char *patient_id);

#endif // REGISTRATION_H
