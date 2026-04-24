#include "ui.h"

extern Patient *patient_list;
extern Doctor *doctor_list;
extern Medicine *medicine_list;
extern Registration *registration_list;
extern Cost *cost_list;
extern Hospitalization *hosp_list;
extern int currentUser;
extern char universal_id[20];

// 设置控制台颜色
void setColor(int color) {
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(a, color);
}

// 绘制方框
void drawBox(int x1, int y1, int x2, int y2, int color, const char* title) {
    setColor(color);
    
    COORD pos;//坐标 
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 画上边框
    pos.X = x1; pos.Y = y1;
    SetConsoleCursorPosition(a, pos);
    printf("╔");
    int i; 
    for(i = x1 + 1; i < x2; i++) {
        pos.X = i; pos.Y = y1;
        SetConsoleCursorPosition(a, pos);
        printf("═");
    }
    
    pos.X = x2; pos.Y = y1;
    SetConsoleCursorPosition(a, pos);
    printf("╗");
    
    // 画标题
    if(title != NULL) {
        int titleLen = strlen(title);
        int titleX = x1 + (x2 - x1 - titleLen) / 2;
        pos.X = titleX; pos.Y = y1;
        SetConsoleCursorPosition(a, pos);
        printf(" %s ", title);
    }
    
    // 画垂直边框
    for( i = y1 + 1; i < y2; i++) {
        pos.X = x1; pos.Y = i;
        SetConsoleCursorPosition(a, pos);
        printf("║");
        pos.X = x2; pos.Y = i;
        SetConsoleCursorPosition(a, pos);
        printf("║");
    }
    
    // 画下边框
    pos.X = x1; pos.Y = y2;
    SetConsoleCursorPosition(a, pos);
    printf("╚");
    for(i = x1 + 1; i < x2; i++) {
        pos.X = i; pos.Y = y2;
        SetConsoleCursorPosition(a, pos);
        printf("═");
    }
    pos.X = x2; pos.Y = y2;
    SetConsoleCursorPosition(a, pos);
    printf("╝");
    
    setColor(WHITE);
}

// 绘制菜单栏 
void drawMenu(int x, int y, const char* items[], int count, int highlight) {
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    int i;
    for( i = 0; i < count; i++) {
        pos.X = x; pos.Y = y + i;
        SetConsoleCursorPosition(a, pos);
        
        if(i == highlight) {
            setColor(CYAN);
            printf("→ %s", items[i]);
            setColor(WHITE);
        } else {
            setColor(WHITE);
            printf("  %s", items[i]);
        }
    }
}

// 显示完整菜单界面并获取动态选择操作 
int showMenu(const char *title, const char *items[], int count) {
    system("cls");
    
    int maxLen = strlen(title);
    int  i;
    for( i = 0; i < count; i++) {
        int len = strlen(items[i]) + 4;
        if(len > maxLen) maxLen = len;
    }
    
    int width = maxLen + 8;
    int height = count + 4;
    int screenWidth = 80;//待修改！！！！！！！！ 
    int screenHeight = 25;//待修改！！！！！！！！
    //为什么怎么改都不会移动位置 
    // 居中显示
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, title);
    
    int highlight = 0;//调整画面＋选择值 
    int choice = -1;
    
    while(1) {
        drawMenu(x + 3, y + 2, items, count, highlight);
        
        // 显示操作提示
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        pos.X = x + 3; pos.Y = y + height + 1;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("使用 ↑↓ 键选择，Enter 键确认");
        setColor(WHITE);
        
        int ch = _getch();
        if(ch == 224) { // 方向键前缀 
            ch = _getch();
            switch(ch) {
                case 72: // 上箭头
                    highlight = (highlight - 1 + count) % count;
                    break;
                case 80: // 下箭头
                    highlight = (highlight + 1) % count;
                    break;
            }
        } else if(ch == 13) { // Enter键
            choice = highlight;
            break;
        } else if(ch == 27) { // ESC键
            choice = -1;
            break;
        }
    }
    
    return choice;
}

// 显示消息提示框
void showMessage(const char *msg, int color) {
    int len = strlen(msg);
    int width = len + 8;
    int height = 5;
    int screenWidth = 80;
    int screenHeight = 25;
    
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    system("cls"); 
    drawBox(x, y, x + width, y + height, color, "消息");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    setColor(color);
    printf("%s", msg);
    setColor(WHITE);
    
    pos.X = x + 3; pos.Y = y + height - 1;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键继续...");
    setColor(WHITE);
    
    _getch();
}

// 确认框
int confirmBox(const char *msg) {
    int len = strlen(msg);
    int width = len + 8;
    int height = 6;
    int screenWidth = 80;
    int screenHeight = 25;
    
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    system("cls");
    drawBox(x, y, x + width, y + height, YELLOW, "确认");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("%s", msg);
    
    pos.X = x + 3; pos.Y = y + height - 2;
    SetConsoleCursorPosition(a, pos);
    setColor(GREEN);
    printf("确定(Y) / 取消(N)");
    setColor(WHITE);
    
    while(1) {
        int ch = _getch();
        if(ch == 'y' || ch == 'Y') return 1;
        if(ch == 'n' || ch == 'N') return 0;
    }
}
// 按任意键继续
void pressAnyKey() {
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = 0; pos.Y = 24;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键继续...");
    setColor(WHITE);
    _getch();
}

// 支持ESC的字符串输入函数
int inputStringWithESC(char *buffer, int maxLen, const char *prompt, int x, int y) {
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    int i=0;
    char temp[50];
    
    COORD pos = {x, y};
    SetConsoleCursorPosition(a, pos);
    printf("%s_\b", prompt);
    
    //完全清空键盘缓冲区
    while(_kbhit()) {
        _getch();
    }
    
    // 初始化缓冲区
    memset(temp, 0, 50);
    
    while(1) {
        int ch = _getch();
        
        // 处理功能键
        if(ch == 0 || ch == 224) {
            _getch();  // 丢弃功能键第二字节
            continue;
        }
        
        // 处理回车
        if(ch == 13) {
            strcpy(buffer,temp);
            return 1;
        }
        
        // 处理ESC
        if(ch == 27) {
            return 0;
        }
        
        // 处理退格
        if(ch == 8) {
            if(i > 0) {
                printf("\b \b");
                temp[--i] = '\0';
            }
            continue;
        }
        
        // 处理普通输入
        if(i < 49 && ch >= 32) {
            temp[i++] = (char)ch;
            putchar(ch);
            temp[i] = '\0';
        }
    }
}

// 支持ESC的整数输入
int inputIntWithESC(int *value, const char *prompt, int x, int y) {
    char buffer[20];
    if(!inputStringWithESC(buffer, 20, prompt, x, y)) {
        return 0;  // ESC取消
    }
    
    *value = atoi(buffer);
    return 1;  // 成功
}

// 支持ESC的double输入
int inputDoubleWithESC(double *value, const char *prompt, int x, int y) {
    char buffer[20];
    if(!inputStringWithESC(buffer, 20, prompt, x, y)) {
        return 0;  // ESC取消
    }
    
    *value = atof(buffer);
    return 1;  // 成功
}
