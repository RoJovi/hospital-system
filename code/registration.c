#include "registration.h"
#include "ui.h"
#include "data.h"
#include "patient.h"
#include "doctor.h"

extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];

// 查找挂号记录
Registration* findRegistrationById(const char *reg_id) {
    Registration *temp = registration_list;
    while(temp) {
        if(strcmp(temp->reg_id, reg_id) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 查找患者的挂号记录
Registration* findRegistrationByPatientId(const char *patient_id) {
    Registration *temp = registration_list;
    while(temp) {
        if(strcmp(temp->patient_id, patient_id) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 挂号登记
void addRegistration() {
    system("cls");
    
    int width = 60;
    int height = 18;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "挂号登记");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Registration newReg;
    
    // 生成挂号单号 (GH + 年月日 + 序号)
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    static int counter = 1;
    sprintf(newReg.reg_id, "GH%04d%02d%02d%03d", 
            tm_info->tm_year + 1900, 
            tm_info->tm_mon + 1, 
            tm_info->tm_mday, 
            counter++);
    
    // 显示生成的挂号单号
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("挂号单号: %s", newReg.reg_id);
    
    // 输入患者ID
    pos.X = x + 3; pos.Y = y + 3;
    SetConsoleCursorPosition(a, pos);
    printf("患者ID: ");
    
    // 这里可以添加患者ID验证
    if(!inputStringWithESC(newReg.patient_id, MAX_ID, "", x + 11, y + 3)) {
        return;
    }
    
    // 输入医生ID
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("医生ID: ");
    
    // 这里可以添加医生ID验证
    if(!inputStringWithESC(newReg.doctor_id, MAX_ID, "", x + 11, y + 4)) {
        return;
    }
    
    // 输入科室
    if(!inputStringWithESC(newReg.department, MAX_DEPARTMENT, "科室: ", x + 3, y + 5)) {
        return;
    }
    
    // 输入挂号费用
    if(!inputDoubleWithESC(&newReg.reg_fee, "挂号费: ", x + 3, y + 6)) {
        return;
    }
    
    // 设置挂号日期为当前日期
    newReg.reg_date.year = tm_info->tm_year + 1900;
    newReg.reg_date.month = tm_info->tm_mon + 1;
    newReg.reg_date.day = tm_info->tm_mday;
    
    // 挂号状态 (1:已挂号, 2:已就诊, 3:已取消)
    newReg.status = 1;
    
    // 显示汇总信息
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    setColor(GREEN);
    printf("===== 挂号信息汇总 =====");
    pos.X = x + 3; pos.Y = y + 10;
    SetConsoleCursorPosition(a, pos);
    printf("挂号单号: %s", newReg.reg_id);
    pos.X = x + 3; pos.Y = y + 11;
    SetConsoleCursorPosition(a, pos);
    printf("患者ID: %s", newReg.patient_id);
    pos.X = x + 3; pos.Y = y + 12;
    SetConsoleCursorPosition(a, pos);
    printf("医生ID: %s", newReg.doctor_id);
    pos.X = x + 3; pos.Y = y + 13;
    SetConsoleCursorPosition(a, pos);
    printf("科室: %s", newReg.department);
    pos.X = x + 3; pos.Y = y + 14;
    SetConsoleCursorPosition(a, pos);
    printf("挂号费: %.2f元", newReg.reg_fee);
    
    pos.X = x + 3; pos.Y = y + 16;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("确认挂号(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
            // 添加到链表
            Registration *newNode = (Registration*)malloc(sizeof(Registration));
            memcpy(newNode, &newReg, sizeof(Registration));
            newNode->next = registration_list;
            registration_list = newNode;
            autoAddCost(newReg.patient_id, "挂号费", 15.00);
            showMessage("挂号成功！", GREEN);
            return;
        }
        if(ch == 'n' || ch == 'N') {
            showMessage("已取消挂号", YELLOW);
            return;
        }
    }
}

// 取消挂号
void cancelRegistration() {
    system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "取消挂号");
    
    char reg_id[20];
    
    // 输入挂号单号
    if(!inputStringWithESC(reg_id, MAX_ID, "挂号单号: ", x + 3, y + 2)) {
        return;
    }
    
    Registration *p = findRegistrationById(reg_id);
    
    if(p == NULL) {
        showMessage("未找到该挂号记录！", RED);
        return;
    }
    
    if(p->status == 3) {
        showMessage("该挂号已取消！", YELLOW);
        return;
    }
    
    if(p->status == 2) {
        showMessage("该挂号已就诊，不能取消！", RED);
        return;
    }
    
    // 确认取消
    int width2 = 40;
    int height2 = 8;
    int x2 = (screenWidth - width2) / 2;
    int y2 = (screenHeight - height2) / 2;
    
    drawBox(x2, y2, x2 + width2, y2 + height2, YELLOW, "确认取消挂号");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    pos.X = x2 + 3; pos.Y = y2 + 2;
    SetConsoleCursorPosition(a, pos);
    printf("挂号单号: %s", p->reg_id);
    pos.X = x2 + 3; pos.Y = y2 + 3;
    SetConsoleCursorPosition(a, pos);
    printf("患者ID: %s", p->patient_id);
    pos.X = x2 + 3; pos.Y = y2 + 4;
    SetConsoleCursorPosition(a, pos);
    printf("医生ID: %s", p->doctor_id);
    
    pos.X = x2 + 3; pos.Y = y2 + 6;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("确定取消挂号？(Y/N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
            p->status = 3;  // 设置为已取消
            showMessage("挂号已取消！", GREEN);
            return;
        }
        if(ch == 'n' || ch == 'N') {
            showMessage("操作已取消", YELLOW);
            return;
        }
    }
}

// 显示单个挂号记录的详细信息
void showSingleRegistrationDetail(const char *reg_id) {
    Registration *p = findRegistrationById(reg_id);
    system("cls");
    if(p == NULL) {
        showMessage("挂号记录不存在", RED);
        return;
    }
    
    int width = 60;
    int height = 16;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "挂号详细信息");
    
    COORD pos;
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("  挂号单号: %s\n", p->reg_id);
    
    pos.X = x + 3; pos.Y = y + 3;
    SetConsoleCursorPosition(a, pos);
    printf("  患者ID: %s\n", p->patient_id);
    
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("  医生ID: %s\n", p->doctor_id);
    
    pos.X = x + 3; pos.Y = y + 5;
    SetConsoleCursorPosition(a, pos);
    printf("  挂号日期: %d年%d月%d日\n", 
           p->reg_date.year, p->reg_date.month, p->reg_date.day);
    
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("  科室: %s\n", p->department);
    
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("  挂号费用: %.2f元\n", p->reg_fee);
    
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    printf("  挂号状态: ");
    switch(p->status) {
        case 1: printf("已挂号"); break;
        case 2: printf("已就诊"); break;
        case 3: printf("已取消"); break;
        default: printf("未知");
    }
    printf("\n");
    
    pos.X = x + 3; pos.Y = y + 11;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}

// 显示所有挂号记录
void showAllRegistrations() {
    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1) {
        system("cls");
        drawBox(x, y, x + width, y + height, CYAN, "所有挂号记录");
        
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        
        if(!registration_list) {
            pos.X = x + 3; pos.Y = y + 2;
            SetConsoleCursorPosition(a, pos);
            setColor(YELLOW);
            printf("暂无挂号记录！");
            setColor(WHITE);
            pressAnyKey();
            return;
        }
        
        // 显示表头
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("%-5s %-15s %-12s %-12s %-10s %-8s %-10s", 
               "序号", "挂号单号", "患者ID", "医生ID", "科室", "费用", "状态");
        setColor(WHITE);
        
        Registration *temp = registration_list;
        int count = 0;
        int currentY = y + 3;
        char *regIds[100];
        
        while(temp && currentY < y + height - 3) {
            regIds[++count] = temp->reg_id;
            
            pos.X = x + 3; pos.Y = currentY++;
            SetConsoleCursorPosition(a, pos);
            
            printf("%-5d %-15s %-12s %-12s %-10s %-8.2f ", 
                   count, temp->reg_id, temp->patient_id, temp->doctor_id, 
                   temp->department, temp->reg_fee);
            
            // 显示状态
            switch(temp->status) {
                case 1: printf("已挂号"); break;
                case 2: printf("已就诊"); break;
                case 3: printf("已取消"); break;
                default: printf("未知");
            }
            
            temp = temp->next;
        }
        
        pos.X = x + 3; pos.Y = y + height - 3;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("共 %d 条挂号记录", count);
        
        // 查看详细
        int choice;
        if(!inputIntWithESC(&choice, "输入序号查看详情(0返回): ", x + 3, y + height - 2)) {
            break;
        }
        setColor(WHITE);
        
        if(choice == 0) {
            break;
        } else if(choice > 0 && choice <= count) {
            showSingleRegistrationDetail(regIds[choice]);
        } else {
            showMessage("输入错误！请重试", RED);
        }
    }
    return;
}

// 修改挂号信息（如状态更新）
void modifyRegistrationStatus() {
    system("cls");
    
    int width = 60;
    int height = 15;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "修改挂号状态");
    
    char reg_id[20];
    
    // 输入挂号单号
    if(!inputStringWithESC(reg_id, MAX_ID, "挂号单号: ", x + 3, y + 2)) {
        return;
    }
    
    Registration *p = findRegistrationById(reg_id);
    
    if(p == NULL) {
        showMessage("未找到该挂号记录！", RED);
        return;
    }
    
    // 显示当前信息
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("当前状态: ");
    switch(p->status) {
        case 1: printf("已挂号"); break;
        case 2: printf("已就诊"); break;
        case 3: printf("已取消"); break;
        default: printf("未知");
    }
    
    // 选择新状态 - 使用简单的选择方式
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("选择新状态:");
    
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("1. 已挂号");
    
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    printf("2. 已就诊");
    
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    printf("3. 已取消");
    
    pos.X = x + 3; pos.Y = y + 10;
    SetConsoleCursorPosition(a, pos);
    printf("0. 返回");
    
    pos.X = x + 3; pos.Y = y + 12;
    SetConsoleCursorPosition(a, pos);
    printf("请选择(0-3): ");
    
    int choice;
    if(!inputIntWithESC(&choice, "", x + 17, y + 12)) {
        return;
    }
    
    switch(choice) {
        case 1: p->status = 1; break;
        case 2: p->status = 2; break;
        case 3: p->status = 3; break;
        case 0: return;
        default: 
            showMessage("无效选择！", RED);
            return;
    }
    
    showMessage("状态更新成功！", GREEN);
}

// 根据患者ID查询挂号记录
void findRegistrationByPatient(const char *id) {
    system("cls");
    
    int width = 60;
    int height = 15;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "按患者查询挂号");
    
    char patient_id[20];
    
    // 输入患者ID
    if(id[0]=='\0'){
	    if(!inputStringWithESC(patient_id, MAX_ID, "患者ID: ", x + 3, y + 2)) {
	        return;
	    }
	}
    else strcpy(patient_id,id);
    
    system("cls");
    drawBox(x, y, x + width, y + height, CYAN, "患者挂号记录");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Registration *temp = registration_list;
    int count = 0;
    int found = 0;
    
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("患者ID: %s", patient_id);
    
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("挂号记录:");
    setColor(WHITE);
    
    int currentY = y + 5;
    
    while(temp) {
        if(strcmp(temp->patient_id, patient_id) == 0) {
            found = 1;
            count++;
            
            pos.X = x + 3; pos.Y = currentY++;
            SetConsoleCursorPosition(a, pos);
            printf("%d. 单号: %s, 医生: %s, 科室: %s, 状态: ", 
                   count, temp->reg_id, temp->doctor_id, temp->department);
            
            switch(temp->status) {
                case 1: printf("已挂号"); break;
                case 2: printf("已就诊"); break;
                case 3: printf("已取消"); break;
                default: printf("未知");
            }
        }
        temp = temp->next;
    }
    
    if(!found) {
        pos.X = x + 3; pos.Y = currentY;
        SetConsoleCursorPosition(a, pos);
        setColor(RED);
        printf("未找到该患者的挂号记录");
        setColor(WHITE);
    }
    
    pos.X = x + 3; pos.Y = y + height - 2;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}

// 挂号管理菜单
void registrationManagement() {
    while(1) {
    	
		saveRegistrations();
    	
        const char *items[] = {
            "挂号登记",
            "取消挂号", 
            "修改挂号状态",
            "显示所有挂号记录",
            "按患者查询挂号",
            "返回上级菜单"
        };
        
        int choice = showMenu("挂号管理", items, 6);
        
        switch(choice) {
            case 0: addRegistration(); break;
            case 1: cancelRegistration(); break;
            case 2: modifyRegistrationStatus(); break;
            case 3: showAllRegistrations(); break;
            case 4: findRegistrationByPatient(universal_id); break;
            case 5: return;
            case -1: return;
        }
    }
}

