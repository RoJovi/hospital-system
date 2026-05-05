#include "utils.h"
#include "ui.h"
#include "data.h"
#include "patient.h"
#include "doctor.h"
#include "medicine.h"
#include "registration.h"
#include "cost.h"
#include "hospitalization.h"
#include "hashmap.h"

Patient *patient_list = NULL;
Doctor *doctor_list = NULL;
Medicine *medicine_list = NULL;
Registration *registration_list = NULL;
Cost *cost_list = NULL;
Hospitalization *hosp_list = NULL;
int currentUser = 0;
char universal_id[20] = {'\0'};

// 添加哈希表
HashMap patient_hash;
HashMap doctor_hash;
HashMap medicine_hash;
HashMap cost_hash;

// 重建所有哈希表（在数据加载后调用）
void rebuildAllHashMaps() {
    initHashMap(&patient_hash);
    initHashMap(&doctor_hash);
    initHashMap(&medicine_hash);
    initHashMap(&cost_hash);
    
    // 重建患者哈希表
    Patient *p = patient_list;
    while (p) {
        hashMapInsert(&patient_hash, p->id, p);
        p = p->next;
    }
    
    // 重建医生哈希表
    Doctor *d = doctor_list;
    while (d) {
        hashMapInsert(&doctor_hash, d->id, d);
        d = d->next;
    }
    
    // 重建药品哈希表
    Medicine *m = medicine_list;
    while (m) {
        hashMapInsert(&medicine_hash, m->id, m);
        m = m->next;
    }
    
    // 重建费用哈希表（按费用单号）
    Cost *c = cost_list;
    while (c) {
        hashMapInsert(&cost_hash, c->cost_id, c);
        c = c->next;
    }
}

// 登录窗口
void loginWindow() {
    while(1) {
        const char *items[] = {"系统管理员", "普通用户", "退出系统"};
        int choice = showMenu("医院管理系统登录", items, 3);
        
        if(choice == -1 || choice == 2) {
            currentUser = 0;
            return;
        }
        
        if(choice==1){
        	currentUser = 2;
		    return;
        	
		} 
		
        system("cls");
        
        int width = 40;
        int height = 8;
        int screenWidth = 80;
        int screenHeight = 25;
        int x = (screenWidth - width) / 2;
        int y = (screenHeight - height) / 2;
        
        //考虑加入医生系统 
        drawBox(x, y, x + width, y + height, CYAN, "请输入密码");
        
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        pos.X = x + 20; pos.Y = y + height + 1;
    	SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
    	printf("长按tab键显示密码");
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        printf("密码: ");
        
        char password[50];
        int i = 0;
        char ch;
        
        // 密码输入 Tab键按住显示
		int j;
		int tabPressed = 0;

		while(1) {
    		// 检测Tab键状态
    		if(GetAsyncKeyState(VK_TAB) & 0x8000) {  // Tab键被按住
        		if(!tabPressed) {  // 刚刚按下
    		        tabPressed = 1;
    		        // 重新显示实际字符
		            pos.X = x + 9; pos.Y = y + 2;
		        	SetConsoleCursorPosition(a, pos);
		            for( j = 0; j < i; j++) {
		                putchar(password[j]);
		            }
		        }
		    } else {  // Tab键没被按住
		        if(tabPressed) {  // 刚刚松开
		            tabPressed = 0;
		            // 恢复显示星号
		            pos.X = x + 9; pos.Y = y + 2;
		        	SetConsoleCursorPosition(a, pos);
		            for( j = 0; j < i; j++) {
		                putchar('*');
		            }
		        }
		    }
		    
		    // 检查普通按键
		    if(_kbhit()) {
		        ch = _getch();
		        
		        if(ch == 13) break;  // Enter
		        if(ch == 27) return; // ESC
		        if(ch == 8 && i > 0) {
		            printf("\b \b");
		            i--;
		        }
		        else if(i < 49 && ch >= 32 && ch <= 126) {
		            password[i++] = ch;
		            
		            // 根据Tab状态显示
		            if(tabPressed) {
		                putchar(ch);  // 按住Tab，显示实际字符
		            } else {
		                putchar('*');  // 没按Tab，显示星号
		            }
		        }
		    }
		}
		        password[i] = '\0';
		        
		        const char *correctPass = choice ?  USER_PASSWORD : ADMIN_PASSWORD;
		        
		        if(strcmp(password, correctPass) == 0) {
		            currentUser = choice ? 2 : 1;
		            showMessage(choice ?   "用户登录成功！" : "管理员登录成功！", GREEN);
		            return;
		        } else {
		            showMessage("密码错误！", RED);
		        }
		    }
		}


// 管理员菜单
void adminMenu() {
    while(currentUser == 1) {
        const char *items[] = {
            "患者信息管理",
            "医生信息管理", 
            "药品信息管理",
            "挂号管理",
            "住院管理",  
            "费用管理",
            "数据保存",
            "数据读取",
            "退出登录"
        };
        
        int choice = showMenu("系统管理员菜单", items, 9);
        
        switch(choice) {
            case 0: patientManagement(); break;
            case 1: doctorManagement(); break;
            case 2: medicineManagement(); break;
            case 3: registrationManagement(); break;
            case 4: hospitalizationManagement(); break;
            case 5: costManagement(); break;
            case 6: saveAllData(); break;
            case 7: patient_list = loadPatients();
					doctor_list = loadDoctors();
					medicine_list = loadMedicines();
					loadRegistrations();
					loadCosts();
					break;
            case 8: currentUser = 0; break;
            case -1: currentUser = 0; break;
        }
    }
}

// 用户菜单
void userMenu() {
    // 局部变量存储登录状态
    int patient_logged_in = 0;
    char current_patient_id[20] = "";
    char current_patient_phone[20] = "";
    
    while(currentUser == 2) {
        const char *items[] = {
            "查询药品信息",
            "查询医生信息", 
            "我要挂号", 
            "挂号查询",
            "费用查询",
            "智能药品推荐", 
            (patient_logged_in ? "我的账号": "登录" ),
            "返回登录界面"
        };
        
        int choice = showMenu("普通用户菜单", items, 7);
        
        switch(choice) {
            case 0: 
                showAllMedicines(); 
                break;
                
            case 1: 
                showAllDoctors(0); 
                break;
                
            case 2:  // 我要挂号
                if(!patient_logged_in) {
                    showMessage("请先登录！", RED);
                    if(!patientLogin(current_patient_id, current_patient_phone)) {
                        break;  // 登录失败
                    }
                    patient_logged_in = 1;
                }
				addRegistrationForPatient(current_patient_id, current_patient_phone);
				saveRegistrations();
    			savePatients(patient_list);
                break;
                
            case 3:  // 挂号查询
                if(!patient_logged_in) {
                    showMessage("请先登录！", RED);
                    if(!patientLogin(current_patient_id, current_patient_phone)) {
                        break;
                    }
                    patient_logged_in = 1;
                }
                findRegistrationByPatient(current_patient_id);
                break;
                
            case 4:  // 费用查询
                if(!patient_logged_in) {
                    showMessage("请先登录！", RED);
                    if(!patientLogin(current_patient_id, current_patient_phone)) {
                        break;
                    }
                    patient_logged_in = 1;
                }
                findCostByPatient(current_patient_id);
                break;
            
            case 5:  // 智能药品推荐 
                if(!patient_logged_in) {
                    showMessage("请先登录！", RED);
                    if(!patientLogin(current_patient_id, current_patient_phone)) {
                        break;
                    }
                    patient_logged_in = 1;
                }
                smartConsultation();
                break;
			    
            case 6:  // 登录/修改个人信息 
                if(patient_logged_in) {
                	//菜单 
                    
					findPatient(current_patient_id);
                } 
				else {
                    if(patientLogin(current_patient_id, current_patient_phone)) {
                       patient_logged_in = 1;
                    }
                }
                break;
                
            case 7:  // 返回登录界面
                currentUser = 0;
                return;
                
            case -1: 
                currentUser = 0;
                return;
        }
    }
}

// 主菜单
void mainMenu() {
    while(currentUser != 0) {
        if(currentUser == 1) {
            adminMenu();
        } else if(currentUser == 2) {
            userMenu();
        }
    }
}


int main() {
    SetConsoleTitle("医院管理系统");
    
    // 初始化哈希表
    initHashMap(&patient_hash);
    initHashMap(&doctor_hash);
    initHashMap(&medicine_hash);
    initHashMap(&cost_hash);
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(a, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(a, &cursorInfo);
    
    // 加载数据
    patient_list = loadPatients();
	doctor_list = loadDoctors();
	medicine_list = loadMedicines();
	loadRegistrations(); 
	loadCosts();
	loadHospitalizations();
	
	// 重建哈希表
	rebuildAllHashMaps();
	    
    // 显示欢迎信息
    showMessage("欢迎使用医院管理系统", CYAN);
    
    // 主循环
    while(1) {
        loginWindow();
        if(currentUser == 0) {
            if(confirmBox("确定要退出系统吗？")) {
                saveAllData();
                showMessage("感谢使用医院管理系统！", GREEN);
                break;
            }
        } else {
            mainMenu();
        }
    }
    
    // 清理哈希表
    hashMapClear(&patient_hash);
    hashMapClear(&doctor_hash);
    hashMapClear(&medicine_hash);
    hashMapClear(&cost_hash);
    
    // 显示光标
    //力竭了 
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(a, &cursorInfo);
    
    return 0;
}
