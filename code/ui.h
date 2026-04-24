#ifndef UI_H
#define UI_H

#include "utils.h"

// 界面函数声明
void setColor(int color);
void drawBox(int x1, int y1, int x2, int y2, int color, const char* title);
void drawMenu(int x, int y, const char* items[], int count, int highlight);
int showMenu(const char *title, const char *items[], int count);
void showMessage(const char *msg, int color);
int confirmBox(const char *msg);
void pressAnyKey();
int inputStringWithESC(char *buffer, int maxLen, const char *prompt, int x, int y);
int inputIntWithESC(int *value, const char *prompt, int x, int y);
int inputDoubleWithESC(double *value, const char *prompt, int x, int y);

#endif // UI_H
