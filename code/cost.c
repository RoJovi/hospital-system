#include "cost.h"
#include "ui.h"
#include "data.h"
#include "patient.h"
#include "hashmap.h"

extern HashMap cost_hash;
extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];

// 查找费用记录
Cost* findCostById(const char *cost_id) {
    return (Cost*)hashMapGet(&cost_hash, cost_id);
}

// 查找患者的费用记录
Cost* findCostByPatientId(const char *patient_id) {
    Cost *temp = cost_list;
    while(temp) {
        if(strcmp(temp->patient_id, patient_id) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 添加费用记录
void addCost() {
    system("cls");
    
    int width = 60;
    int height = 16;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "添加费用记录");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Cost newCost;
    
    // 生成费用单号 (F + 年月日 + 序号)
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    static int counter = 1;
    sprintf(newCost.cost_id, "F%04d%02d%02d%03d", 
             tm_info->tm_year + 1900,tm_info->tm_mon + 1, tm_info->tm_mday, counter++); 
    // 显示生成的费用单号
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("费用单号: %s", newCost.cost_id);
    
    // 输入患者ID
    if(!inputStringWithESC(newCost.patient_id, MAX_ID, "患者ID: ", x + 3, y + 3)) {
        return;
    }
    
    // 输入费用日期
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("费用日期(年 月 日): ");
    
    if(!inputIntWithESC(&newCost.cost_date.year, "", x + 22, y + 4)) {
        return;
    }
    
    if(!inputIntWithESC(&newCost.cost_date.month, "", x + 27, y + 4)) {
        return;
    }
    
    if(!inputIntWithESC(&newCost.cost_date.day, "", x + 31, y + 4)) {
        return;
    }
    
    // 输入费用项目
    if(!inputStringWithESC(newCost.item, 50, "费用项目: ", x + 3, y + 5)) {
        return;
    }
    
    // 输入费用金额
    if(!inputDoubleWithESC(&newCost.amount, "费用金额: ", x + 3, y + 6)) {
        return;
    }
    
    // 选择支付状态
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("支付状态: ");
    
    const char *statusOptions[] = {"未支付", "已支付", "部分支付"};
    pos.X = x + 13; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("1.未支付 2.已支付 3.部分支付");
    
    int statusChoice;
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    if(!inputIntWithESC(&statusChoice, "选择(1-3): ", x + 13, y + 8)) {
        return;
    }
    
    switch(statusChoice) {
        case 1: newCost.payment_status = 0; break;  // 未支付
        case 2: newCost.payment_status = 1; break;  // 已支付
        case 3: newCost.payment_status = 2; break;  // 部分支付
        default: newCost.payment_status = 0;
    }
    
    // 显示汇总信息
    pos.X = x + 3; pos.Y = y + 10;
    SetConsoleCursorPosition(a, pos);
    setColor(GREEN);
    printf("===== 费用信息汇总 =====");
    pos.X = x + 3; pos.Y = y + 11;
    SetConsoleCursorPosition(a, pos);
    printf("费用单号: %s", newCost.cost_id);
    pos.X = x + 3; pos.Y = y + 12;
    SetConsoleCursorPosition(a, pos);
    printf("患者ID: %s", newCost.patient_id);
    pos.X = x + 3; pos.Y = y + 13;
    SetConsoleCursorPosition(a, pos);
    printf("费用项目: %s", newCost.item);
    pos.X = x + 3; pos.Y = y + 14;
    SetConsoleCursorPosition(a, pos);
    printf("费用金额: %.2f元", newCost.amount);
    pos.X = x + 3; pos.Y = y + 15;
    SetConsoleCursorPosition(a, pos);
    printf("支付状态: %s", 
           newCost.payment_status == 0 ? "未支付" : 
           newCost.payment_status == 1 ? "已支付" : "部分支付");
    
    pos.X = x + 3; pos.Y = y + height - 3;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("确认添加(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
            // 添加到链表
            Cost *newNode = (Cost*)malloc(sizeof(Cost));
            memcpy(newNode, &newCost, sizeof(Cost));
            newNode->next = cost_list;
            cost_list = newNode;
            
            hashMapInsert(&cost_hash, newNode->cost_id, newNode);
            
            updatePatientTotalCost(newCost.patient_id, newCost.amount);
            showMessage("费用记录添加成功！", GREEN);
            return;
        }
        if(ch == 'n' || ch == 'N') {
            showMessage("已取消添加", YELLOW);
            return;
        }
    }
}

// 患者费用更新
void updatePatientTotalCost(const char *patient_id, double amount_change) {
    if(patient_id == NULL || patient_id[0] == '\0') return;
    
    Patient *patient = findPatientById(patient_id);
    if(patient == NULL) {
        return;
    }
    
    patient->total_cost += amount_change;
    
    if(patient->total_cost < 0) {
        patient->total_cost = 0.0;
    }
}

// 自动添加费用函数
void autoAddCost(const char *patient_id, const char *item_name, double amount) {
    if(patient_id == NULL || patient_id[0] == '\0' || 
       item_name == NULL || item_name[0] == '\0' || 
       amount <= 0) {
        return;
    }
    
    // 2. 创建费用记录
    Cost newCost;
    
    // 生成费用单号
    static int cost_counter = 1;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    
    sprintf(newCost.cost_id, "F%04d%02d%02d%04d", 
            tm_info->tm_year + 1900, 
            tm_info->tm_mon + 1, 
            tm_info->tm_mday, 
            cost_counter++);
    
    strcpy(newCost.patient_id, patient_id);
    strcpy(newCost.item, item_name);
    newCost.amount = amount;
    
    newCost.cost_date.year = tm_info->tm_year + 1900;
    newCost.cost_date.month = tm_info->tm_mon + 1;
    newCost.cost_date.day = tm_info->tm_mday;
    
    newCost.payment_status = 0;

    Cost *newNode = (Cost*)malloc(sizeof(Cost));
    if(newNode == NULL) {
        return;
    }
    
    memcpy(newNode, &newCost, sizeof(Cost));
    newNode->next = cost_list;
    cost_list = newNode;
    
    hashMapInsert(&cost_hash, newNode->cost_id, newNode);
    
    updatePatientTotalCost(patient_id, amount);
}

// 显示单个费用记录的详细信息
void showSingleCostDetail(const char *cost_id) {
    Cost *p = findCostById(cost_id);
    system("cls");
    if(p == NULL) {
        showMessage("费用记录不存在", RED);
        return;
    }
    
    int width = 60;
    int height = 14;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "费用详细信息");
    
    COORD pos;
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("  费用单号: %s", p->cost_id);
    
    pos.X = x + 3; pos.Y = y + 3;
    SetConsoleCursorPosition(a, pos);
    printf("  患者ID: %s", p->patient_id);
    
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("  费用日期: %d年%d月%d日", 
           p->cost_date.year, p->cost_date.month, p->cost_date.day);
    
    pos.X = x + 3; pos.Y = y + 5;
    SetConsoleCursorPosition(a, pos);
    printf("  费用项目: %s", p->item);
    
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("  费用金额: %.2f元", p->amount);
    
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("  支付状态: ");
    switch(p->payment_status) {
        case 0: printf("未支付"); break;
        case 1: printf("已支付"); break;
        case 2: printf("部分支付"); break;
        default: printf("未知");
    }
    
    pos.X = x + 3; pos.Y = y + 10;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}

// 显示所有费用记录
void showAllCosts() {
    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1) {
        system("cls");
        drawBox(x, y, x + width, y + height, CYAN, "所有费用记录");
        
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        
        if(!cost_list) {
            pos.X = x + 3; pos.Y = y + 2;
            SetConsoleCursorPosition(a, pos);
            setColor(YELLOW);
            printf("暂无费用记录！");
            setColor(WHITE);
            pressAnyKey();
            return;
        }
        
        // 显示表头
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("%-5s %-15s %-12s %-15s %-10s %-10s", 
               "序号", "费用单号", "患者ID", "费用项目", "金额", "状态");
        setColor(WHITE);
        
        Cost *temp = cost_list;
        int count = 0;
        int currentY = y + 3;
        char *costIds[100];
        
        while(temp && currentY < y + height - 3) {
            costIds[++count] = temp->cost_id;
            
            pos.X = x + 3; pos.Y = currentY++;
            SetConsoleCursorPosition(a, pos);
            
            printf("%-5d %-15s %-12s %-15s %-10.2f ", 
                   count, temp->cost_id, temp->patient_id, 
                   temp->item, temp->amount);
            
            // 显示状态
            switch(temp->payment_status) {
                case 0: printf("未支付"); break;
                case 1: printf("已支付"); break;
                case 2: printf("部分支付"); break;
                default: printf("未知");
            }
            
            temp = temp->next;
        }
        
        pos.X = x + 3; pos.Y = y + height - 3;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("共 %d 条费用记录", count);
        
        // 查看详细
        int choice;
        if(!inputIntWithESC(&choice, "输入序号查看详情(0返回): ", x + 3, y + height - 2)) {
            break;
        }
        setColor(WHITE);
        
        if(choice == 0) {
            break;
        } else if(choice > 0 && choice <= count) {
            showSingleCostDetail(costIds[choice]);
        } else {
            showMessage("输入错误！请重试", RED);
        }
    }
    return;
}

// 修改费用信息
void modifyCost() {
    system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "修改费用信息");
    
    char cost_id[20];
    
    // 输入费用单号
    if(!inputStringWithESC(cost_id, MAX_ID, "费用单号: ", x + 3, y + 2)) {
        return;
    }
    
    Cost *p = findCostById(cost_id);
    
    if(p == NULL) {
        showMessage("未找到该费用记录！", RED);
        return;
    }
    
    // 显示修改菜单
    while(1) {
        system("cls");
        drawBox(x, y, x + width, y + height, CYAN, "选择修改项目");
        
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        
        // 显示当前信息
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        printf("费用单号: %s", p->cost_id);
        
        pos.X = x + 3; pos.Y = y + 3;
        SetConsoleCursorPosition(a, pos);
        printf("当前信息:");
        
        pos.X = x + 3; pos.Y = y + 4;
        SetConsoleCursorPosition(a, pos);
        printf("患者ID: %s", p->patient_id);
        
        pos.X = x + 3; pos.Y = y + 5;
        SetConsoleCursorPosition(a, pos);
        printf("费用项目: %s", p->item);
        
        pos.X = x + 3; pos.Y = y + 6;
        SetConsoleCursorPosition(a, pos);
        printf("费用金额: %.2f", p->amount);
        
        // 修改选项
        pos.X = x + 3; pos.Y = y + 8;
        SetConsoleCursorPosition(a, pos);
        printf("选择修改项目:");
        
        pos.X = x + 3; pos.Y = y + 9;
        SetConsoleCursorPosition(a, pos);
        printf("1. 修改费用项目");
        
        pos.X = x + 3; pos.Y = y + 10;
        SetConsoleCursorPosition(a, pos);
        printf("2. 修改费用金额");
        
        pos.X = x + 3; pos.Y = y + 11;
        SetConsoleCursorPosition(a, pos);
        printf("3. 修改支付状态");
        
        pos.X = x + 3; pos.Y = y + 12;
        SetConsoleCursorPosition(a, pos);
        printf("0. 返回");
        
        int choice;
        pos.X = x + 3; pos.Y = y + 14;
        SetConsoleCursorPosition(a, pos);
        if(!inputIntWithESC(&choice, "请选择(0-3): ", x + 20, y + 14)) {
            continue;
        }
        
        switch(choice) {
            case 0:
                return;
                
            case 1:  // 修改费用项目
                system("cls");
                drawBox(x, y, x + width, y + height, CYAN, "修改费用项目");
                pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原费用项目: %s", p->item);
                if(!inputStringWithESC(p->item, 50, "新费用项目: ", x + 3, y + 5)) {
                    break;
                }
                showMessage("费用项目修改成功！", GREEN);
                break;
                
            case 2:  // 修改费用金额
                system("cls");
                drawBox(x, y, x + width, y + height, CYAN, "修改费用金额");
                pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原费用金额: %.2f", p->amount);
                double old_amount = p->amount;
                if(!inputDoubleWithESC(&p->amount, "新费用金额: ", x + 3, y + 5)) {
                    break;
                }
                double difference = p->amount - old_amount;
    			updatePatientTotalCost(p->patient_id, difference);
                showMessage("费用金额修改成功！", GREEN);
                break;
                
            case 3:  // 修改支付状态
                system("cls");
                drawBox(x, y, x + width, y + height, CYAN, "修改支付状态");
                pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原支付状态: ");
                switch(p->payment_status) {
                    case 0: printf("未支付"); break;
                    case 1: printf("已支付"); break;
                    case 2: printf("部分支付"); break;
                    default: printf("未知");
                }
                
                pos.X = x + 3; pos.Y = y + 5;
                SetConsoleCursorPosition(a, pos);
                printf("新支付状态:");
                
                pos.X = x + 3; pos.Y = y + 6;
                SetConsoleCursorPosition(a, pos);
                printf("1. 未支付");
                
                pos.X = x + 3; pos.Y = y + 7;
                SetConsoleCursorPosition(a, pos);
                printf("2. 已支付");
                
                pos.X = x + 3; pos.Y = y + 8;
                SetConsoleCursorPosition(a, pos);
                printf("3. 部分支付");
                
                int statusChoice;
                pos.X = x + 3; pos.Y = y + 10;
                SetConsoleCursorPosition(a, pos);
                if(!inputIntWithESC(&statusChoice, "选择(1-3): ", x + 15, y + 10)) {
                    break;
                }
                
                switch(statusChoice) {
                    case 1: p->payment_status = 0; break;
                    case 2: p->payment_status = 1; break;
                    case 3: p->payment_status = 2; break;
                    default: break;
                }
                
                showMessage("支付状态修改成功！", GREEN);
                break;
                
            default:
                showMessage("无效选择！", RED);
        }
    }
}

// 删除费用记录
void deleteCost() {
    system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "删除费用记录");
    
    char cost_id[20];
    
    // 输入费用单号
    if(!inputStringWithESC(cost_id, MAX_ID, "费用单号: ", x + 3, y + 2)) {
        return;
    }
    
    Cost *p1, *p2;
    
    if(cost_list == NULL) {
        showMessage("暂无费用记录！", RED);
        return;
    }
    
    // 删除头节点
    if(strcmp(cost_list->cost_id, cost_id) == 0) {
        p1 = cost_list;
        
        hashMapRemove(&cost_hash, p1->cost_id);
        
        updatePatientTotalCost(p1->patient_id, -p1->amount);
        cost_list = cost_list->next;
        free(p1);
        showMessage("费用记录删除成功！", GREEN);
        return;
    }
    
    // 查找并删除中间或尾部节点
    p1 = cost_list;
    while(p1 != NULL && strcmp(p1->cost_id, cost_id) != 0) {
        p2 = p1;
        p1 = p1->next;
    }
    
    if(p1 == NULL) {
        showMessage("未找到该费用记录！", RED);
        return;
    }
    
    hashMapRemove(&cost_hash, p1->cost_id);
    
    updatePatientTotalCost(p1->patient_id, -p1->amount);
    p2->next = p1->next;
    free(p1);
    showMessage("费用记录删除成功！", GREEN);
}


// 按患者查询费用记录
void findCostByPatient(const char *id) {
    system("cls");
    
    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "按患者查询费用");
    
    char patient_id[20];
    
    // 输入患者ID
    if(id[0]=='\0'){
	    if(!inputStringWithESC(patient_id, MAX_ID, "患者ID: ", x + 3, y + 2)) {
	        return;
	    }
	}
    else strcpy(patient_id,id);
    
    system("cls");
    drawBox(x, y, x + width, y + height, CYAN, "患者费用记录");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Cost *temp = cost_list;
    int count = 0;
    double totalAmount = 0.0;
    double paidAmount = 0.0;
    
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("患者ID: %s", patient_id);
    
    // 显示表头
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("%-5s %-15s %-15s %-10s %-10s", 
           "序号", "费用单号", "费用项目", "金额", "状态");
    setColor(WHITE);
    
    int currentY = y + 5;
    
    while(temp) {
        if(strcmp(temp->patient_id, patient_id) == 0) {
            count++;
            totalAmount += temp->amount;
            if(temp->payment_status == 1) {
                paidAmount += temp->amount;
            } else if(temp->payment_status == 2) {
                paidAmount += temp->amount * 0.5;  // 假设部分支付为50%
            }
            
            pos.X = x + 3; pos.Y = currentY++;
            SetConsoleCursorPosition(a, pos);
            
            printf("%-5d %-15s %-15s %-10.2f ", 
                   count, temp->cost_id, temp->item, temp->amount);
            
            // 显示状态
            switch(temp->payment_status) {
                case 0: printf("未支付"); break;
                case 1: printf("已支付"); break;
                case 2: printf("部分支付"); break;
                default: printf("未知");
            }
        }
        temp = temp->next;
    }
    
    if(count == 0) {
        pos.X = x + 3; pos.Y = currentY;
        SetConsoleCursorPosition(a, pos);
        setColor(RED);
        printf("未找到该患者的费用记录");
        setColor(WHITE);
    } else {
        // 显示统计信息
        pos.X = x + 3; pos.Y = currentY + 2;
        SetConsoleCursorPosition(a, pos);
        setColor(GREEN);
        printf("===== 费用统计 =====");
        
        pos.X = x + 3; pos.Y = currentY + 3;
        SetConsoleCursorPosition(a, pos);
        printf("总费用记录: %d 条", count);
        
        pos.X = x + 3; pos.Y = currentY + 4;
        SetConsoleCursorPosition(a, pos);
        printf("总金额: %.2f 元", totalAmount);
        
        pos.X = x + 3; pos.Y = currentY + 5;
        SetConsoleCursorPosition(a, pos);
        printf("已支付: %.2f 元", paidAmount);
        
        pos.X = x + 3; pos.Y = currentY + 6;
        SetConsoleCursorPosition(a, pos);
        printf("待支付: %.2f 元", totalAmount - paidAmount);
        
        setColor(WHITE);
    }
    
    pos.X = x + 3; pos.Y = y + height - 2;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}

// 费用管理主菜单
void costManagement() {
    while(1) {
    	
    	saveCosts();
    	savePatients(patient_list);
    	
        const char *items[] = {
            "添加费用记录",
            "修改费用信息", 
            "删除费用记录",
            "显示所有费用",
            "按患者查询费用",
            "返回上级菜单"
        };
        
        int choice = showMenu("费用管理", items, 6);
        
        switch(choice) {
            case 0: addCost(); break;
            case 1: modifyCost(); break;
            case 2: deleteCost(); break;
            case 3: showAllCosts(); break;
            case 4: findCostByPatient(universal_id); break;
            case 5: return;
            case -1: return;
        }
    }
}
