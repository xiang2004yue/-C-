#ifndef EMPLOYEE_H_H
#define EMPLOYEE_H_H
#include "head.h"
Node_list* init_list(void);
Node_list* creat_node();
void inset_list(Node_list *head);
Node_list* read_all_employee();
void admin_read_info(Node_list *head,int id,char name[20],int age,char dep[20],float salary);
void destory_employee(Node_list *head);
void print_all_employee();
void find_employee_department();
void find_employee_name();
void find_employee_id();
void add_employee(Node_list *head);
int  match_employee(char *name);
int inset_list_employee(Node_list *head);
void modify_employee();
Node_list* creat_node_employee(Data_type* node);
void admin_read_info_employee(Node_list *head,Data_type *p);
void delete_employee();
void printf_all_list(Node_list * head);
#endif 
