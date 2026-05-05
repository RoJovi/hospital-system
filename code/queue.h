#ifndef QUEUE_H
#define QUEUE_H

#include "utils.h"
#include "patient.h"

// 优先队列函数声明
void initQueue(PriorityQueue *q);
int isQueueEmpty(PriorityQueue *q);
void enqueue(PriorityQueue *q, Registration *reg);
Registration* dequeue(PriorityQueue *q);
Registration* peekQueue(PriorityQueue *q);
void showQueue(PriorityQueue *q);

#endif // QUEUE_H
