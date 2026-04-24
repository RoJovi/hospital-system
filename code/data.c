#include "data.h"
#include "ui.h"

extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];


// 保存所有数据
void saveAllData() {
	savePatients(patient_list); 
	saveDoctors(doctor_list); 
	saveMedicines(medicine_list); 
	saveRegistrations();
	saveCosts();
	saveHospitalizations();
    showMessage("数据保存成功！", GREEN);
}

void savePatients(Patient *head) {
    system("mkdir data 2>nul");
    
    FILE *fp = fopen("data/patients.dat", "wb");
    if(fp == NULL) return;
    
    Patient *current = head;
    while(current != NULL) {
        // 关键修复：创建临时副本
        Patient temp = *current;
        temp.next = NULL;  // 清空指针
        
        fwrite(&temp, sizeof(Patient), 1, fp);
        current = current->next;
    }
    
    fclose(fp);
}

Patient* loadPatients() {
    FILE *fp = fopen("data/patients.dat", "rb");
    if(fp == NULL) {
        return NULL;  // 文件不存在，返回空链表
    }
    
    Patient *head = NULL;
    Patient *tail = NULL;
    Patient temp;
    
    while(fread(&temp, sizeof(Patient), 1, fp) == 1) {
        Patient *newNode = (Patient*)malloc(sizeof(Patient));
        if(newNode == NULL) {
            fclose(fp);
            return head;
        }
        
        // 复制数据
        memcpy(newNode, &temp, sizeof(Patient));
        newNode->next = NULL;
        
        // 添加到链表
        if(head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    fclose(fp);
    return head;
}

void saveDoctors(Doctor *head) {
    system("mkdir data 2>nul");
    
    FILE *fp = fopen("data/doctors.dat", "wb");
    if(fp == NULL) return;
    
    Doctor *current = head;
    while(current != NULL) {
        Doctor temp = *current;
        temp.next = NULL;
        
        fwrite(&temp, sizeof(Doctor), 1, fp);
        current = current->next;
    }
    
    fclose(fp);
}

Doctor* loadDoctors() {
    FILE *fp = fopen("data/doctors.dat", "rb");
    if(fp == NULL) {
        return NULL;  // 文件不存在，返回空链表
    }
    
    Doctor *head = NULL;
    Doctor *tail = NULL;
    Doctor temp;
    
    while(fread(&temp, sizeof(Doctor), 1, fp) == 1) {
        Doctor *newNode = (Doctor*)malloc(sizeof(Doctor));
        if(newNode == NULL) {
            fclose(fp);
            return head;
        }
        
        // 复制数据
        memcpy(newNode, &temp, sizeof(Doctor));
        newNode->next = NULL;
        
        // 添加到链表
        if(head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    fclose(fp);
    return head;
}

void saveMedicines(Medicine *head) {
    system("mkdir data 2>nul");
    
    FILE *fp = fopen("data/medicines.dat", "wb");
    if(fp == NULL) return;
    
    Medicine *current = head;
    while(current != NULL) {
        Medicine temp = *current;
        temp.next = NULL;
        
        fwrite(&temp, sizeof(Medicine), 1, fp);
        current = current->next;
    }
    
    fclose(fp);
}

Medicine* loadMedicines() {
    FILE *fp = fopen("data/medicines.dat", "rb");
    if(fp == NULL) {
        return NULL;  // 文件不存在，返回空链表
    }
    
    Medicine *head = NULL;
    Medicine *tail = NULL;
    Medicine temp;
    
    while(fread(&temp, sizeof(Medicine), 1, fp) == 1) {
        Medicine *newNode = (Medicine*)malloc(sizeof(Medicine));
        if(newNode == NULL) {
            fclose(fp);
            return head;
        }
        
        // 复制数据
        memcpy(newNode, &temp, sizeof(Medicine));
        newNode->next = NULL;
        
        // 添加到链表
        if(head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    fclose(fp);
    return head;
}

// 保存挂号数据
void saveRegistrations() {
    system("mkdir data 2>nul");
    
    FILE *fp = fopen("data/registrations.dat", "wb");
    if(fp == NULL) return;
    
    Registration *current = registration_list;
    while(current != NULL) {
        Registration temp = *current;
        temp.next = NULL;
        
        fwrite(&temp, sizeof(Registration), 1, fp);
        current = current->next;
    }
    
    fclose(fp);
}

// 从文件加载挂号数据
void loadRegistrations() {
    FILE *fp = fopen("registrations.dat", "rb");
    if(fp == NULL) return;
    
    Registration reg;
    while(fread(&reg, sizeof(Registration), 1, fp)) {
        Registration *newNode = (Registration*)malloc(sizeof(Registration));
        memcpy(newNode, &reg, sizeof(Registration));
        newNode->next = registration_list;
        registration_list = newNode;
    }
    
    fclose(fp);
}

// 保存费用数据到文件
void saveCosts() {
    system("mkdir data 2>nul");
    
    FILE *fp = fopen("data/costs.dat", "wb");
    if(fp == NULL) return;
    
    Cost *current = cost_list;
    while(current != NULL) {
        Cost temp = *current;
        temp.next = NULL;
        
        fwrite(&temp, sizeof(Cost), 1, fp);
        current = current->next;
    }
    
    fclose(fp);
}

// 从文件加载费用数据
void loadCosts() {
    FILE *fp = fopen("costs.dat", "rb");
    if(fp == NULL) return;
    
    Cost cost;
    while(fread(&cost, sizeof(Cost), 1, fp)) {
        Cost *newNode = (Cost*)malloc(sizeof(Cost));
        memcpy(newNode, &cost, sizeof(Cost));
        newNode->next = cost_list;
        cost_list = newNode;
    }
    
    fclose(fp);
}

// 保存住院数据
void saveHospitalizations() {
    system("mkdir data 2>nul");
    
    FILE *fp = fopen("data/hospitalizations.dat", "wb");
    if(fp == NULL) return;
    
    Hospitalization *current = hosp_list;
    while(current != NULL) {
        Hospitalization temp = *current;
        temp.next = NULL;
        fwrite(&temp, sizeof(Hospitalization), 1, fp);
        current = current->next;
    }
    
    fclose(fp);
}

// 加载住院数据
void loadHospitalizations() {
    // 清空现有链表
    Hospitalization *temp;
    while(hosp_list) {
        temp = hosp_list;
        hosp_list = hosp_list->next;
        free(temp);
    }
    
    FILE *fp = fopen("data/hospitalizations.dat", "rb");
    if(fp == NULL) return;
    
    Hospitalization tempHosp;
    Hospitalization *tail = NULL;
    
    while(fread(&tempHosp, sizeof(Hospitalization), 1, fp) == 1) {
        Hospitalization *newNode = (Hospitalization*)malloc(sizeof(Hospitalization));
        if(newNode == NULL) break;
        
        memcpy(newNode, &tempHosp, sizeof(Hospitalization));
        newNode->next = NULL;
        
        if(hosp_list == NULL) {
            hosp_list = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    fclose(fp);
}
