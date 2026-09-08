#ifndef _HEAD_H_
#define _HEAD_H_

typedef struct employee 
{
    int id;     
    char name[20];  
    int age; 
    char department[20]; 
    float salary;    
} Data_type;

typedef struct Node 
{
    Data_type info;
    struct Node *next;
} Node_list;

// 出勤日期链表节点
typedef struct DateNode
{
    char date[11];   // 2026-08-05
    struct DateNode *next;
} DateNode;

typedef struct User{
    char username[20];
    char password[20];
    struct User *next;
}User;

typedef struct apply
{
    char name[20];
    char department[20];
    char reason[100];
    char start_date[11];
    char end_date[11];  
    char type[12];     
    int status;   
    struct apply *next;    
}LeaveNode;

typedef struct salary_record {
    int year;
    int month;
    int id;
    char name[20];
    char department[20];
    float base_salary;
    int work_days;
    float real_salary;
    float overtime_bonus; 
    float early_deduct;  
} SalaryRecord;

typedef struct salary_node {
    SalaryRecord data;
    struct salary_node *next;
} SalaryNode;


// 导入所有模块头文件
#include "employee.h"
#include "attendance.h"
#include "apply_leave.h"
#include "user.h"
#include "utils.h"
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "salary.h"

// 控制台文字颜色快捷宏
#define SET_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color)
#define RESET_COLOR() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)

// 全局宏定义
// #define LOG_FILE "log/runtime.log"
// #define EMP_FILE "data/employees.txt"
// #define ATT_FILE "data/attendance.txt"
// #define SAL_FILE "data/salary.txt"
// #define USER_FILE "data/user.txt"

#define BASE_WORK_DAYS 22       // 月标准计薪天数
#define FULL_ATTENDANCE_BONUS 1000  // 全勤奖金额
#endif // _HEAD_H_
