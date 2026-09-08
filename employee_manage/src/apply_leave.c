#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

void wait_enter2()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("请按回车键进行提交申请...");
    while ((ch = getchar()) != '\n' && ch != EOF);
}

LeaveNode * init_apply_leave_list()
{
    LeaveNode *head=(LeaveNode*)malloc(sizeof(LeaveNode));
    if(head==NULL)
    {
        return NULL;
    }
    head->next=NULL;
    return head;
}

LeaveNode *create_leave_node(char *name, char *dept, char *type,
                             char *s_date, char *e_date, char *reason)
{
    LeaveNode *new_node = (LeaveNode*)malloc(sizeof(LeaveNode));
    if(new_node == NULL)
    {
        perror("malloc leave node fail");
        return NULL;
    }
    strcpy(new_node->name, name);
    strcpy(new_node->department, dept);
    strcpy(new_node->type, type);
    strcpy(new_node->start_date, s_date);
    strcpy(new_node->end_date, e_date);
    strcpy(new_node->reason, reason);
    new_node->status = 0;
    new_node->next = NULL;
    return new_node;
}

void leave_append(LeaveNode *head, LeaveNode *new_node)
{
    LeaveNode *p = head;
    while(p->next != NULL)
    {
        p = p->next;
    }
    p->next = new_node;
}
/*
void write_apply_file(LeaveNode *head)
{
    if(head==NULL)
    {
        printf("没有申请信息\n");
        return;
    }
    FILE *pp=fopen("../data/apply_leave.txt","a");
    if(pp == NULL)
    {
        perror("打开请假文件失败");
        return;
    }
    LeaveNode *p=head->next;
    while(p)
    {
        fprintf(pp,"%s %s %s %s %s %s %d\n",p->name,p->department,p->type,p->start_date,p->end_date,p->reason,p->status);
        p=p->next;
    }
    fclose(pp);
}
*/
void destory(LeaveNode *head)
{
    if(head==NULL)
    {
        return;
    }
    LeaveNode *p=head;
    LeaveNode *q;
    while(p!=NULL)
    {
        q=p->next;
        free(p);
        p=q;
    }

}

int check_date(char *date_str)
{
    if(strlen(date_str) != 10)
        return 0;
    if(date_str[4] != '-' || date_str[7] != '-')
        return 0;
    int year,mon,day;
    if(sscanf(date_str,"%d-%d-%d",&year,&mon,&day) !=3)
        return 0;
    if(mon <1 || mon>12) return 0;
    if(day <1 || day>31) return 0;
    return 1;
}
void apply_leave()
{
    FILE *p=fopen("../data/temporary_buffer.txt","r");
    if (p==NULL||match_myself()==0)
    {
        printf("获取个人信息失败\n");
        Sleep(2000);
        return;
    }
    char name[20];
    fscanf(p,"%s",name);
    fclose(p);
    char department[20];
    char reason[100];
    char start_date[11];
    char end_date[11];
    char type[12];
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("===================请假申请表==================\n");
    printf("姓名：%s\n",name);
    RESET_COLOR();
    printf("部门：");
    scanf("%s",department);
    printf("请假类型(事假/病假/年假)：");
    scanf("%s",type);
    while(1)
    {
        printf("请假开始时间(yyyy-mm-dd)：");
        scanf("%s",start_date);
        int a=check_date(start_date);
        if (a==1)
        {
            break;
        }
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("格式不对，请重新填写:\n");
        RESET_COLOR();
    }
    while(1)
    {
        printf("请假结束时间(yyyy-mm-dd)：");
        scanf("%s",end_date);
        int a=check_date(end_date);
        if (a==1)
        {
            break;
        }
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("格式不对，请重新填写:\n");
        RESET_COLOR();
    }
    printf("请假理由：");
    scanf("%s",reason);
    wait_enter2();
    LeaveNode *list_head = init_apply_leave_list();
    FILE *fp = fopen("../data/apply_leave.txt","r");
    if(fp != NULL)
    {
        char n[20],d[20],t[12],s[11],e[11],r[100];
        int sta;
        while(fscanf(fp,"%s %s %s %s %s %s %d",n,d,t,s,e,r,&sta) ==7)
        {
            LeaveNode *new = create_leave_node(n,d,t,s,e,r);
            new->status = sta;
            leave_append(list_head, new);
        }
        fclose(fp);
    }
    LeaveNode * pr=create_leave_node(name,department,type,start_date, end_date,reason);
    leave_append(list_head,pr);
    write_file_apply(list_head);
    destory(list_head);
    Sleep(1500);
    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("提交申请成功！请等待审批\n");
    RESET_COLOR();
    ungetc('\n', stdin); 
    wait_enter();
}

void read_apply()
{
    FILE *p=fopen("../data/temporary_buffer.txt","r");
    if (p==NULL||match_myself()==0)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("获取个人信息失败\n");
        RESET_COLOR();
        Sleep(2000);
        return;
    }
    char name[20];
    fscanf(p,"%s",name);
    fclose(p);
    FILE *pp=fopen("../data/apply_leave.txt","r");
    if (pp==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("请假信息为空\n");
        RESET_COLOR();
        Sleep(2000);
        return;
    }
    char name_01[20];
    char dep[20];
    char type[20];
    char start_data[11];
    char end_data[11];
    char reason[100];
    int status;
    int denpx=0;
    printf("===============审批记录==================\n");
    while(fscanf(pp,"%s %s %s %s %s %s %d",name_01,dep,type,start_data,end_data,reason,&status)==7)
    {
        if(strcmp(name,name_01)==0)
        {
            denpx=1;
            if(status==0)
            {
                printf("%s %s %s %s %s %s 待审批\n",name,dep,type,start_data,end_data,reason);
            }
            if(status==1)
            {
                printf("%s %s %s %s %s %s 审批通过\n",name,dep,type,start_data,end_data,reason);
            }
            if(status==-1)
            {
                printf("%s %s %s %s %s %s 审批不通过\n",name,dep,type,start_data,end_data,reason);
            }
        }
    }
    if(denpx==0)
    {
        printf("没有请假记录\n");
    }
    fclose(pp);
    wait_enter();
}

void write_file_apply(LeaveNode *head)
{
    LeaveNode *p=head->next;
    FILE  *pr=fopen("../data/apply_leave.txt","w");
    if(pr==NULL)
    {
        perror("打开请假信息表失败\n");
        return;
    }
    while(p!=NULL)
    {
        fprintf(pr,"%s %s %s %s %s %s %d\n",p->name,p->department,p->type,p->start_date,p->end_date,p->reason,p->status);
        p=p->next;
    }
    fclose(pr);
    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("审批结果已保存！\n");
    RESET_COLOR();
}

void approve_leave()
{
    FILE *fp = fopen("../data/apply_leave.txt","r");
    if(fp == NULL)
    {
        printf("暂无请假申请！\n");
        wait_enter();
        return;
    }
    LeaveNode *list_head = init_apply_leave_list();
    Data_type temp;
    char n[20],d[20],t[12],s[11],e[11],r[100];
    int sta;
    while(fscanf(fp,"%s %s %s %s %s %s %d",n,d,t,s,e,r,&sta) ==7)
    {
        LeaveNode *new = create_leave_node(n,d,t,s,e,r);
        new->status = sta;
        leave_append(list_head, new);
    }
    fclose(fp);
    LeaveNode *cur = list_head->next;
    int has_apply = 0;
    while(cur != NULL)
    {
        if(cur->status == 0) 
        {
            has_apply = 1;
            printf("\n====待审批申请====\n");
            printf("姓名：%s\n部门：%s\n请假类型：%s\n时间：%s ~ %s\n理由：%s\n",
                cur->name,cur->department,cur->type,cur->start_date,cur->end_date,cur->reason);
            int op,ret;
            printf("1 通过  2 驳回  跳过本次(其他数字)：");
            ret = scanf("%d",&op);
            if(ret !=1)
            {
                while(getchar()!='\n');
                SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("输入无效，跳过该条\n");
                RESET_COLOR();
            }
            else if(op == 1)
                cur->status = 1;
            else if(op == 2)
                cur->status = -1;
            system("cls");
        }
        cur = cur->next;
    }
    if(!has_apply)
    {
        printf("当前没有待审批的请假单\n");
        destory(list_head);
        wait_enter();
        return;
    }
    write_file_apply(list_head);
    destory(list_head);
    wait_enter();
}
