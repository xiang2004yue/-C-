#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

void wait_enter()
{
    // 先清空缓冲区所有残留字符
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("请按回车键返回上一级菜单...");
    RESET_COLOR();
    
    // 等待用户按回车
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int login_user()
{
    User people;
    while(1)
    {
        int flag=0;
        int dd;
        int login_id;
        char username[20];

        printf("工号：");
        scanf("%d", &login_id);
        printf("密码：");
        scanf("%s", people.password);

        // 管理员：工号0 + 管理员密码
        if(login_id == 0 && strcmp(people.password, ADMIN_password) == 0)
        {
            system("cls");
            SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("管理员登录成功\n");
            RESET_COLOR();
            Sleep(1500);
            system("cls");
            return 1;
        }

        // 工号转姓名，后续完全沿用原逻辑
        if(id_to_name(login_id, username) == 0)
        {
            system("cls");
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("该工号不存在，请联系管理员\n");
            RESET_COLOR();
            Sleep(1500);
            return -1;
        }
        strcpy(people.username, username);

        // ========== 以下原代码完全不动 ==========
        FILE *pr=fopen("../data/user.txt","r");
        if(pr==NULL)
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            perror("打开密码储存文件失败");
            RESET_COLOR();
            continue ;
        }
        User user;
        while( fscanf(pr,"%s %s",user.username,user.password)==2)
        {
            if (strcmp(people.password,user.password)==0&&strcmp(people.username,user.username)==0)
            {
                system("cls");
                SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("用户登录成功\n");
                RESET_COLOR();
                Sleep(1500);
                system("cls");
                flag=1;
                break;
            }
        }
        fclose(pr);
        if(flag==1)
        {
            FILE *p=fopen("../data/temporary_buffer.txt","w");
            if(p==NULL)
            {
                SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("临时文件打开失败,无法获取个人信息\n");
                RESET_COLOR();
            }
            fprintf(p,"%s",people.username);
            fclose(p);
            return 0;
        }
        system("cls");
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("工号或者密码输入错误，是否忘记了密码,如果忘记请按3退出并联系管理员找回，否则请按其他键继续登录\n");
        RESET_COLOR();
        scanf("%d",&dd);
        if(dd==3)
        {
            break;
        }
        system("cls");
    }
}

void register_user()
{
    User re,ra,u1;
    while(1)
    {
        system("cls");
        int index=0;
        int reg_id;
        char reg_name[20];

        printf("请输入工号：");
        scanf("%d", &reg_id);
        // 先校验工号是否在员工表中
        if(id_to_name(reg_id, reg_name) == 0)
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("该工号不存在，请先让管理员录入员工信息\n");
            RESET_COLOR();
            Sleep(2000);
            return ;
        }
        strcpy(re.username, reg_name);

        // ========== 以下原查重逻辑完全不动 ==========
        FILE *pr=fopen("../data/user.txt","r");
        if(pr != NULL)
        {
            while(fscanf(pr,"%s %s",u1.username,u1.password)==2)
            {
                if(strcmp(re.username,u1.username)==0)
                {
                    index=1;
                    SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("该工号已经注册，请直接登录\n");
                    RESET_COLOR();
                    Sleep(2000);
                    return;
                }
            }
            fclose(pr); 
        }
        if(index == 0)
        {
            break; 
        }
    }

    // ========== 后面密码确认、写文件原代码完全不动 ==========
    printf("请输入密码：");
    while(1)
    {
        scanf("%s",re.password);
        printf("请再次输入密码：");
        scanf("%s",ra.password);
        if(strcmp(re.password,ra.password)==0)
        {
            system("cls");
            SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("注册成功\n");
            RESET_COLOR();
            Sleep(1500);
            system("cls");
            write_user(&re);
            FILE *p=fopen("../data/temporary_buffer.txt","w");
            if(p==NULL)
            {
                SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("临时文件打开失败,无法获取个人信息\n");
                RESET_COLOR();
            }
            fprintf(p,"%s",re.username);
            fclose(p);
            return ;
        }else
        {
            system("cls");
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("两次输入的密码不一样，请重新输入密码\n");
            RESET_COLOR();
        }
    }
}
void read_myself()
{
    char cur_name[20];
    FILE *fp_tmp=fopen("../data/temporary_buffer.txt","r");
    if(fp_tmp == NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("未检测到登录信息，请重新登录！\n");
        RESET_COLOR();
        Sleep(1500);
        system("cls");
        return;
    }
    fscanf(fp_tmp, "%s", cur_name);
    fclose(fp_tmp);
    FILE *fp_emp = fopen("../data/employee.txt", "r");
    if(fp_emp == NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("暂无员工个人信息！\n");
        RESET_COLOR();
        Sleep(1500);
        system("cls");
        return;
    }
    Data_type emp;
    int find = 0;
    while(fscanf(fp_emp, "%d %s %d %s %f", &emp.id, emp.name, &emp.age, emp.department, &emp.salary) == 5)
    {
        if(strcmp(cur_name, emp.name) == 0)
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("=========你的个人信息=========\n");
            printf("工号：%d\n", emp.id);
            printf("姓名：%s\n", emp.name);
            printf("年龄：%d\n", emp.age);
            printf("部门：%s\n", emp.department);
            printf("工资：%.2f\n", emp.salary);
            RESET_COLOR();
            find = 1;
            break;
        }
    }
    fclose(fp_emp);

    if(find == 0)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("未录入你的个人信息，请先完善资料！\n");
        RESET_COLOR();
    }
    printf("\n");
    wait_enter();
    system("cls");
}

void search_employee_by_id()
{
    int target_id;
    printf("请输入需要查询的员工工号：");
    scanf("%d", &target_id);

    FILE *fp = fopen("../data/employee.txt", "r");
    if(fp == NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("员工信息文件打开失败！\n");
        RESET_COLOR();
        Sleep(1500);
        system("cls");
        return;
    }

    Data_type emp;
    int flag = 0;
    while(fscanf(fp, "%d %s %d %s %f", &emp.id, emp.name, &emp.age, &emp.department, &emp.salary) == 5)
    {
        if(emp.id == target_id)
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("\n=====查询结果=====\n");
            printf("工号：%d\n", emp.id);
            printf("姓名：%s\n", emp.name);
            printf("年龄：%d\n", emp.age);
            printf("部门：%s\n", emp.department);
            printf("工资：%.2f\n", emp.salary);
            RESET_COLOR();
            flag = 1;
            break;
        }
    }
    fclose(fp);

    if(flag == 0)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("不存在该工号的员工！\n");
        RESET_COLOR();
    }
    wait_enter();
    system("cls");
}


void edit_self_info()//修改个人信息
{
    char str[20];
    FILE *pr=fopen("../data/temporary_buffer.txt","r");
    if(pr==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("个人信息获取失败\n");
        RESET_COLOR();
        Sleep(1500);
        system("cls");
        return ;
    }
    fscanf(pr,"%s",str);
    fclose(pr);
    Data_type all[20000];
    Data_type temp;
    int find_flag=0,count=0;
    FILE *pl=fopen("../data/employee.txt","r");
    if(pl==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("个人信息获取失败\n");
        RESET_COLOR();
        Sleep(1500);
        system("cls");
        return ;
    }
    while(fscanf(pl,"%d %s %d %s %f",&temp.id,temp.name,&temp.age,temp.department,&temp.salary)==5)
    {
        if(strcmp(str,temp.name)==0)
        {
            find_flag = 1;
            printf("====修改你的信息====\n");
            printf("原年龄：%d，原部门：%s\n",temp.age,temp.department);
            printf("输入新年龄：");
            scanf("%d",&temp.age);
            printf("输入新部门：");
            scanf("%s",temp.department);
        }
        all[count++]=temp;
    }
    fclose(pl);
    if(find_flag==0)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("未找到你的个人信息\n");
        RESET_COLOR();
        Sleep(1500);
        system("cls");
        return;
    }
    FILE *pp=fopen("../data/employee.txt","w");
    for(int i = 0; i < count; i++)
    {
        fprintf(pp,"%d %s %d %s %.2f\n",
            all[i].id,all[i].name,all[i].age,all[i].department,all[i].salary);
    }
    fclose(pp);
    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("修改完成！\n");
    RESET_COLOR();
    Sleep(1200);
    system("cls");
}

User * init_user_password_list()
{
    User *p=(User *)malloc(sizeof(User));
    if(p==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        perror("初始化链表失败\n");
        RESET_COLOR();
        return NULL;
    }
    p->next=NULL;
    return p;
}

void creat_insert_user_password_list(User *head,User user)
{
    if(head==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        perror("链表打开失败\n");
        RESET_COLOR();
        return;
    }
    User *pr=(User *)malloc(sizeof(User));
    if(pr==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        perror("初始化链表失败\n");
        RESET_COLOR();
        return ;
    }
    pr->next=NULL;
    strcpy(pr->password,user.password);
    strcpy(pr->username,user.username);
    User *p=head;
    while (p->next!=NULL)
    {
        p=p->next;
    }
    p->next=pr; 
}

void destory_user_list(User *head)
{
    if(head == NULL) return;
    User *cur = head, *tmp;
    while(cur != NULL)
    {
        tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}

void view_user_password()
{
    User *head=init_user_password_list();
    FILE  *pr=fopen("../data/user.txt","r");
    if(pr==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        perror("打开用户文件失败\n");
        RESET_COLOR();
        destory_user_list(head);
        wait_enter();
        return ;
    }
    User temp;
    while (fscanf(pr,"%s %s",temp.username,temp.password)==2)
    {
        creat_insert_user_password_list(head,temp);
    }
    fclose(pr);

    char name[20];
    printf("请输入要查看的账号：");
    scanf("%s",name);
    User *find = head->next;
    while (find!=NULL && strcmp(name,find->username)!=0)
    {
        find=find->next;
    }

    if(find==NULL)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("没有找到该账号\n");
        RESET_COLOR();
        destory_user_list(head);
        wait_enter();
        return ;
    }
    printf("该账号密码为：%s\n",find->password);
    int s;
    printf("是否重置密码为默认(0不重置 / 1重置):");
    scanf("%d",&s);

    if(s == 1)
    {
        strcpy(find->password, PASSWORD_INIT);
        FILE  *px=fopen("../data/user.txt","w");
        if(px==NULL)
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            perror("写入文件失败\n");
            RESET_COLOR();
            destory_user_list(head);
            wait_enter();
            return ;
        }
        User *write_p = head->next;
        while (write_p != NULL)
        {
            fprintf(px,"%s %s\n",write_p->username,write_p->password);
            write_p=write_p->next;      
        }
        fclose(px);
        SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("密码重置成功！默认密码：%s\n",PASSWORD_INIT);
        RESET_COLOR();
    }
    destory_user_list(head);
    wait_enter();
}

void view_myself_password()
{
    User *head=init_user_password_list();
    FILE  *pr=fopen("../data/user.txt","r");
    if(pr==NULL)
    {
        perror("打开用户文件失败\n");
        destory_user_list(head);
        wait_enter();
        return ;
    }
    User temp;
    while (fscanf(pr,"%s %s",temp.username,temp.password)==2)
    {
        creat_insert_user_password_list(head,temp);
    }
    fclose(pr);
    char name[20];
    FILE  *pp=fopen("../data/temporary_buffer.txt","r");
    if(pp==NULL)   
    {
        perror("读取登录临时信息失败\n");
        destory_user_list(head);
        wait_enter();
        return ;
    }
    fscanf(pp,"%s",name);
    fclose(pp);
    User *find = head->next;
    char new[20],new1[20];
    while (find!=NULL && strcmp(name,find->username)!=0)
    {
        find=find->next;
    }
    if(find == NULL)
    {
        printf("未找到当前登录用户！\n");
        destory_user_list(head);
        wait_enter();
        return;
    }
    printf("您当前的密码为：%s\n",find->password);
    int flag_ok = 0;
    while(1)
    {
        printf("请输入新密码：");
        scanf("%s",new);
        printf("请再次输入新密码：");
        scanf("%s",new1);

        if(strcmp(new,new1)==0)
        {
            strcpy(find->password, new);
            SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("密码更改成功!\n");
            RESET_COLOR();
            flag_ok = 1;
            break;
        }
        else
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("两次密码不一致，请重新输入！\n");
            RESET_COLOR();
            Sleep(1500);
            system("cls");
        }
    }

    if(flag_ok == 1)
    {
        FILE  *pa=fopen("../data/user.txt","w");
        if(pa==NULL)
        {
            perror("写入用户文件失败\n");
            destory_user_list(head);
            wait_enter();
            return ;
        }
        User *write_p=head->next;
        while (write_p != NULL)
        {
            fprintf(pa,"%s %s\n",write_p->username,write_p->password);
            write_p=write_p->next;
        }
        fclose(pa);
    }
    destory_user_list(head);
    wait_enter();
}