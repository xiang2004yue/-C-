#ifndef ATTENDANCE_H_H
#define ATTENDANCE_H_H


void get_now_time(char buf[], int buf_len);
void clock_in_out();
void wait_enter1();
DateNode *init_Data_list();
DateNode *creat_Date_list_node(const char *d);
int data_exis(DateNode *head,const char *d);
void date_append(DateNode *head, const char *d);
void show_my_attendance();
int count_date_list(DateNode *head);
void free_date_list(DateNode *head);
int match_myself();
#endif 
