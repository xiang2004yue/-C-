#ifndef USER_H_H
#define USER_H_H
#include "head.h"


#define ADMIN_username "admin1234"
#define ADMIN_password "123456"
#define PASSWORD_INIT "666666"

int login_user();
void register_user();
void read_myself();
void edit_self_info ();
void wait_enter();
void view_user_password();
User * init_user_password_list();
void creat_insert_user_password_list(User *head,User date);
void destory_user_list(User *head);
void view_myself_password();
#endif 
