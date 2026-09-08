#ifndef _APPLY_LEAVE_H_H   
#define _APPLY_LEAVE_H_H   

void apply_leave();
LeaveNode * init_apply_leave_list();
LeaveNode* create_leave_node(char *name, char *dept, char *type,
                             char *s_date, char *e_date, char *reason);
void wait_enter2();
void write_apply_file(LeaveNode *head);
void destory(LeaveNode *head);
int check_date(char *date_str);
void read_apply();
void approve_leave();
void write_file_apply(LeaveNode *head);
#endif 
