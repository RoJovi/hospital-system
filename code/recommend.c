#include "recommend.h"
#include "ui.h"
#include "medicine.h"

// 规则结构体
typedef struct {
    char *keywords[5];
    char medicine[50];
    int priority;
} Rule;

// 规则库
Rule rules[] = {
    {{"发烧","发热","高热","体温高",""}, "布洛芬缓释胶囊", 1},
    {{"咳嗽","咳痰","干咳","咳喘",""}, "急支糖浆", 1},
    {{"头痛","偏头痛","头疼",""}, "头痛宁胶囊", 2},
    {{"胃痛","胃胀","消化不良","胃酸"}, "奥美拉唑肠溶胶囊", 2},
    {{"过敏","皮疹","荨麻疹","瘙痒"}, "氯雷他定片", 1},
    {{"高血压","血压高","头晕"}, "硝苯地平缓释片", 1},
    {{"糖尿病","血糖高","多饮","多尿"}, "二甲双胍片", 1},
    {{"感冒","流鼻涕","鼻塞","打喷嚏"}, "感冒灵颗粒", 1},
    {{"消炎","感染","红肿","化脓"}, "阿莫西林胶囊", 2}
};

#define RULE_COUNT 9

// 推荐结果
typedef struct {
    char medicine[50];
    int score;
    char reason[100];
} RecommendResult;

// 匹配函数
RecommendResult matchSymptoms(char *input) {
    RecommendResult result = {"", 0, ""};
    int best_index = -1;
    int best_score = 0;
    int i,j = 0; 
    for( i = 0; i < RULE_COUNT; i++) {
        int score = 0;
        for(j = 0; j < 5; j++) {
            if(rules[i].keywords[j] == NULL || strlen(rules[i].keywords[j]) == 0) break;
            if(strstr(input, rules[i].keywords[j]) != NULL) {
                score++;
            }
        }
        if(score > best_score) {
            best_score = score;
            best_index = i;
        }
    }
    
    if(best_index != -1 && best_score > 0) {
        strcpy(result.medicine, rules[best_index].medicine);
        result.score = best_score * 20;
        if(result.score > 100) result.score = 100;
        sprintf(result.reason, "匹配到 %d 个关键词", best_score);
    }
    
    return result;
}

// 智能问诊功能（使用统一UI风格）
void smartConsultation() {
    system("cls");
    
    int width = 60;
    int height = 14;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "智能药品推荐");
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    char symptoms[200];
    
    // 输入症状
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    printf("请输入症状（用空格分隔）：");
    
    pos.X = x + 3; pos.Y = y + 3;
    SetConsoleCursorPosition(a, pos);
    printf("例如：发烧 咳嗽 头痛");
    
    if(!inputStringWithESC(symptoms, 200, "", x + 3, y + 4)) {
        return;
    }
    
    // 匹配
    RecommendResult result = matchSymptoms(symptoms);
    
    // 显示结果
    pos.X = x + 3; pos.Y = y + 6;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("===== 推荐结果 =====");
    setColor(WHITE);
    
    if(result.score > 0) {
        pos.X = x + 3; pos.Y = y + 7;
        SetConsoleCursorPosition(a, pos);
        printf("推荐药品：%s", result.medicine);
        
        pos.X = x + 3; pos.Y = y + 8;
        SetConsoleCursorPosition(a, pos);
        printf("匹配度：%d%%", result.score);
        
        pos.X = x + 3; pos.Y = y + 9;
        SetConsoleCursorPosition(a, pos);
        printf("原因：%s", result.reason);
        
        // 检查库存
        Medicine *m = findMedicineByName(result.medicine);
        if(m != NULL) {
            pos.X = x + 3; pos.Y = y + 10;
            SetConsoleCursorPosition(a, pos);
            printf("库存：%d盒", m->stock);
        }
    } else {
        pos.X = x + 3; pos.Y = y + 7;
        SetConsoleCursorPosition(a, pos);
        setColor(RED);
        printf("未找到匹配药品，建议咨询医生");
        setColor(WHITE);
    }
    
    pos.X = x + 3; pos.Y = y + height - 2;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("按任意键返回...");
    setColor(WHITE);
    _getch();
}
