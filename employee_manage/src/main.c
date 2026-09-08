#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/head.h"



int main()
{
    int a,b,role;
    Node_list *head = init_list();
    system("color 0A"); // 0=黑色背景，A=亮绿色文字
    while(1)
    {
        while(1)
        {
            int dexp=0;
            system("cls");
            SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("-------------员工管理系统-------------\n");
            printf("1.登录账号\n");
            printf("2.注册账号登录\n");
            printf("3.退出\n");
            printf("请输入以上其中一个序号：");
            RESET_COLOR();
            scanf("%d",&a);
            switch (a)
            {
                case 1:
                    role = login_user();
                    if(role==-1)
                    {
                        break;
                    }else
                    dexp=1;
                    break;
                case 2:
                    register_user();
                    break;
                case 3:
                    return 0;   
                default:
                    SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("输入序号无效！");
                    RESET_COLOR();
                    Sleep(800);
                    break;
            }
            if(dexp==1)
            {
                break;
            }
        }
        if(role == 0)
        {
            while(1)
            {
                system("cls");
                SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("欢迎访问系统，您当前的身份为用户\n");
                printf("1.查看个人基本信息\n");
                printf("2.修改个人基本信息\n");
                printf("3.上下班打卡\n");
                printf("4.考勤汇总\n");
                printf("5.请假申请\n");
                printf("6.查看审批\n");
                printf("7.查看我的工资条\n");
                printf("8.更改密码\n");
                printf("9.退出系统\n");
                printf("请输入以上其中一个序号：");
                RESET_COLOR();
                while(scanf("%d", &b) != 1)
                {
                    while(getchar() != '\n'); // 清空缓冲区所有脏数据
                    SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("输入无效，请输入数字序号：");
                    RESET_COLOR();
                }
                switch(b)
                {
                    case 1:
                        system("cls");
                        read_myself();
                        break;
                    case 2:
                        system("cls");
                        edit_self_info();
                        break;
                    case 3:
                        system("cls");
                        clock_in_out();
                        break;
                    case 4:
                        system("cls");
                        show_my_attendance();
                        break;
                    case 5:
                        system("cls");
                        apply_leave();
                        break;
                    case 6:
                        system("cls");
                        read_apply();
                        break;
                    case 7:
                        system("cls");
                        show_my_salary();
                        break;
                    case 8:
                        system("cls");
                        view_myself_password();
                        break;
                    case 9:
                        goto exit_user_menu;
                    default:
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                        printf("无效序号！");
                        RESET_COLOR();
                        Sleep(800);
                        break;
                }
            }
            exit_user_menu: ;
        }
        else if(role == 1)
        {
            while(1)
            {
                int c;
                system("cls");
                SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("欢迎访问系统，您当前的身份为管理员\n");
                printf("1.查看所有员工的信息\n");
                printf("2.查找特定员工信息\n");
                printf("3.录入员工信息\n");
                printf("4.修改员工信息\n");
                printf("5.删除员工信息\n");
                printf("6.审批请假条\n");
                printf("7.打印工资条\n");
                printf("8.查看员工登录密码\n");
                printf("9.退出\n");
                printf("请输入以上其中一个序号：");
                RESET_COLOR();
                while(scanf("%d", &c) != 1)
                {
                    while(getchar() != '\n'); // 清空缓冲区所有脏数据
                    SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("输入无效，请输入数字序号：");
                    RESET_COLOR();
                }
                switch (c)
                {
                    case 1:
                        system("cls");
                        print_all_employee();
                        break;
                    case 2:
                        system("cls");
                        int d;
                        while(1)
                        {
                            system("cls");
                            SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                            printf("1.按姓名查找\n");
                            printf("2.按部门查找\n");
                            printf("3.按工号查找\n");
                            printf("4.返回上一级\n");
                            printf("请输入以上其中一个序号：");
                            RESET_COLOR();
                            scanf("%d",&d);
                            while(1)
                            {
                                if (d==1)
                                {
                                    system("cls");
                                    find_employee_name();
                                    wait_enter();
                                    break;
                                }else if(d==2)
                                {
                                    system("cls");
                                    find_employee_department();
                                    wait_enter();
                                    break;
                                }else if(d==3)
                                {
                                    system("cls");
                                    find_employee_id();
                                    wait_enter();
                                    break;
                                }else if(d==4)
                                {
                                    goto  break_find;
                                    break;
                                }
                                else
                                {
                                    SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                                    printf("序号有误，请重新输入！\n");
                                    RESET_COLOR();
                                    break;
                                }
                            }
                        }
                        break_find:;
                        break;
                    case 3:
                        system("cls");
                        add_employee(head);
                        break;
                    case 4:
                        system("cls");
                        modify_employee();
                        break;
                    case 5:
                        system("cls");
                        delete_employee();
                        break;
                    case 6:
                        system("cls");
                        approve_leave();
                        break;
                    case 7:
                        system("cls");
                        admin_salary_menu();
                        break;
                    case 8:
                        system("cls");
                        view_user_password();
                        break;
                    case 9:
                        goto exit_admin_menu;
                    default:
                        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                        printf("无效序号！");
                        RESET_COLOR();
                        Sleep(800);
                        break;
                }
            }
            exit_admin_menu:;
        }
    }
    destory_employee(head);
    return 0;
}