#ifndef HOSPITALIZATION_H
#define HOSPITALIZATION_H

#include "utils.h"

// º¯ÊýÉùÃ÷
void hospitalizationManagement();
void addHospitalization();
void dischargePatient();
void findHospitalization();
void showCurrentHospitalizations();
void showAllHospitalizations();
Hospitalization* findHospitalizationById(const char *hosp_id);
void findHospitalizationByIdAndShow(const char *hosp_id);

#endif // HOSPITALIZATION_H
