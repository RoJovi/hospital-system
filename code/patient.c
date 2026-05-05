#include "patient.h"
#include "doctor.h"
#include "ui.h"
#include "data.h"
#include "hashmap.h"


extern HashMap patient_hash;
extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];

// 患者管理
void patientManagement() {
    while(1) {
    	savePatients(patient_list);
    	
        const char *items[] = {
            "添加患者信息",
            "修改患者信息",
            "删除患者信息", 
            "查询患者信息",
            "显示所有患者",
            "返回上级菜单"
        };
        
        int choice = showMenu("患者信息管理", items, 6);
        
        switch(choice) {
            case 0: addPatient(); break;
            case 1: modPatient(); break;
            case 2: delPatient(); break;
            case 3: findPatient(universal_id); break;
            case 4: showAllPatients(); break;
            case 5: return;
            case -1: return;
        }
    }
}

// 添加患者
void addPatient() {
    system("cls");
    
    int width = 60;
    int height = 18;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "添加患者信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Patient newPatient;
    
    // 输入患者ID
    if(!inputStringWithESC(newPatient.id, MAX_ID, "患者ID: ", x + 3, y + 2)) {
        return;
    }
    
    // 检查ID是否重复
    if(findPatientById(newPatient.id) != NULL) {
        showMessage("该患者ID已存在！", RED);
        return;
    }
    
    if(!inputStringWithESC(newPatient.name, MAX_NAME, "姓名: ", x + 3, y + 3)) {
        return;
    }
    
    if(!inputIntWithESC(&newPatient.age, "年龄: ", x + 3, y + 4)) {
        return;
    }
    
    if(!inputStringWithESC(newPatient.gender, 10, "性别(男/女): ", x + 3, y + 5)) {
        return;
    }
    
    if(!inputStringWithESC(newPatient.phone, MAX_PHONE, "联系电话: ", x + 3, y + 6)) {
        return;
    }
    
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("入院日期(年 月 日): ");
    
    // 分别输入年月日
    if(!inputIntWithESC(&newPatient.admission_date.year, "", x + 24, y + 7)) {
        return;
    }
    
    if(!inputIntWithESC(&newPatient.admission_date.month, "", x + 29, y + 7)) {
        return;
    }
    
    if(!inputIntWithESC(&newPatient.admission_date.day, "", x + 33, y + 7)) {
        return;
    }
    
    if(!inputStringWithESC(newPatient.diagnosis, MAX_DIAGNOSIS, "诊断结果: ", x + 3, y + 8)) {
        return;
    }
    
    if(!inputStringWithESC(newPatient.treatment, MAX_TREATMENT, "治疗方案: ", x + 3, y + 9)) {
        return;
    }
    
    if(!inputDoubleWithESC(&newPatient.total_cost, "总费用: ", x + 3, y + 10)) {
        return;
    }
    
    pos.X = x + 3; pos.Y = y + 12;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("请确认患者信息：");
    pos.X = x + 3; pos.Y = y + 13;
    printf("确定(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
	        // 添加到链表
	        Patient *newNode = (Patient*)malloc(sizeof(Patient));
	        memcpy(newNode, &newPatient, sizeof(Patient));
	        newNode->next = patient_list;
	        patient_list = newNode;
	        
	        // 添加到哈希表
            hashMapInsert(&patient_hash, newNode->id, newNode);
            
	        showMessage("患者信息添加成功！", GREEN);
	        return;
		}
        if(ch == 'n' || ch == 'N') {
        	showMessage("已取消添加患者", YELLOW);
        	return;
    	}
    }

    
}


// 显示单个患者的完整信息
void showSinglePatientDetail(const char *id) {
    Patient *p = findPatientById(id);
    system("cls");
    if(p == NULL) {
        showMessage("患者不存在", RED);
        return;
    }
    
    int width = 60;
    int height = 18;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "患者详细信息");
    COORD pos;//坐标 
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("  病历号: %s\n", p->id);
    pos.X = x + 3; pos.Y = y + 3;
    SetConsoleCursorPosition(a, pos);
    printf("  姓名: %s\n", p->name);
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("  年龄: %d\n", p->age);
    pos.X = x + 3; pos.Y = y + 5;
    SetConsoleCursorPosition(a, pos);
    printf("  性别: %s\n", p->gender);
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("  联系电话: %s\n", p->phone);
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("  入院日期: %d年%d月%d日\n", 
           p->admission_date.year,
           p->admission_date.month,
           p->admission_date.day);
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    printf("  诊断结果: %s\n", p->diagnosis);
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    printf("  治疗方案: %s\n", p->treatment);
    pos.X = x + 3; pos.Y = y + 10;
    SetConsoleCursorPosition(a, pos);
    printf("  总费用: %.2f元\n", p->total_cost);
    pos.X = x + 3; pos.Y = y + 19;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}


// 显示所有患者
void showAllPatients() {

    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1){
		system("cls");
	    drawBox(x, y, x + width, y + height, CYAN, "所有患者信息");
	
	    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
	    COORD pos;
	    
	    if(!patient_list) {
	        pos.X = x + 3; pos.Y = y + 2;
	        SetConsoleCursorPosition(a, pos);
	        setColor(YELLOW);
	        printf("暂无患者信息！");
	        setColor(WHITE);
	        pressAnyKey();
	        return;
	    }
	    
	    //逐一显示信息 
	    pos.X = x + 3; pos.Y = y + 2;
	    SetConsoleCursorPosition(a, pos);
	    setColor(YELLOW);
	    printf("%-5s %-10s %-10s %-5s %-5s %-15s", "序号","患者ID", "姓名", "年龄", "性别", "联系电话");
	    setColor(WHITE);
	    
	    Patient *temp = patient_list;
	    int count = 0;
	    int currentY = y + 3;
	    char *patientIds[100];
	    
	    while(temp && currentY < y + height - 2) {
	    	patientIds[++count] = temp->id;
	        pos.X = x + 3; pos.Y = currentY++;
	        SetConsoleCursorPosition(a, pos);
	        printf("%-5d %-10s %-10s %-5d %-5s %-15s",
	               count,temp->id, temp->name, temp->age, temp->gender, temp->phone);
			temp = temp->next;

	    }
	    
	    pos.X = x + 3; pos.Y = y + height - 3;
	    SetConsoleCursorPosition(a, pos);
	    setColor(YELLOW);
	    printf("共 %d 位患者", count);

		//点菜 

	    int choice;
		if(!inputIntWithESC(&choice, "输入序号可查看详细信息：", x+3, y + height - 2))break;
		setColor(WHITE);
	    if(choice >  0 && choice <= count) {
	        if(!findPatientById(patientIds[choice])) {
	        	showMessage("暂无此患者", RED);
			}
			else showSinglePatientDetail(patientIds[choice]);
	    }
	    else {
	    	showMessage("输入错误！请重试", RED);
		}
	}
	return; 
}

// 用id查找患者
Patient* findPatientById(const char *id) {
    return (Patient*)hashMapGet(&patient_hash, id);
}

// 用电话号码查找患者
Patient* findPatientByPhone(const char *phone) {
    Patient *temp = patient_list;
    while(temp) {
        if(strcmp(temp->phone,phone) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

//查找患者信息
void findPatient(const char *id){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "查找患者信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    char patient_id[20];
    
     // 输入患者ID
    if(id[0]=='\0'){
	    if(!inputStringWithESC(patient_id, MAX_ID, "患者ID: ", x + 3, y + 2)) {
	        return;
	    }
	}
    else strcpy(patient_id,id);
    
	Patient *p1=findPatientById(patient_id);
	
	if(p1!=NULL){
		showSinglePatientDetail(patient_id);
		return;
    } 
    showMessage("暂无该患者信息", RED);
	return;
} 

//删除患者 
void delPatient(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "删除患者信息");
    
    char id[20];
    
     // 输入患者ID
     if(!inputStringWithESC(id, MAX_ID, "患者ID: ", x + 3, y + 2)) {
        return;
    }
    
	Patient *p1,*p2;
	if(patient_list==NULL){
		showMessage("暂无该患者信息", RED);
		return;
	}
	p1=patient_list;
	if(strcmp(patient_list->id,id)==0){
		// 从哈希表中移除
        hashMapRemove(&patient_hash, patient_list->id);
        
        patient_list=patient_list->next;
        free(p1);
        showMessage("已删除该患者信息", YELLOW);
        return;
    }
    while(strcmp(p1->id,id)!=0 &&p1->next!=NULL) {
        p2=p1;
        p1=p1->next;
    }
    if(strcmp(p1->id,id)==0){
    	// 从哈希表中移除
        hashMapRemove(&patient_hash, p1->id);
        
        p2->next=p1->next;
        free(p1);
        showMessage("已删除该患者信息", YELLOW);
        return;
    } 
    showMessage("暂无该患者信息", RED);
	return;
}

//修改患者信息 
void modPatient(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "修改患者信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    char id[20];
    
     // 输入患者ID
     if(!inputStringWithESC(id, MAX_ID, "患者ID: ", x + 3, y + 2)) {
        return;
    }
    
	Patient *p1=findPatientById(id);
	
	if(p1!=NULL){
		while(1) {
        const char *items[] = {
            "姓名",
            "年龄",
            "性别（男/女）", 
            "联系电话",
            "入院日期",
            "诊断结果",
            "治疗方案",
            "总费用",
            "返回"
        };
        
        int choice = showMenu("选择修改的项", items, 9);
        
        switch(choice) {
            case 0: 
	            system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "姓名");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原姓名: %s", p1->name);
				if(!inputStringWithESC(p1->name, MAX_NAME, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 1: 
	            system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "年龄");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原年龄: %d", p1->age);
				if(!inputIntWithESC(&(p1->age), "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
	    		break;
            case 2: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "性别（男/女）");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原性别: %s", p1->gender);
				if(!inputStringWithESC(p1->gender, 10, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 3: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "联系电话");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原电话: %s", p1->phone);
				if(!inputStringWithESC(p1->phone, MAX_PHONE, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 4: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "入院日期");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原日期: %d 年 %d 月 %d日：", 
				p1->admission_date.year,p1->admission_date.month,p1->admission_date.day);
                 pos.X = x + 3; pos.Y = y + 7;
    			SetConsoleCursorPosition(a, pos);
    			printf("更改为(年 月 日): ");
            	if(!inputIntWithESC(&p1->admission_date.year, "", x + 20, y + 7)) {
			        continue;
			    }
			    
			    if(!inputIntWithESC(&p1->admission_date.month, "", x + 25, y + 7)) {
			        continue;
			    }
			    
			    if(!inputIntWithESC(&p1->admission_date.day, "", x + 28, y + 7)) {
			        continue;
			    }
			    break;
            case 5:
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "诊断结果");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原结果: %s", p1->diagnosis);
				if(!inputStringWithESC(p1->diagnosis, MAX_DIAGNOSIS, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 6: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "治疗方案");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原方案: %s", p1->treatment);
				if(!inputStringWithESC(p1->treatment, MAX_TREATMENT, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 7: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "总花费");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原花费: %lf", p1->total_cost);
				if(!inputDoubleWithESC(&(p1->total_cost), "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
	    		break;
            case 8: 
            case -1: 
				return;
        }
    }
    } 
    showMessage("暂无该患者信息", RED);
	return;
}

int patientLogin(char *patient_id, char *phone) {
    system("cls");
    
    int width = 50;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "患者登录");
    
    char input_phone[20];
    
    // 输入手机号
    if(!inputStringWithESC(input_phone, 20, "请输入手机号: ", x + 3, y + 2)) {
        return 0;  // 用户取消
    }
    
    // 查找患者
    Patient *p = findPatientByPhone(input_phone);
    
    if(p == NULL) {
        // 新患者
        strcpy(patient_id, input_phone);  // 临时用手机号作为ID
        strcpy(phone, input_phone);
        showMessage("临时登录", GREEN);
    } 
	else {
        // 老患者
        strcpy(patient_id, p->id);
        strcpy(phone, input_phone);
        
        char msg[100];
        sprintf(msg, "欢迎回来，%s！", p->name);
        showMessage(msg, GREEN);
    }
    return 1;
}

// 患者专用挂号函数
void addRegistrationForPatient(const char *patient_id, const char *phone) {
    if(patient_id[0] == '\0' || phone[0] == '\0') {
        showMessage("请先登录！", RED);
        return;
    }
    
    system("cls");
    
    int width = 60;
    int height = 15;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "患者挂号");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    // 显示当前患者
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("当前患者手机号: %s", phone);
    
    // 先显示医生列表供选择
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("正在加载医生列表...");
    
    Sleep(500);  // 短暂等待
    
    // 调用医生选择函数
    Doctor *selected_doctor = selectDoctorFromList();
    
    if(selected_doctor == NULL) {
        showMessage("未选择医生，挂号取消", YELLOW);
        return;
    }
    
    // 清屏显示确认信息
    system("cls");
    drawBox(x, y, x + width, y + height, CYAN, "确认挂号信息");
    
    // 固定挂号费
    double registration_fee = 15.00;
    
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("===== 挂号信息 =====\n");
    
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("患 者: %s\n", phone);
    
    pos.X = x + 3; pos.Y = y + 5;
    SetConsoleCursorPosition(a, pos);
    printf("医 生: %s\n", selected_doctor->name);
    
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("科 室: %s\n", selected_doctor->department);
    
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("挂号费: 15.00 元\n");
    
    // 确认挂号
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("确认挂号(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
        	Patient *existing_patient = findPatientById(patient_id);
        	char name[MAX_NAME];
        	int age;
        	char gender[10];
        	if(existing_patient == NULL && strcmp(patient_id, phone) == 0){
        		system("cls"); 
        		drawBox(x, y, x + width, y + height, CYAN, "请先完善个人信息");
        		if(!inputStringWithESC(name, MAX_NAME, "姓名: ", x + 3, y + 3)) {
        			return;
			    }
			    
			    if(!inputIntWithESC(&age, "年龄: ", x + 3, y + 4)) {
			        return;
			    }
			    
			    if(!inputStringWithESC(gender, 10, "性别(男/女): ", x + 3, y + 5)) {
			        return;
			    }
			}
            // 创建挂号记录
            Registration newReg;
            
            // 生成挂号单号
            time_t t = time(NULL);
		    struct tm *tm_info = localtime(&t);
		    static int counter = 1;
		    sprintf(newReg.reg_id, "GH%04d%02d%02d%03d", 
            tm_info->tm_year + 1900, 
            tm_info->tm_mon + 1, 
            tm_info->tm_mday, 
            counter++);
            
            // 填充信息
            strcpy(newReg.patient_id, patient_id);
            strcpy(newReg.doctor_id, selected_doctor->id);
            strcpy(newReg.department, selected_doctor->department);
            newReg.reg_fee = registration_fee;
            newReg.status = 1;  // 已挂号
            
            // 设置日期
            newReg.reg_date.year = 2024;
            newReg.reg_date.month = 1;
            newReg.reg_date.day = 1;
            
            // 添加到链表
            Registration *newNode = (Registration*)malloc(sizeof(Registration));
            memcpy(newNode, &newReg, sizeof(Registration));
            newNode->next = registration_list;
            registration_list = newNode;
            
            // 创建挂号费用
            autoAddCost(patient_id, "挂号费", 15.00);
            
            // 新患者创建档案
            if(existing_patient == NULL && strcmp(patient_id, phone) == 0) {
                Patient newPatient;
                strcpy(newPatient.id, patient_id);
                strcpy(newPatient.phone, phone);
                strcpy(newPatient.name, name);
				strcpy(newPatient.gender, gender);
				strcpy(newPatient.diagnosis, "");
				strcpy(newPatient.treatment, "");
				newPatient.age = age;
                newPatient.admission_date.year = 0;
				newPatient.admission_date.month = 0;
				newPatient.admission_date.day = 0;
                newPatient.total_cost = 15.00;
                
                // 添加到链表
                Patient *newPatientNode = (Patient*)malloc(sizeof(Patient));
                memcpy(newPatientNode, &newPatient, sizeof(Patient));
                newPatientNode->next = patient_list;
                patient_list = newPatientNode;
            }
			showMessage("挂号成功！", GREEN);
            return;
        }
        if(ch == 'n' || ch == 'N') {
            showMessage("已取消挂号", YELLOW);
            return;
        }
    }
}
