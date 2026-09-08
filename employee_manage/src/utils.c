#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

int get_max_id(void)//id自动增加
{
    FILE *fp=fopen("../data/employee.txt","r");
    int max_id=0;
    Data_type temp;
    if(fp==NULL)
    {
        max_id=2000;
        return max_id;
    }
    while(fscanf(fp,"%d %s %d %s %f", &temp.id, temp.name, &temp.age, temp.department,&temp.salary) == 5)
    {
        if(temp.id > max_id)
        {
            max_id = temp.id;
        }
    }
    fclose(fp);
    return max_id+1;
}

void write_attendance(Data_type stm)//写入员工数据
{
    FILE *pr=fopen("../data/employee.txt","a");
    if(pr==NULL)
    {
        perror("打开员工文件失败");
        return ;
    }
    fprintf(pr,"%d %s %d %s %.2f\n",
    stm.id,stm.name,stm.age,stm.department,stm.salary);
    printf("\n");
    fclose(pr);
}

void write_user(User *sd)//写入账户密码
{
    FILE *pr=fopen("../data/user.txt","a");
    if(pr==NULL)
    {
        perror("打开密码储存文件失败");
        return ;
    }
    fprintf(pr,"%s %s\n",sd->username,sd->password);
    printf("\n");
    fclose(pr);
}

// 工号转姓名，成功返回1，失败返回0
int id_to_name(int emp_id, char *out_name)
{
    FILE *fp = fopen("../data/employee.txt", "r");
    if(fp == NULL) return 0;
    Data_type temp;
    int find = 0;
    while(fscanf(fp, "%d %s %d %s %f", &temp.id, temp.name, &temp.age, temp.department, &temp.salary) == 5)
    {
        if(temp.id == emp_id)
        {
            strcpy(out_name, temp.name);
            find = 1;
            break;
        }
    }
    fclose(fp);
    return find;
}