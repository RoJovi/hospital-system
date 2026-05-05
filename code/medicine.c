#include "medicine.h"
#include "ui.h"
#include "data.h"
#include "hashmap.h"

extern HashMap medicine_hash;
extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];

// 药品管理
void medicineManagement() {
    while(1) {
    	
    	saveMedicines(medicine_list);
    	
        const char *items[] = {
            "添加药品信息", 
            "修改药品信息",
            "删除药品信息",
            "查询药品信息",
            "显示所有药品",
            "返回上级菜单"
        };
        
        int choice = showMenu("药品信息管理", items, 6);
        
        switch(choice) {
			case 0: addMedicine(); break;
            case 1: modMedicine(); break;
            case 2: delMedicine(); break;
            case 3: findMedicine(); break;
            case 4: showAllMedicines(); break;
            case 5: return;
            case -1: return;
        }
    }
}

// 添加药品
void addMedicine() {
    system("cls");
    
    int width = 60;
    int height = 18;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "添加药品信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    Medicine newMedicine;
    
    // 输入药品ID
    if(!inputStringWithESC(newMedicine.id, MAX_ID, "药品ID: ", x + 3, y + 2)) {
        return;
    }
    
    // 检查ID是否重复
    if(findMedicineById(newMedicine.id) != NULL) {
        showMessage("该药品ID已存在！", RED);
        return;
    }
    
    if(!inputStringWithESC(newMedicine.name, MAX_MEDICINE, "药品名称: ", x + 3, y + 3)) {
        return;
    }
    
    if(!inputStringWithESC(newMedicine.specification, 50, "药品规格: ", x + 3, y + 4)) {
        return;
    }
    
    if(!inputStringWithESC(newMedicine.manufacturer, 50, "生产厂家: ", x + 3, y + 5)) {
        return;
    }
    
    if(!inputDoubleWithESC(&newMedicine.price, "药品单价: ", x + 3, y + 6)) {
        return;
    }
    
    if(!inputIntWithESC(&newMedicine.stock, "库存数量: ", x + 3, y + 7)) {
        return;
    }
    
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    printf("生产日期(年 月 日): ");
    
    // 分别输入年月日
    if(!inputIntWithESC(&newMedicine.production_date.year, "", x + 24, y + 8)) {
        return;
    }
    
    if(!inputIntWithESC(&newMedicine.production_date.month, "", x + 29, y + 8)) {
        return;
    }
    
    if(!inputIntWithESC(&newMedicine.production_date.day, "", x + 33, y + 8)) {
        return;
    }
    
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    printf("有效期至(年 月 日): ");
    
    // 分别输入年月日
    if(!inputIntWithESC(&newMedicine.expiry_date.year, "", x + 24, y + 9)) {
        return;
    }
    
    if(!inputIntWithESC(&newMedicine.expiry_date.month, "", x + 29, y + 9)) {
        return;
    }
    
    if(!inputIntWithESC(&newMedicine.expiry_date.day, "", x + 33, y + 9)) {
        return;
    }
    
    pos.X = x + 3; pos.Y = y + 12;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("请确认药品信息：");
    pos.X = x + 3; pos.Y = y + 13;
    printf("确定(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') {
	        // 添加到链表
	        Medicine *newNode = (Medicine*)malloc(sizeof(Medicine));
	        memcpy(newNode, &newMedicine, sizeof(Medicine));
	        newNode->next = medicine_list;
	        medicine_list = newNode;
	        
            hashMapInsert(&medicine_hash, newNode->id, newNode);
	        
	        showMessage("药品信息添加成功！", GREEN);
	        return;
		}
        if(ch == 'n' || ch == 'N') {
        	showMessage("已取消添加药品", YELLOW);
        	return;
    	}
    }
}


// 显示单个药品的完整信息
void showSingleMedicineDetail(const char *id) {
    Medicine *p = findMedicineById(id);
    system("cls");
    if(p == NULL) {
        showMessage("药品不存在", RED);
        return;
    }
    
    int width = 60;
    int height = 18;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "药品详细信息");
    COORD pos;//坐标 
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("  药品编号: %s\n", p->id);
    pos.X = x + 3; pos.Y = y + 3;
    SetConsoleCursorPosition(a, pos);
    printf("  药品名称: %s\n", p->name);
    pos.X = x + 3; pos.Y = y + 4;
    SetConsoleCursorPosition(a, pos);
    printf("  药品规格: %s\n", p->specification);
    pos.X = x + 3; pos.Y = y + 5;
    SetConsoleCursorPosition(a, pos);
    printf("  生产厂家: %s\n", p->manufacturer);
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    printf("  药品单价: %.2f元\n", p->price);
    pos.X = x + 3; pos.Y = y + 7;
    SetConsoleCursorPosition(a, pos);
    printf("  库存数量: %d\n", p->stock);
    pos.X = x + 3; pos.Y = y + 8;
    SetConsoleCursorPosition(a, pos);
    printf("  生产日期: %d年%d月%d日\n", 
           p->production_date.year,
           p->production_date.month,
           p->production_date.day);
    pos.X = x + 3; pos.Y = y + 9;
    SetConsoleCursorPosition(a, pos);
    printf("  有效期至: %d年%d月%d日\n",
           p->expiry_date.year,
           p->expiry_date.month,
           p->expiry_date.day);
    pos.X = x + 3; pos.Y = y + 11;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}


// 显示所有药品
void showAllMedicines() {

    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    while(1){
		system("cls");
	    drawBox(x, y, x + width, y + height, CYAN, "所有药品信息");
	
	    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
	    COORD pos;
	    
	    if(!medicine_list) {
	        pos.X = x + 3; pos.Y = y + 2;
	        SetConsoleCursorPosition(a, pos);
	        setColor(YELLOW);
	        printf("暂无药品信息！");
	        setColor(WHITE);
	        pressAnyKey();
	        return;
	    }
	    
	    //逐一显示信息 
	    pos.X = x + 3; pos.Y = y + 2;
	    SetConsoleCursorPosition(a, pos);
	    setColor(YELLOW);
	    printf("%-5s %-10s %-15s %-10s %-8s %-6s", "序号","药品ID", "药品名称", "规格", "单价", "库存");
	    setColor(WHITE);
	    
	    Medicine *temp = medicine_list;
	    int count = 0;
	    int currentY = y + 3;
	    char *medicineIds[100];
	    
	    while(temp && currentY < y + height - 2) {
	    	medicineIds[++count] = temp->id;
	        pos.X = x + 3; pos.Y = currentY++;
	        SetConsoleCursorPosition(a, pos);
	        printf("%-5d %-10s %-15s %-10s %-8.2f %-6d",
	               count, temp->id, temp->name, temp->specification, temp->price, temp->stock);
			temp = temp->next;

	    }
	    
	    pos.X = x + 3; pos.Y = y + height - 3;
	    SetConsoleCursorPosition(a, pos);
	    setColor(YELLOW);
	    printf("共 %d 种药品", count);

		//点菜 

	    int choice;
		if(!inputIntWithESC(&choice, "输入序号可查看详细信息（输入 0 库存由低到高排序）：", x+3, y + height - 2))break;
		setColor(WHITE);
		if(choice==0) sortMedicinesByStock();
	    else if(choice >  0 && choice <= count) {
	        if(!findMedicineById(medicineIds[choice])) {
	        	showMessage("暂无此药品", RED);
			}
			else showSingleMedicineDetail(medicineIds[choice]);
	    }
	    else {
	    	showMessage("输入错误！请重试", RED);
		}
	}
	return; 
}

// 药品按库存升序排序（库存少的排前面）
void sortMedicinesByStock() {
    if(!medicine_list || !medicine_list->next) {
        showMessage("药品数量不足，无需排序", YELLOW);
        return;
    }
    
    int swapped;
    Medicine *ptr1;
    Medicine *lptr = NULL;
    
    // 使用冒泡排序
    do {
        swapped = 0;
        ptr1 = medicine_list;
        
        while(ptr1->next != lptr) {
            // 比较库存，库存高的往后排
            if(ptr1->stock > ptr1->next->stock) {
                // 交换两个节点数据
                swapMedicineData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while(swapped);
    
    showMessage("药品按库存升序排序完成！", GREEN);
}

// 交换两个药品节点数据的辅助函数
void swapMedicineData(Medicine *a, Medicine *b) {
    // 创建临时变量保存a的数据
    Medicine temp = *a;
    
    // 复制b的数据到a
    strcpy(a->id, b->id);
    strcpy(a->name, b->name);
    strcpy(a->specification, b->specification);
    strcpy(a->manufacturer, b->manufacturer);
    a->price = b->price;
    a->stock = b->stock;
    a->production_date = b->production_date;
    a->expiry_date = b->expiry_date;
    
    // 复制temp的数据到b
    strcpy(b->id, temp.id);
    strcpy(b->name, temp.name);
    strcpy(b->specification, temp.specification);
    strcpy(b->manufacturer, temp.manufacturer);
    b->price = temp.price;
    b->stock = temp.stock;
    b->production_date = temp.production_date;
    b->expiry_date = temp.expiry_date;
}
//查找药品信息
void findMedicine(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "查找药品信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    char id[20];
    
     // 输入药品ID
    if(!inputStringWithESC(id, MAX_ID, "药品ID: ", x + 3, y + 2)) {
        return;
    }
    
	Medicine *p1=findMedicineById(id);
	
	if(p1!=NULL){
		showSingleMedicineDetail(id);
		return;
    } 
    showMessage("暂无该药品信息", RED);
	return;
} 

//删除药品 
void delMedicine(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "删除药品信息");
    
    char id[20];
    
     // 输入药品ID
     if(!inputStringWithESC(id, MAX_ID, "药品ID: ", x + 3, y + 2)) {
        return;
    }
    
	Medicine *p1,*p2;
	if(medicine_list==NULL){
		showMessage("暂无该药品信息", RED);
		return;
	}
	p1=medicine_list;
	if(strcmp(medicine_list->id,id)==0){
		
		hashMapRemove(&medicine_hash, medicine_list->id);
		
        medicine_list=medicine_list->next;
        free(p1);
        showMessage("已删除该药品信息", YELLOW);
        return;
    }
    while(strcmp(p1->id,id)!=0 &&p1->next!=NULL) {
        p2=p1;
        p1=p1->next;
    }
    if(strcmp(p1->id,id)==0){
    	
    	hashMapRemove(&medicine_hash, p1->id);
    	
        p2->next=p1->next;
        free(p1);
        showMessage("已删除该药品信息", YELLOW);
        return;
    } 
    showMessage("暂无该药品信息", RED);
	return;
}

//修改药品信息 
void modMedicine(){
	system("cls");
    
    int width = 60;
    int height = 10;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "修改药品信息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    char id[20];
    
     // 输入药品ID
     if(!inputStringWithESC(id, MAX_ID, "药品ID: ", x + 3, y + 2)) {
        return;
    }
    
	Medicine *p1=findMedicineById(id);
	
	if(p1!=NULL){
		while(1) {
        const char *items[] = {
            "药品名称",
            "药品规格",
            "生产厂家", 
            "药品单价",
            "库存数量",
            "生产日期",
            "有效期至",
            "返回"
        };
        
        int choice = showMenu("选择修改的项", items, 8);
        
        switch(choice) {
            case 0: 
	            system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "药品名称");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原名称: %s", p1->name);
				if(!inputStringWithESC(p1->name, MAX_MEDICINE, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 1: 
	            system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "药品规格");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原规格: %s", p1->specification);
				if(!inputStringWithESC(p1->specification, 50, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
	    		break;
            case 2: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "生产厂家");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原厂家: %s", p1->manufacturer);
				if(!inputStringWithESC(p1->manufacturer, 50, "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 3: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "药品单价");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原单价: %.2f", p1->price);
				if(!inputDoubleWithESC(&(p1->price), "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 4: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "库存数量");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原库存: %d", p1->stock);
				if(!inputIntWithESC(&(p1->stock), "更改为: ", x + 3, y + 7)) {
	        		continue;
	        	}
				break;
            case 5: 
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "生产日期");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原日期: %d 年 %d 月 %d日", 
				p1->production_date.year, p1->production_date.month, p1->production_date.day);
                 pos.X = x + 3; pos.Y = y + 7;
    			SetConsoleCursorPosition(a, pos);
    			printf("更改为(年 月 日): ");
            	if(!inputIntWithESC(&p1->production_date.year, "", x + 20, y + 7)) {
			        continue;
			    }
			    
			    if(!inputIntWithESC(&p1->production_date.month, "", x + 25, y + 7)) {
			        continue;
			    }
			    
			    if(!inputIntWithESC(&p1->production_date.day, "", x + 28, y + 7)) {
			        continue;
			    }
			    break;
            case 6:
            	system("cls"); 
	            drawBox(x, y, x + width, y + height, CYAN, "有效期至");
	            pos.X = x + 3; pos.Y = y + 3;
                SetConsoleCursorPosition(a, pos);
                printf("原有效期: %d 年 %d 月 %d日", 
                p1->expiry_date.year, p1->expiry_date.month, p1->expiry_date.day);
                 pos.X = x + 3; pos.Y = y + 7;
                SetConsoleCursorPosition(a, pos);
                printf("更改为(年 月 日): ");
                if(!inputIntWithESC(&p1->expiry_date.year, "", x + 20, y + 7)) {
                    continue;
                }
                
                if(!inputIntWithESC(&p1->expiry_date.month, "", x + 25, y + 7)) {
                    continue;
                }
                
                if(!inputIntWithESC(&p1->expiry_date.day, "", x + 28, y + 7)) {
                    continue;
                }
				break;
            case 7: 
            case -1: 
				return;
        }
    }
    } 
    showMessage("暂无该药品信息", RED);
	return;
}

// 查找药品
Medicine* findMedicineById(const char *id) {
    return (Medicine*)hashMapGet(&medicine_hash, id);
}

