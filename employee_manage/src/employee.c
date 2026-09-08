#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

void admin_input_info(Data_type *stm)
{
    printf("请录入员工完整信息：\n");
    printf("姓名：");
    scanf("%s",stm->name);
    printf("年龄：");
    scanf("%d",&stm->age);
    printf("部门：");
    scanf("%s",stm->department);
    printf("薪资：");
    scanf("%f",&stm->salary);
    int id=get_max_id();
    stm->id=id;
}
//接受信息插入链表中
void admin_read_info(Node_list *head,int id,char name[20],int age,char dep[20],float salary)
{
    if(head == NULL)
    {
        printf("员工链表初始化失败\n");
        Sleep(2000);
        return;
    }
    Node_list *new_node = (Node_list*)malloc(sizeof(Node_list));
    if(new_node == NULL)
    {
        perror("malloc fail");
        return;
    }
    new_node->info.id = id;
    new_node->info.age = age;
    strcpy(new_node->info.name, name);
    strcpy(new_node->info.department, dep);
    new_node->info.salary = salary;
    new_node->next = NULL;
    Node_list *p = head;
    while(p->next != NULL)
    {
        p = p->next;
    }
    p->next = new_node;
}

void admin_read_info_employee(Node_list *head,Data_type *p)//升级版插入链表
{
    if(head == NULL)
    {
        printf("员工链表初始化失败\n");
        Sleep(2000);
        return;
    }
    Node_list *new_node = (Node_list*)malloc(sizeof(Node_list));
    if(new_node == NULL)
    {
        perror("malloc fail");
        return;
    }
    new_node->info.id = p->id;
    new_node->info.age = p->age;
    strcpy(new_node->info.name, p->name);
    strcpy(new_node->info.department, p->department);
    new_node->info.salary = p->salary;
    new_node->next = NULL;
    Node_list *pr = head;
    while(pr->next != NULL)
    {
        pr = pr->next;
    }
    pr->next = new_node;
}

Node_list* init_list(void)//创建个人信息链表
{
    Node_list *head = (Node_list*)malloc(sizeof(Node_list));
    if(head == NULL)
    {
        perror("malloc fail");
        return NULL;
    }
    head->next = NULL;
    return head;
}

Node_list* creat_node()//管理员创建个人信息节点
{
    Node_list *p=(Node_list *)malloc(sizeof(Node_list));
    if(p==NULL) return NULL;
    admin_input_info(&p->info);
    p->next=NULL;
    return p;
}

Node_list* creat_node_employee(Data_type* node)//管理员接受个人信息节点
{
    Node_list *p=(Node_list *)malloc(sizeof(Node_list));
    if(p==NULL) return NULL;
    p->next=NULL;
    p->info.id=node->id;
    strcpy(p->info.name,node->name);
    p->info.age=node->age;
    strcpy(p->info.department,node->department);
    p->info.salary=node->salary;
    return p;
}

void inset_list(Node_list *head)//插入个人信息
{
    Node_list *new_node=creat_node();
    if(new_node == NULL) return;
    Node_list *hid=head;
    while(hid->next!=NULL)
    {
        hid=hid->next;
    }
    hid->next=new_node;
    write_attendance(new_node->info);
}

int inset_list_employee(Node_list *head)//管理员插入个人信息
{
    Node_list *new_node=creat_node();
    if(new_node == NULL) return 0;
    int  a=match_employee(new_node->info.name);
    if(a==0)
    {
        printf("该员工已存在!\n");
        free(new_node); 
        return 1;
    } 
    Node_list *hid=head;
    while(hid->next!=NULL)
    {
        hid=hid->next;
    }
    hid->next=new_node;
    write_attendance(new_node->info);
    return 0;
}

int  match_employee(char *name)
{
    FILE *p=fopen("../data/employee.txt","r");
    if (p==NULL)
    {
        return 1;
    }
    Data_type str;
    while(fscanf(p,"%d %s %d %s %f",&str.id,str.name,&str.age,str.department,&str.salary)==5)
    {
        if(strcmp(str.name,name)==0)
        {
            fclose(p); 
            return 0;
        }
    }
    fclose(p); 
    return 1;
}

void destory_employee(Node_list *head)
{
    if(head==NULL)
    {
        return;
    }
    Node_list *p=head;
    Node_list *q;
    while(p!=NULL)
    {
        q=p->next;
        free(p);
        p=q;
    }

}

Node_list* read_all_employee()
{
    FILE *p=fopen("../data/employee.txt","r");
    if (p==NULL)
    {
        printf("暂无员工信息!\n");
        Sleep(2000);
        return NULL;
    }
    Node_list *pr=init_list();
    Data_type temp;
    while (fscanf(p,"%d %s %d %s %f",&temp.id,temp.name,&temp.age,temp.department,&temp.salary)==5)
    {
        admin_read_info(pr,temp.id,temp.name,temp.age,temp.department,temp.salary);
    }   
    fclose(p);
    return pr;
}

void print_all_employee()
{
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("==================所有员工信息=================\n");
    Node_list *p;
    p=read_all_employee();
    if(p == NULL)   
    {
        wait_enter();
        return;
    }
    Node_list *pr=p->next;
    int denpx=0;
    while (pr!=NULL)
    {
        printf("%d %s %d %s %.2f\n",pr->info.id,pr->info.name,pr->info.age,pr->info.department,pr->info.salary);
        pr=pr->next;
        denpx++;
    }
    printf("-----------------------------------------------\n");
    printf("                                         共计%d人\n",denpx);
    RESET_COLOR();
    wait_enter();
    destory_employee(p);
}

void find_employee_name()
{
    Node_list *p;
    p=read_all_employee();
    if(p == NULL)   
    {
        wait_enter();
        return;
    }
    Node_list *pp=p->next;
    char name[20];
    printf("====================\n");
    printf("请输入查找员工的姓名:");
    scanf("%s",name);
    while (pp!=NULL &&  strcmp(name,pp->info.name)!=0)
    {
        pp=pp->next;
    }
    if(pp!=NULL && strcmp(name,pp->info.name)==0)
    {
        printf("%d %s %d %s %.2f\n",pp->info.id,pp->info.name,pp->info.age,pp->info.department,pp->info.salary);
        destory_employee(p); 
        return;
    }
    else
    {
        printf("没有查询到此员工信息\n");
        destory_employee(p); 
        return;
    }
}

void find_employee_id()
{
    Node_list *p;
    p = read_all_employee();
    if(p == NULL)   
    {
        wait_enter();
        return;
    }
    Node_list *pp = p->next;
    int id;
    printf("====================\n");
    printf("请输入查找员工的工号:");

    // 新增：输入非法就循环重输，自动清空缓冲区
    while(scanf("%d", &id) != 1)
    {
        while(getchar() != '\n');
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("输入无效，请输入数字工号:");
        RESET_COLOR();
    }

    while (pp != NULL && pp->info.id != id)
    {
        pp = pp->next;
    }
    if(pp != NULL && pp->info.id == id)
    {
        printf("%d %s %d %s %.2f\n", pp->info.id, pp->info.name, pp->info.age, pp->info.department, pp->info.salary);
        destory_employee(p); 
        return;
    }
    else
    {
        printf("没有查询到此员工信息\n");
        destory_employee(p); 
        return;
    }
}

void find_employee_department()
{
    Node_list *p;
    p=read_all_employee();
    if(p == NULL)   
    {
        wait_enter();
        return;
    }
    Node_list *pp=p->next;
    char dep[20];
    int denpx=0;
    printf("====================\n");
    printf("请输入查找员工的部门:");

    scanf("%s",dep);
    while (pp!=NULL)
    {
        if(strcmp(dep,pp->info.department)==0)
        {
            printf("%d %s %d %s %.2f\n",pp->info.id,pp->info.name,pp->info.age,pp->info.department,pp->info.salary);
            denpx++;
        }
        pp=pp->next;
    }
    if(denpx==0)
    {
        printf("没有查询到员工信息\n");
        destory_employee(p); 
        return;
    }else
    {
        printf("--------------------\n");
        printf("             共有%d人\n",denpx);
        destory_employee(p); 
        return;
    }
}


void add_employee(Node_list *head)
{
    char a1;
    while(1)
    {
        int a=inset_list_employee(head);
        if(a==0)
        {  
            printf("录入成功!\n");
        }
        while(1)
        {
            printf("是否继续添加员工信息(Y/N)：");
            scanf(" %c", &a1);
            if(a1 == 'Y' || a1 == 'y')
            {
                system("cls");
                break;
            }
            else if(a1 == 'N' || a1 == 'n')
            {
                Sleep(800);
                return;
            }
            else
            {
                SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("输入错误，请重新选择！\n");
                RESET_COLOR();
                Sleep(800);
                system("cls");
            }
        }
    }
}

void modify_employee()
{
    Node_list *head = read_all_employee();
    if(head == NULL)
    {
        printf("暂无员工数据，无法修改！\n");
        wait_enter();
        return;
    }
    int select_find;
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("=====选择查找方式=====\n");
    printf("1.按员工ID查找\n");
    printf("2.按员工姓名查找\n");
    RESET_COLOR();
    Node_list *p = NULL;

    while(1)
    {
        printf("请选择：");
        int ret = scanf("%d",&select_find);
        if(ret != 1)
        {
            while(getchar() != '\n');
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("输入不是数字，请输入1或者2！\n");
            RESET_COLOR();
            continue;
        }

        if(select_find == 1)
        {
            int tar_id;
            printf("请输入要修改员工的ID：");
            scanf("%d",&tar_id);
            p = head->next;
            while(p != NULL && p->info.id != tar_id)
            {
                p = p->next;
            }
            break; 
        }
        else if(select_find == 2)
        {
            char tar_name[20];
            printf("请输入要修改员工的姓名：");
            scanf("%s",tar_name);
            p = head->next;
            while(p != NULL && strcmp(p->info.name, tar_name) != 0)
            {
                p = p->next;
            }
            break; 
        }
        else
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("选择错误！请输入1或者2\n");
            RESET_COLOR();
        }
    }

    if(p == NULL)
    {
        printf("没有找到该员工！\n");
        destory_employee(head);
        wait_enter();
        return;
    }
    system("cls");
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n=====找到员工=====\n");
    printf("ID：%d\n",p->info.id);
    printf("姓名：%s\n",p->info.name);
    printf("年龄：%d\n",p->info.age);
    printf("部门：%s\n",p->info.department);
    printf("薪资：%.2f\n",p->info.salary);
    RESET_COLOR();
    int op;
    while(1)
    {
        SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("\n=====修改菜单=====\n");
        printf("1 修改姓名\n");
        printf("2 修改年龄\n");
        printf("3 修改部门\n");
        printf("4 修改薪资\n");
        printf("0 保存并退出修改\n");
        RESET_COLOR();
        printf("请输入要修改的选项：");
        int ret2 = scanf("%d",&op);
        if(ret2 != 1)
        {
            while(getchar()!='\n');
            printf("请输入数字选项！\n");
            continue;
        }

        if(op == 0)
        {
            break;
        }
        if(op == 1)
        {
            printf("输入新姓名：");
            scanf("%s",p->info.name);
        }
        else if(op == 2)
        {
            printf("输入新年龄：");
            scanf("%d",&p->info.age);
        }
        else if(op == 3)
        {
            printf("输入新部门：");
            scanf("%s",p->info.department);
        }
        else if(op == 4)
        {
            printf("输入新薪资：");
            scanf("%f",&p->info.salary);
        }
        else
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("无效选项，请重新输入！\n");
            RESET_COLOR();
        }
        system("cls");
        printf("--修改后--\n");
        printf("ID:%d 姓名:%s 年龄:%d 部门:%s 薪资:%.2f\n",
            p->info.id,p->info.name,p->info.age,p->info.department,p->info.salary);
    }

    FILE *fp = fopen("../data/employee.txt","w");
    if(fp == NULL)
    {
        perror("保存文件失败");
        destory_employee(head);
        wait_enter();
        return;
    }
    Node_list *cur = head->next;
    while(cur != NULL)
    {
        fprintf(fp,"%d %s %d %s %.2f\n",
            cur->info.id,
            cur->info.name,
            cur->info.age,
            cur->info.department,
            cur->info.salary);
        cur = cur->next;
    }
    fclose(fp);
    system("cls");
    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\n修改已保存到文件！\n");
    RESET_COLOR();
    wait_enter();
    destory_employee(head);
}

void delete_employee()
{
    Node_list *head = read_all_employee();
    if(head == NULL)
    {
        printf("暂无员工数据,无需删除！\n");
        wait_enter();
        return;
    }
    int a;
    printf("=======请选择删除方式==========\n");
    printf("1.按工号删除\n");
    printf("2.按名字删除\n");
    while (1)
    {    
        printf("请输入要修改的选项：");
        scanf("%d",&a);
        int denxp = 0;   
        Node_list *p = head; 
        if(a==1)
        {
            int id;
            printf("请输入要删除的工号:");
            scanf("%d",&id);
            while(p!=NULL && p->next!=NULL)
            {
                if(p->next->info.id == id)
                {
                    Node_list *pp = p->next;
                    p->next = pp->next;
                    free(pp);
                    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    printf("已经删除该工号员工信息\n");
                    RESET_COLOR();
                    denxp=1;
                    break;
                }
                p=p->next;
            }
            if(denxp==0)
            {
                printf("没有找到该工号员工\n");
            }
        }
        else if(a==2)
        {
            char name[20];
            printf("请输入要删除的名字:");
            scanf("%s",name);
            while(p!=NULL && p->next!=NULL)
            {
                if(strcmp(name, p->next->info.name)==0)
                {
                    Node_list *pp = p->next;
                    p->next = pp->next;
                    free(pp);
                    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    printf("已经删除该工号员工信息\n");
                    RESET_COLOR();
                    denxp=1;
                    break;
                }
                p=p->next;
            }
            if(denxp==0)
            {
                printf("没有找到该名字员工\n");
            }
        }
        else
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("选项错误，请重新输入:\n");
            RESET_COLOR();
            continue;
        }
        if(denxp == 1)
        {
            break;
        }
    }
    printf_all_list(head);
    destory_employee(head);
}

void printf_all_list(Node_list * head)
{
  FILE *fp = fopen("../data/employee.txt","w");
    if(fp == NULL)
    {
        perror("保存员工文件失败");
        wait_enter();
        return;
    }
    Node_list *cur = head->next;
    while(cur != NULL)
    {
        fprintf(fp,"%d %s %d %s %.2f\n",
            cur->info.id,
            cur->info.name,
            cur->info.age,
            cur->info.department,
            cur->info.salary);
        cur = cur->next;
    }
    fclose(fp);
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("删除操作已保存到文件！\n");
    RESET_COLOR();
    wait_enter();
}