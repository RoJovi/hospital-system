#include "doctor.h"
#include "ui.h"
#include "data.h"
#include "hashmap.h"

extern HashMap doctor_hash;
extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];

// 医生管理
void doctorManagement() {
    while(1) {
    	
    	saveDoctors(doctor_list);
    	
        const char *items[] = {
            "添加医生信息",
            "修改医生信息", 
            "删除医生信息",
            "查询医生信息",
            "显示所有医生",
            "返回上级菜单"
        };
        
        int choice = showMenu("医生信息管理", items, 6);
        
        switch(choice) {
            case 0: addDoctor(); break;
            case 1: modDoctor(); break;
            case 2: delDoctor(); break;
            case 3: findDoctor(); break;
            case 4: showAllDoctors(1); break;
            case 5: return;
            case -1: return;
        }
    }
}

// 添加医生
void addDoctor() {
    system("cls");
    
    int width = 60;
    int height = 18;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "添加医生信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Doctor newDoctor;
    
    if(!inputStringWithESC(newDoctor.id, MAX_ID, "医生ID: ", x + 3, y + 2)) {
        return;
    }
    
    // 检查ID是否重复
    if(findDoctorById(newDoctor.id) != NULL) {
        showMessage("该医生ID已存在！", RED);
        return;
    }
    
    if(!inputStringWithESC(newDoctor.name, MAX_NAME, "姓名: ", x + 3, y + 3)) {
        return;
    }
    
    if(!inputIntWithESC(&newDoctor.age, "年龄: ", x + 3, y + 4)) {
        return;
    }
    
    if(!inputStringWithESC(newDoctor.gender, 10, "性别(男/女): ", x + 3, y + 5)) {
        return;
    }
    
    if(!inputStringWithESC(newDoctor.phone, MAX_PHONE, "联系电话: ", x + 3, y + 6)) {
        return;
    }
    
    if(!inputStringWithESC(newDoctor.department, MAX_DEPARTMENT, "科室: ", x + 3, y + 7)) {
        return;
    }
    
    if(!inputStringWithESC(newDoctor.specialty, 50, "专业领域: ", x + 3, y + 8)) {
        return;
    }
    
    if(!inputStringWithESC(newDoctor.schedule, 50, "排班信息: ", x + 3, y + 9)) {
        return;
    }

    pos.X = x + 3; pos.Y = y + 12;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("请确认医生信息：");
    pos.X = x + 3; pos.Y = y + 13;
    printf("确定(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
	        // 添加到链表
	        Doctor *newNode = (Doctor*)malloc(sizeof(Doctor));
	        memcpy(newNode, &newDoctor, sizeof(Doctor));
	        newNode->next = doctor_list;
	        doctor_list = newNode;
	        
	        // 添加到哈希表
            hashMapInsert(&doctor_hash, newNode->id, newNode);
            
	        showMessage("医生信息添加成功！", GREEN);
	        return;
		}
        if(ch == 'n' || ch == 'N') {
        	showMessage("已取消添加医生", YELLOW);
        	return;
    	}
    }

    
}

// 显示单个医生的完整信息
void showSingleDoctorDetail(const char *id) {
    Doctor *p = findDoctorById(id);
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
    
    drawBox(x, y, x + width, y + height, CYAN, "医生详细信息");
    COORD pos;//坐标 
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("  医生id号: %s\n", p->id);
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
    printf("  科室: %s\n", p->department);
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    printf("  专业领域: %s\n", p->specialty);
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    printf("  排班信息: %s\n", p->schedule);
    pos.X = x + 3; pos.Y = y + 19;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}


// 显示所有医生
void showAllDoctors(int m) {

    int width = 80;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1){
		system("cls");
	    drawBox(x, y, x + width, y + height, CYAN, "所有医生信息");
	
	    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
	    COORD pos;
	    
	    if(!doctor_list) {
	        pos.X = x + 3; pos.Y = y + 2;
	        SetConsoleCursorPosition(a, pos);
	        setColor(YELLOW);
	        printf("暂无医生信息！");
	        setColor(WHITE);
	        pressAnyKey();
	        return;
	    }
	    
	    //逐一显示信息 
	    pos.X = x + 3; pos.Y = y + 2;
	    SetConsoleCursorPosition(a, pos);
	    setColor(YELLOW);
	    if(m)
	    	printf("%-5s %-10s %-10s %-5s %-5s %-15s %-10s", "序号","医生ID", "姓名", "年龄", "性别", "联系电话","科室");
	    else
	    	printf("%-5s %-8s %-5s %-5s %-15s %-15s %-10s", 
               "序号", "姓名", "性别", "年龄","科室", "擅长","排班");
	    setColor(WHITE);
	    
	    Doctor *temp = doctor_list;
	    int count = 0;
	    int currentY = y + 3;
	    char *doctorIds[100];
	    
	    if(!m){
		    while(temp && currentY < y + height - 2) {
		    	doctorIds[++count] = temp->id;
		        pos.X = x + 3; pos.Y = currentY++;
		        SetConsoleCursorPosition(a, pos);
		        printf("%-5d %-8s %-5s %-5d %-15s %-15s %-10s",
		               count, temp->name, temp->gender, temp->age,
                   temp->department, temp->specialty,temp->schedule);
				temp = temp->next;
	
		    }
		}
		else{
			while(temp && currentY < y + height - 2) {
		    	doctorIds[++count] = temp->id;
		        pos.X = x + 3; pos.Y = currentY++;
		        SetConsoleCursorPosition(a, pos);
		        printf("%-5d %-10s %-10s %-5d %-5s %-15s %-10s",
		               count,temp->id, temp->name, temp->age, temp->gender, temp->phone,temp->department);
				temp = temp->next;
	
		    }
		}
	    
	    pos.X = x + 3; pos.Y = y + height - 3;
	    SetConsoleCursorPosition(a, pos);
	    setColor(YELLOW);
	    printf("共 %d 位医生", count);

		//点菜 
		if(m){
	    int choice;
			if(!inputIntWithESC(&choice, "输入序号可查看详细信息：", x+3, y + height - 2))break;
			setColor(WHITE);
		    if(choice >  0 && choice <= count) {
		        if(!findDoctorById(doctorIds[choice])) {
		        	showMessage("未查询到此医生", RED);
				}
				else showSingleDoctorDetail(doctorIds[choice]);
		    }
		    else {
		    	showMessage("输入错误！请重试", RED);
			}
		}
		else {
			pressAnyKey();
			return;
		}
	}
	return; 
}

//查找医生信息
void findDoctor(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "查找医生信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    char id[20];
    
     // 输入医生ID
    if(!inputStringWithESC(id, MAX_ID, "医生ID: ", x + 3, y + 2)) {
        return;
    }
    
	Doctor *p1=findDoctorById(id);
	
	if(p1!=NULL){
		showSingleDoctorDetail(id);
		return;
    } 
    showMessage("未查询到此医生", RED);
	return;
} 

//删除医生
void delDoctor(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "删除医生信息");
    
    char id[20];
    
     // 输入医生ID
     if(!inputStringWithESC(id, MAX_ID, "医生ID: ", x + 3, y + 2)) {
        return;
    }
    
	Doctor *p1,*p2;
	if(doctor_list==NULL){
		showMessage("未查询到此医生", RED);
		return;
	}
	p1=doctor_list;
	if(strcmp(doctor_list->id,id)==0){
        doctor_list=doctor_list->next;
        free(p1);
        showMessage("已删除该医生信息", YELLOW);
        return;
    }
    while(strcmp(p1->id,id)!=0 &&p1->next!=NULL) {
        p2=p1;
        p1=p1->next;
    }
    if(strcmp(p1->id,id)==0){
    	 // 从哈希表移除
        hashMapRemove(&doctor_hash, p1->id);
        
        p2->next=p1->next;
        free(p1);
        showMessage("已删除该医生信息", YELLOW);
        return;
    } 
    showMessage("未查询到此医生", RED);
	return;
}

//修改医生信息 
void modDoctor(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "修改医生信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    char id[20];
    
     // 输入医生ID
     if(!inputStringWithESC(id, MAX_ID, "医生ID: ", x + 3, y + 2)) {
        return;
    }
    
	Doctor *p1=findDoctorById(id);
	
	if(p1!=NULL){
		while(1) {
        const char *items[] = {
            "姓名",
            "年龄",
            "性别（男/女）", 
            "联系电话",
            "科室",
            "专业领域",
            "排班信息",
            "返回"
        };
        
        int choice = showMenu("选择修改的项", items, 8);
        
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
	            drawBox(x, y, x + width, y + height, CYAN, "科室");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原结果: %s", p1->department);
				if(!inputStringWithESC(p1->department, MAX_DEPARTMENT, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 5:
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "专业领域");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原结果: %s", p1->specialty);
				if(!inputStringWithESC(p1->specialty, 50, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 6: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "排班信息");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原方案: %s", p1->schedule);
				if(!inputStringWithESC(p1->schedule, 50, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 7: 
            case -1: 
            	//showMessage("修改信息已保存", GREEN);
				return;
        }
    }
    } 
    showMessage("暂无该医生信息", RED);
	return;
}

// 查找医生
Doctor* findDoctorById(const char *id) {
    return (Doctor*)hashMapGet(&doctor_hash, id);
}

// 显示医生列表并让用户选择的函数
Doctor* selectDoctorFromList() {
    system("cls");
    
    int width = 80;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1) {
        system("cls");
        drawBox(x, y, x + width, y + height, CYAN, "选择医生");
        
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        
        if(!doctor_list) {
            pos.X = x + 3; pos.Y = y + 2;
            SetConsoleCursorPosition(a, pos);
            setColor(YELLOW);
            printf("暂无医生信息！");
            setColor(WHITE);
            
            pos.X = x + 3; pos.Y = y + 4;
            SetConsoleCursorPosition(a, pos);
            printf("按任意键返回...");
            _getch();
            return NULL;
        }
        
        // 显示表头（使用现有字段）
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("%-5s %-10s %-8s %-15s %-15s %-15s", 
               "序号", "姓名", "性别", "科室", "擅长","排班");
        setColor(WHITE);
        
        Doctor *temp = doctor_list;
        int count = 0;
        int currentY = y + 3;
        Doctor *doctor_ptrs[100];  // 保存医生指针
        
        while(temp && currentY < y + height - 3) {
            doctor_ptrs[++count] = temp;
            
            pos.X = x + 3; pos.Y = currentY++;
            SetConsoleCursorPosition(a, pos);
            
            printf("%-5d %-10s %-8s %-15s %-15s %-15s", 
                   count, temp->name, temp->gender, 
                   temp->department, temp->specialty,temp->schedule);
            
            temp = temp->next;
        }
        
        pos.X = x + 3; pos.Y = y + height - 3;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("共 %d 位医生", count);
        
        // 让用户选择
        int choice;
        if(!inputIntWithESC(&choice, "输入医生序号: ", x + 3, y + height - 2)) {
            return NULL;
        }
        
        if(choice > 0 && choice <= count) {
            return doctor_ptrs[choice];
        } else {
            showMessage("序号无效！", RED);
        }
    }
}
