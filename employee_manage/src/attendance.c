#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"
// 获取当前时间，输出格式 2026-08-05 14:30:22
void get_now_time(char buf[], int buf_len)
{
    time_t t;
    struct tm tm_now;
    time(&t);
    localtime_s(&tm_now, &t); // Windows安全获取时间
    strftime(buf, buf_len, "%Y-%m-%d %H:%M:%S", &tm_now);
}

void wait_enter1()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("请按回车键进行打卡\n");
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void clock_in_out()
{
    int type;
    char name[20];
    char tag[20];
    FILE *p=fopen("../data/temporary_buffer.txt","r");
    if (p==NULL|| match_myself()==0)
    {
        printf("获取个人信息失败\n");
        Sleep(1500);
        return;
    }
    fscanf(p,"%s",name);
    fclose(p);
    char time_str[32] = {0};
    get_now_time(time_str, sizeof(time_str));
    printf("请选择上班还是下班(0/1):");
    scanf("%d",&type);
    if(type==0)
    {
        strcpy(tag, "上班"); 
    }
    else if(type==1)
    {
        strcpy(tag, "下班"); 
    }else
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("输入错误，只能选0或1！\n");
        RESET_COLOR();
        Sleep(1200);
        return;
    }
    wait_enter1();
    system("cls");
    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("====打卡成功====\n");
    printf("姓名：%s\n", name);
    printf("类型：%s\n", tag);
    printf("时间：%s\n", time_str);
    RESET_COLOR();
    FILE *pr=fopen("../data/attendance.txt","a");
    if(pr==NULL)
    {
        printf("考勤文件打开失败\n");
        return;
    }
    fprintf(pr,"%s %s %s\n",name,tag,time_str);
    fclose(pr);
    ungetc('\n', stdin); 
    wait_enter();
    system("cls");
}

DateNode *init_Data_list()
{
    DateNode *p=(DateNode *)malloc(sizeof(DateNode));
    if(p==NULL)
    {
        printf("创建链表失败\n");
        return NULL;
    }
    p->next=NULL;
    return p;
}

DateNode *creat_Date_list_node(const char *d)
{
    DateNode *p = (DateNode*)malloc(sizeof(DateNode));
    p->next = NULL;
    strcpy(p->date, d);  
    return p;
}

int data_exis(DateNode *head,const char *d)
{
    DateNode *p=head->next;
    while(p)
    {
        if(strcmp(p->date,d)==0)
        {
            return 1;
        }
        p=p->next;
    }
    return 0;
}

void date_append(DateNode *head, const char *d)
{
    DateNode *p=head;
    if(data_exis(head,d)==0)
    {
        while (p->next!=NULL)
        {
            p=p->next;
        }
        DateNode *pr=creat_Date_list_node(d);
        p->next=pr;
    }else return ;
}

int count_date_list(DateNode *head)
{
    int cnt = 0;
    DateNode *p = head->next;
    while(p)
    {
        cnt++;
        p = p->next;
    }
    return cnt;
}

void free_date_list(DateNode *head)
{
    DateNode *p = head->next, *q;
    while(p)
    {
        q = p;
        p = p->next;
        free(q);
    }
    head->next = NULL;
}

int match_myself()
{
    FILE *p=fopen("../data/temporary_buffer.txt","r");
    if (p==NULL)
    {
        return 0;
    }
    char name[20];
    fscanf(p,"%s",name);
    fclose(p);
    Data_type arr;
    FILE *pr=fopen("../data/employee.txt","r");
    if (pr==NULL)
    {
        return 0;
    }
    while (fscanf(pr,"%d %s %d %s %f",&arr.id,arr.name,&arr.age,arr.department,&arr.salary)==5)
    {
        if(strcmp(arr.name,name)==0)
        {
            return 1;
        }
    }
    return 0;
}

void show_my_attendance()
{
    FILE *p=fopen("../data/temporary_buffer.txt","r");
    if (p==NULL || match_myself()==0)
    {
        printf("获取个人信息失败\n");
        Sleep(2000);
        return;
    }
    char name[20];
    fscanf(p,"%s",name);
    fclose(p);
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("====================我的考勤汇总====================\n");
    FILE *pr=fopen("../data/attendance.txt","r");
    if (pr==NULL)
    {
        printf("暂无出勤记录\n");
        wait_enter1();
        system("cls");
        return ;
    }
    DateNode *work_head = init_Data_list();
    DateNode *valid_head = init_Data_list();
    char name_buf[20];
    char type_buf[10];
    char time_buf[32];
    while(fscanf(pr,"%s %s %[^\n]",name_buf,type_buf,time_buf)==3)
    {
        if(strcmp(name_buf,name)!=0)
            continue;
        printf("%s\t%s\n",time_buf,type_buf);
        char data[11];
        strncpy(data,time_buf,10);
        data[10]='\0';
        if(strcmp(type_buf,"上班")==0)
        {
            date_append(work_head,data);
        }
        else if(strcmp(type_buf,"下班")==0)
        {
            if(data_exis(work_head,data))
            {
                date_append(valid_head,data);
            }
        }
    }
    fclose(pr);
    int total_day=count_date_list(valid_head);
    printf("----------------------------------------------------\n");
    printf("有效出勤总天数：%d 天\n", total_day);
    printf("====================================================\n");
    RESET_COLOR();
    free_date_list(work_head);
    free_date_list(valid_head);
    free(work_head);
    free(valid_head);
    wait_enter();
    system("cls");
}

