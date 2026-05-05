#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include "ui.h"

// 初始化优先队列
void initQueue(PriorityQueue *q) {
    q->front = NULL;
    q->size = 0;
}

// 判断队列是否为空
int isQueueEmpty(PriorityQueue *q) {
    return q->front == NULL;
}

// 获取优先级数值（数字越小优先级越高）
int getPriorityValue(Registration *reg) {
    // 可以根据病情紧急程度、年龄等设置优先级
    // 这里示例：1=急诊(最高), 2=老年人(中), 3=普通(低)
    
    // 查找患者信息获取年龄
    Patient *p = findPatientById(reg->patient_id);
    if(p != NULL) {
        if(p->age >= 70) {
            return 2;  // 老年人优先
        }
    }
    
    // 根据科室判断（示例）
    if(strcmp(reg->department, "急诊科") == 0) {
        return 1;  // 急诊最高优先级
    }
    
    return 3;  // 普通
}

// 入队（按优先级插入）
void enqueue(PriorityQueue *q, Registration *reg) {
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->reg = reg;
    newNode->priority = getPriorityValue(reg);
    newNode->next = NULL;
    
    // 空队列，直接插入
    if(q->front == NULL) {
        q->front = newNode;
        q->size++;
        return;
    }
    
    // 按优先级插入（优先级数字越小越靠前）
    QueueNode *curr = q->front;
    QueueNode *prev = NULL;
    
    while(curr != NULL && curr->priority <= newNode->priority) {
        prev = curr;
        curr = curr->next;
    }
    
    if(prev == NULL) {
        // 插入到头部
        newNode->next = q->front;
        q->front = newNode;
    } else {
        // 插入到中间或尾部
        prev->next = newNode;
        newNode->next = curr;
    }
    q->size++;
}

// 出队（取出优先级最高的）
Registration* dequeue(PriorityQueue *q) {
    if(q->front == NULL) {
        return NULL;
    }
    
    QueueNode *temp = q->front;
    Registration *reg = temp->reg;
    q->front = q->front->next;
    free(temp);
    q->size--;
    return reg;
}

// 查看队首（不移除）
Registration* peekQueue(PriorityQueue *q) {
    if(q->front == NULL) {
        return NULL;
    }
    return q->front->reg;
}

// 显示排队队列
void showQueue(PriorityQueue *q) {
    system("cls");
    
    int width = 70;
    int height = 20;
    int screenWidth = 80;
    int screenHeight = 25;
    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    
    drawBox(x, y, x + width, y + height, CYAN, "挂号排队队列（优先级顺序）");
    
    if(q->front == NULL) {
        HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos;
        pos.X = x + 3; pos.Y = y + 2;
        SetConsoleCursorPosition(a, pos);
        setColor(YELLOW);
        printf("当前无排队患者");
        setColor(WHITE);
        pressAnyKey();
        return;
    }
    
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    
    // 显示表头
    pos.X = x + 3; pos.Y = y + 2;
    SetConsoleCursorPosition(a, pos);
    setColor(YELLOW);
    printf("%-5s %-8s %-12s %-10s %-15s", "序号", "优先级", "患者ID", "科室", "挂号时间");
    setColor(WHITE);
    
    QueueNode *curr = q->front;
    int count = 0;
    int currentY = y + 3;
    
    while(curr && currentY < y + height - 3) {
        count++;
        pos.X = x + 3; pos.Y = currentY++;
        SetConsoleCursorPosition(a, pos);
        
        char *priorityStr;
        switch(curr->priority) {
            case 1: priorityStr = "急诊"; break;
            case 2: priorityStr = "老年优先"; break;
            default: priorityStr = "普通";
        }
        
        printf("%-5d %-8s %-12s %-10s %-15s",
               count, priorityStr, curr->reg->patient_id, 
               curr->reg->department, "今日");
        
        curr = curr->next;
    }
    
    pos.X = x + 3; pos.Y = y + height - 2;
    SetConsoleCursorPosition(a, pos);
    setColor(GREEN);
    printf("共 %d 人在排队", q->size);
    setColor(WHITE);
    
    pressAnyKey();
}
