#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <time.h>

// 颜色定义
#define RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define BLUE FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define CYAN FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define MAGENTA FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY

// 哈希表大小
#define HASH_SIZE 1000

// 常量定义
#define MAX_NAME 50
#define MAX_ID 20
#define MAX_PHONE 15
#define MAX_DEPARTMENT 30
#define MAX_MEDICINE 40
#define MAX_DIAGNOSIS 100
#define MAX_TREATMENT 100
#define ADMIN_PASSWORD "admin123"
#define USER_PASSWORD "user123"

// 日期结构体
typedef struct {
    int year;
    int month;
    int day;
} Date;

// 哈希表项结构体
typedef struct HashEntry {
    char key[MAX_ID];
    void *data;  // 指向实际数据
    struct HashEntry *next;  // 拉链法解决冲突
} HashEntry;

// 哈希表结构体
typedef struct {
    HashEntry *buckets[HASH_SIZE];
} HashMap;

// 患者结构体
typedef struct Patient {
    char id[MAX_ID];
    char name[MAX_NAME];
    int age;
    char gender[10];
    char phone[MAX_PHONE];
    char diagnosis[MAX_DIAGNOSIS];
    char treatment[MAX_TREATMENT];
    Date admission_date;
    double total_cost;
    struct Patient *next;
} Patient;

//医生结构体 
typedef struct Doctor {
    char id[MAX_ID];
    char name[MAX_NAME];
    int age;
    char gender[10];
    char department[MAX_DEPARTMENT];
    char specialty[50];
    char phone[MAX_PHONE];
    char schedule[100];
    struct Doctor *next;
} Doctor;

//药品结构体 
typedef struct Medicine {
    char id[MAX_ID];
    char name[MAX_MEDICINE];
    char specification[50];
    char manufacturer[50];
    double price;
    int stock;
    Date production_date;
    Date expiry_date;
    struct Medicine *next;
} Medicine;

//挂号单结构体 
typedef struct Registration {
    char reg_id[MAX_ID];
    char patient_id[MAX_ID];
    char doctor_id[MAX_ID];
    Date reg_date;
    char department[MAX_DEPARTMENT];
    double reg_fee;
    int status;
    struct Registration *next;
} Registration;

//费用单结构体 
typedef struct Cost {
    char cost_id[MAX_ID];
    char patient_id[MAX_ID];
    Date cost_date;
    char item[50];
    double amount;
    int payment_status;
    struct Cost *next;
} Cost;

//住院结构体 
typedef struct Hospitalization {
    char hosp_id[MAX_ID]; 
    char patient_id[MAX_ID];
    char doctor_id[MAX_ID];
    Date admission_date;
    char bed_number[20];
    char diagnosis[MAX_DIAGNOSIS];
    double total_cost;
    int status;
    struct Hospitalization *next;
} Hospitalization;

// 优先队列节点（用于挂号排队）
typedef struct QueueNode {
    Registration *reg;           // 挂号记录
    int priority;                // 优先级（1=急诊，2=普通，3=老年人优先）
    struct QueueNode *next;
} QueueNode;

// 优先队列
typedef struct {
    QueueNode *front;            // 队首（优先级最高）
    int size;                    // 队列大小
} PriorityQueue;
#endif // UTILS_H
