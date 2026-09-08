#ifndef SALARY_H_H
#define SALARY_H_H

#include "head.h"

void adjust_salary();
void generate_all_salary();
void show_my_salary();
void admin_salary_menu();
static SalaryNode* read_all_salary();
static void destory_salary_list(SalaryNode *head);
void adjust_salary_record();
static void count_early_overtime(char *name, int year, int month, int *early_days, float *overtime_hours);
#endif