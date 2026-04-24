#include "hospitalization.h"
#include "ui.h"
#include "data.h"
#include "patient.h"
#include "doctor.h"
#include "cost.h"

extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];

// 住院管理主菜单
void hospitalizationManagement() {
    while(1) {
        const char *items[] = {
            "办理入院登记",
            "办理出院手续", 
            "查询住院记录",
            "显示所有在院患者",
            "显示所有住院记录",
            "返回上级菜单"
        };
        
        int choice = showMenu("住院管理", items, 6);
        
        switch(choice) {
            case 0: addHospitalization(); break;
            case 1: dischargePatient(); break;
            case 2: findHospitalization(); break;
            case 3: showCurrentHospitalizations(); break;
            case 4: showAllHospitalizations(); break;
            case 5: 
            case -1:
                saveHospitalizations(); 
                return;
        }
    }
}

// 办理入院登记
void addHospitalization() {
    system("cls");
    
    int width = 60;
    int height = 15;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "办理入院登记");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Hospitalization newHosp;
    
    // 生成住院号
    static int hosp_counter = 1;
    sprintf(newHosp.hosp_id, "ZY%05d", hosp_counter++);
    
   
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("住院号: %s", newHosp.hosp_id);
    
    if(!inputStringWithESC(newHosp.patient_id, MAX_ID, "患者ID: ", x + 3, y + 3)) {
        return;
    }
    
    if(!inputStringWithESC(newHosp.doctor_id, MAX_ID, "主治医生ID: ", x + 3, y + 4)) {
        return;
    }
    
    if(!inputStringWithESC(newHosp.bed_number, 20, "床位号: ", x + 3, y + 5)) {
        return;
    }
    
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("入院日期(年 月 日): ");
    if(!inputIntWithESC(&newHosp.admission_date.year, "", x + 22, y + 6)) return;
    if(!inputIntWithESC(&newHosp.admission_date.month, "", x + 27, y + 6)) return;
    if(!inputIntWithESC(&newHosp.admission_date.day, "", x + 31, y + 6)) return;
    if(!inputStringWithESC(newHosp.diagnosis, MAX_DIAGNOSIS, "住院诊断: ", x + 3, y + 7)) {
        return;
    }
    
    // 初始总费用为0
    newHosp.total_cost = 0.0;
    
    // 状态为在院
    newHosp.status = 1;
    
    // 汇总信息
    pos.X = x + 3; pos.Y = y + 10;
    SetConsoleCursorPosition(a, pos);
    setColor(GREEN);
    printf("===== 入院信息汇总 =====");
    pos.X = x + 3; pos.Y = y + 11;
    printf("住院号: %s", newHosp.hosp_id);
    pos.X = x + 3; pos.Y = y + 12;
    printf("患者ID: %s", newHosp.patient_id);
    pos.X = x + 3; pos.Y = y + 13;
    printf("医生ID: %s", newHosp.doctor_id);
    pos.X = x + 3; pos.Y = y + 14;
    printf("床位号: %s", newHosp.bed_number);
    
    pos.X = x + 3; pos.Y = y + height - 3;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("确认办理入院(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
            // 添加到链表
            Hospitalization *newNode = (Hospitalization*)malloc(sizeof(Hospitalization));
            memcpy(newNode, &newHosp, sizeof(Hospitalization));
            newNode->next = hosp_list;
            hosp_list = newNode;
            Cost hospCost;
		    static int hosp_cost_counter = 1;
		    sprintf(hospCost.cost_id, "ZYF%05d", hosp_cost_counter++);
		    strcpy(hospCost.patient_id, newHosp.patient_id);
		    strcpy(hospCost.item, "住院费用");
		    hospCost.amount = newHosp.total_cost;
		    
		    // 使用入院日期
		    hospCost.cost_date = newHosp.admission_date;
		    hospCost.payment_status = 0;
		    
		    // 添加到费用链表
		    Cost *costNode = (Cost*)malloc(sizeof(Cost));
		    memcpy(costNode, &hospCost, sizeof(Cost));
		    costNode->next = cost_list;
		    cost_list = costNode;
		    
		    // 更新患者总费用
		    updatePatientTotalCost(newHosp.patient_id, newHosp.total_cost);
            showMessage("入院登记成功！", GREEN);
            return;
        }
        if(ch == 'n' || ch == 'N') {
            showMessage("已取消入院登记", YELLOW);
            return;
        }
    }
}

// 办理出院手续
void dischargePatient() {
    system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "办理出院手续");
    
    char hosp_id[20];
    
    // 输入住院号
    if(!inputStringWithESC(hosp_id, MAX_ID, "住院号: ", x + 3, y + 2)) {
        return;
    }
    
    // 查找住院记录
    Hospitalization *hosp = findHospitalizationById(hosp_id);
    if(hosp == NULL) {
        showMessage("未找到该住院记录！", RED);
        return;
    }
    
    if(hosp->status == 2) {
        showMessage("该患者已出院！", YELLOW);
        return;
    }
    
    // 确认出院
    if(!confirmBox("确认办理出院手续？")) {
        showMessage("已取消出院手续", YELLOW);
        return;
    }
    
    // 更新状态为已出院
    hosp->status = 2;
    showMessage("出院手续办理成功！", GREEN);
}




// 查询住院记录
void findHospitalization() {
    system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "查询住院记录");
    
    char hosp_id[20];
    
    // 输入住院号
    if(!inputStringWithESC(hosp_id, MAX_ID, "住院号: ", x + 3, y + 2)) {
        return;
    }
    
    // 查找并显示
    Hospitalization *hosp = findHospitalizationById(hosp_id);
    if(hosp == NULL) {
        showMessage("未找到该住院记录！", RED);
        return;
    }
    
    // 显示详细信息
    system("cls");
    drawBox(x, y, x + width, y + height + 5, CYAN, "住院详细信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("住院号: %s", hosp->hosp_id);
    
    pos.X = x + 3; pos.Y = y + 3;
    SetConsoleCursorPosition(a, pos);
    printf("患者ID: %s", hosp->patient_id);
    
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("主治医生ID: %s", hosp->doctor_id);
    
    pos.X = x + 3; pos.Y = y + 5;
    SetConsoleCursorPosition(a, pos);
    printf("入院日期: %d年%d月%d日", 
           hosp->admission_date.year, hosp->admission_date.month, hosp->admission_date.day);
    
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("床位号: %s", hosp->bed_number);
    
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("住院诊断: %s", hosp->diagnosis);
    
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    printf("总费用: %.2f元", hosp->total_cost);
    
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    printf("状态: %s", hosp->status == 1 ? "在院" : "已出院");
    
    pos.X = x + 3; pos.Y = y + height + 3;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}

// 显示所有在院患者
void showCurrentHospitalizations() {
    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1) {
        system("cls");
        drawBox(x, y, x + width, y + height, CYAN, "在院患者列表");
        
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        
        if(!hosp_list) {
            pos.X = x + 3; pos.Y = y + 2;
            SetConsoleCursorPosition(a, pos);
            setColor(YELLOW);
            printf("暂无在院患者！");
            setColor(WHITE);
            pressAnyKey();
            return;
        }
        
        // 显示表头
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("%-5s %-10s %-12s %-10s %-10s %-12s", 
               "序号", "住院号", "患者ID", "医生ID", "床位号", "入院日期");
        setColor(WHITE);
        
        Hospitalization *temp = hosp_list;
        int count = 0;
        int currentY = y + 3;
        char *hospIds[100];
        
        while(temp && currentY < y + height - 2) {
            if(temp->status == 1) {  // 只显示在院患者
                hospIds[++count] = temp->hosp_id;
                
                pos.X = x + 3; pos.Y = currentY++;
                SetConsoleCursorPosition(a, pos);
                
                printf("%-5d %-10s %-12s %-10s %-10s %d/%02d/%02d",
                       count, temp->hosp_id, temp->patient_id, temp->doctor_id,
                       temp->bed_number, temp->admission_date.year, 
                       temp->admission_date.month, temp->admission_date.day);
            }
            temp = temp->next;
        }
        
        if(count == 0) {
            pos.X = x + 3; pos.Y = y + 4;
            SetConsoleCursorPosition(a, pos);
            setColor(YELLOW);
            printf("暂无在院患者！");
            setColor(WHITE);
        } else {
            pos.X = x + 3; pos.Y = y + height - 3;
            SetConsoleCursorPosition(a, pos);
            setColor(GREEN);
            printf("共 %d 位在院患者", count);
            setColor(WHITE);
        }
        
        // 选择查看详情
        int choice;
        if(!inputIntWithESC(&choice, "输入序号查看详情(0返回): ", x + 3, y + height - 2)) {
            break;
        }
        
        if(choice == 0) {
            break;
        } else if(choice > 0 && choice <= count) {
            // 这里可以调用显示详细信息的函数
            // 简单起见，直接显示基本信息
            Hospitalization *hosp = findHospitalizationById(hospIds[choice]);
            if(hosp) {
                char msg[200];
                sprintf(msg, "住院号: %s\n患者ID: %s\n医生ID: %s\n床位: %s\n诊断: %s",
                        hosp->hosp_id, hosp->patient_id, hosp->doctor_id,
                        hosp->bed_number, hosp->diagnosis);
                showMessage(msg, CYAN);
            }
        } else {
            showMessage("输入错误！", RED);
        }
    }
}

// 显示所有住院记录
void showAllHospitalizations() {
    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1) {
        system("cls");
        drawBox(x, y, x + width, y + height, CYAN, "所有住院记录");
        
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        
        if(!hosp_list) {
            pos.X = x + 3; pos.Y = y + 2;
            SetConsoleCursorPosition(a, pos);
            setColor(YELLOW);
            printf("暂无住院记录！");
            setColor(WHITE);
            pressAnyKey();
            return;
        }
        
        // 显示表头
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("%-5s %-10s %-12s %-10s %-8s %-12s %-8s", 
               "序号", "住院号", "患者ID", "医生ID", "床位", "入院日期", "状态");
        setColor(WHITE);
        
        Hospitalization *temp = hosp_list;
        int count = 0;
        int currentY = y + 3;
        char *hospIds[100];
        
        while(temp && currentY < y + height - 2) {
            hospIds[++count] = temp->hosp_id;
            
            pos.X = x + 3; pos.Y = currentY++;
            SetConsoleCursorPosition(a, pos);
            
            printf("%-5d %-10s %-12s %-10s %-8s %d/%02d/%02d %-8s",
                   count, temp->hosp_id, temp->patient_id, temp->doctor_id,
                   temp->bed_number, temp->admission_date.year, 
                   temp->admission_date.month, temp->admission_date.day,
                   temp->status == 1 ? "在院" : "出院");
            temp = temp->next;
        }
        
        pos.X = x + 3; pos.Y = y + height - 3;
        SetConsoleCursorPosition(a, pos);
        setColor(GREEN);
        printf("共 %d 条住院记录", count);
        setColor(WHITE);
        
        // 选择查看详情
        int choice;
        if(!inputIntWithESC(&choice, "输入序号查看详情(0返回): ", x + 3, y + height - 2)) {
            break;
        }
        
        if(choice == 0) {
            break;
        } else if(choice > 0 && choice <= count) {
            findHospitalizationByIdAndShow(hospIds[choice]);
        } else {
            showMessage("输入错误！", RED);
        }
    }
}

// 按ID查找并显示
void findHospitalizationByIdAndShow(const char *hosp_id) {
    Hospitalization *hosp = findHospitalizationById(hosp_id);
    if(hosp) {
        char msg[300];
        sprintf(msg, 
            "=== 住院详细信息 ===\n"
            "住院号: %s\n"
            "患者ID: %s\n"
            "主治医生ID: %s\n"
            "入院日期: %d年%d月%d日\n"
            "床位号: %s\n"
            "住院诊断: %s\n"
            "总费用: %.2f元\n"
            "状态: %s",
            hosp->hosp_id, hosp->patient_id, hosp->doctor_id,
            hosp->admission_date.year, hosp->admission_date.month, hosp->admission_date.day,
            hosp->bed_number, hosp->diagnosis, hosp->total_cost,
            hosp->status == 1 ? "在院" : "已出院");
        showMessage(msg, CYAN);
    }
}

// 按住院号查找
Hospitalization* findHospitalizationById(const char *hosp_id) {
    Hospitalization *temp = hosp_list;
    while(temp) {
        if(strcmp(temp->hosp_id, hosp_id) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
