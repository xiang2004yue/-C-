#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

// 管理员：调整员工基本工资
void adjust_salary()
{
    Node_list *head = read_all_employee();
    if (head == NULL)
    {
        printf("暂无员工数据！\n");
        wait_enter();
        return;
    }

    int tar_id;
    printf("请输入要调薪的员工工号：");
    scanf("%d", &tar_id);

    Node_list *p = head->next;
    while (p != NULL && p->info.id != tar_id)
    {
        p = p->next;
    }

    if (p == NULL)
    {
        printf("未找到该员工！\n");
        destory_employee(head);
        wait_enter();
        return;
    }

    printf("员工姓名：%s\n", p->info.name);
    printf("当前基本工资：%.2f\n", p->info.salary);
    printf("请输入新的基本工资：");
    scanf("%f", &p->info.salary);
    FILE *fp = fopen("../data/employee.txt", "w");
    if (fp == NULL)
    {
        perror("保存员工工资失败");
        destory_employee(head);
        wait_enter();
        return;
    }
    Node_list *cur = head->next;
    while (cur != NULL)
    {
        fprintf(fp, "%d %s %d %s %.2f\n",cur->info.id,cur->info.name,cur->info.age,cur->info.department,cur->info.salary);
        cur = cur->next;
    }
    fclose(fp);
    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("基本工资调整成功！\n");
    RESET_COLOR();
    destory_employee(head);
    wait_enter();
}

static int count_work_days(char *name, int year, int month)
{
    FILE *fp = fopen("../data/attendance.txt", "r");
    if (fp == NULL)
    {
        return 0;
    }

    char buf[512];
    int count = 0;
    int day_flag[32] = {0}; 
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        char file_name[20];
        char type[20];
        int file_year, file_month, file_day;
        char time_str[20];
        char a[10],b[10];

        if (sscanf(buf, "%s %s %d-%d-%d %s",file_name,type,&file_year,&file_month,&file_day,time_str) != 6)
        {
            continue;
        }

        if (strcmp(file_name, name) != 0)
        {
            continue;
        }

        if (file_year != year || file_month != month)
        {
            continue;
        }

        if(file_day < 1 || file_day > 31)
        {
            continue;
        }

        if (strcmp(type, "上班") != 0)
        {
            continue;
        }
        if (day_flag[file_day] == 0)
        {
            count++;
            day_flag[file_day] = 1;
        }

    }
    fclose(fp);
    return count;
}

// 统计员工某月：早退天数 + 加班总小时数
static void count_early_overtime(char *name, int year, int month, int *early_days, float *overtime_hours)
{
    FILE *fp = fopen("../data/attendance.txt", "r");
    if (fp == NULL)
    {
        *early_days = 0;
        *overtime_hours = 0;
        return;
    }

    // 按天记录上下班时间：每天最早上班、最晚下班
    struct {
        int day;
        int on_duty_min;  // 上班时间，转为当天分钟数
        int off_duty_min; // 下班时间，转为当天分钟数
        int has_on;
        int has_off;
    } day_rec[32] = {0};

    char buf[512];
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        char file_name[20], type[20];
        int y, m, d, hour, min, sec;
        if (sscanf(buf, "%s %s %d-%d-%d %d:%d:%d",
                   file_name, type, &y, &m, &d, &hour, &min, &sec) != 8)
        {
            continue;
        }
        if (strcmp(file_name, name) != 0) continue;
        if (y != year || m != month) continue;
        if (d < 1 || d > 31) continue;

        int total_min = hour * 60 + min;
        if (strcmp(type, "上班") == 0)
        {
            if (!day_rec[d].has_on || total_min < day_rec[d].on_duty_min)
            {
                day_rec[d].on_duty_min = total_min;
                day_rec[d].has_on = 1;
                day_rec[d].day = d;
            }
        }
        else if (strcmp(type, "下班") == 0)
        {
            if (!day_rec[d].has_off || total_min > day_rec[d].off_duty_min)
            {
                day_rec[d].off_duty_min = total_min;
                day_rec[d].has_off = 1;
                day_rec[d].day = d;
            }
        }
    }
    fclose(fp);

    int early = 0;
    float ot_hours = 0;
    const int standard_min = 8 * 60; // 标准8小时=480分钟

    for (int i = 1; i <= 31; i++)
    {
        if (day_rec[i].has_on && day_rec[i].has_off)
        {
            int work_min = day_rec[i].off_duty_min - day_rec[i].on_duty_min;
            if (work_min <= 0) continue;

            if (work_min < standard_min)
            {
                early++; // 不足8小时算早退，扣50
            }
            else if (work_min > standard_min)
            {
                int ot_min = work_min - standard_min;
                // 不足半小时不加，满半小时算1小时
                ot_hours += (float)((ot_min + 30) / 60);
            }
        }
    }

    *early_days = early;
    *overtime_hours = ot_hours;
}
void generate_all_salary()
{
    int year, month;
    printf("请输入生成工资条的年份：");
    scanf("%d", &year);
    printf("请输入生成工资条的月份：");
    scanf("%d", &month);

    Node_list *head = read_all_employee();
    if (head == NULL)
    {
        wait_enter();
        return;
    }

    FILE *fp = fopen("../data/salary.txt", "a");
    if (fp == NULL)
    {
        perror("工资文件打开失败");
        destory_employee(head);
        wait_enter();
        return;
    }

    system("cls");
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("==================== %d年%d月 全员工资条 ====================\n", year, month);
    printf("%-6s%-10s%-10s%-12s%-10s%-10s%-10s%-10s\n",
           "工号", "姓名", "部门", "基本工资", "出勤天", "早退扣", "加班补", "实发工资");
    printf("--------------------------------------------------------------------------------\n");
    RESET_COLOR();

    Node_list *cur = head->next;
    while (cur != NULL)
    {
        int work_days = count_work_days(cur->info.name, year, month);
        int early_days = 0;
        float overtime_hours = 0;
        count_early_overtime(cur->info.name, year, month, &early_days, &overtime_hours);

        float base = cur->info.salary;
        float daily = base / BASE_WORK_DAYS;
        float real = base - daily * (BASE_WORK_DAYS - work_days);

        if (work_days >= BASE_WORK_DAYS)
        {
            real += FULL_ATTENDANCE_BONUS;
        }

        float min_salary = base * 0.2f;
        if (real < min_salary)
        {
            real = min_salary;
        }

        // 早退扣款、加班补贴计算
        float early_deduct = early_days * 50.0f;
        float overtime_bonus = overtime_hours * 50.0f;
        real = real - early_deduct + overtime_bonus;

        printf("%-6d%-10s%-10s%-12.2f%-10d%-10.2f%-10.2f%-10.2f\n",
               cur->info.id, cur->info.name, cur->info.department,
               base, work_days, early_deduct, overtime_bonus, real);

        fprintf(fp, "%d %d %d %s %s %.2f %d %.2f %.2f %.2f\n",
                year, month, cur->info.id, cur->info.name, cur->info.department,
                base, work_days, real, early_deduct, overtime_bonus);

        cur = cur->next;
    }
    printf("================================================================================\n");
    fclose(fp);
    destory_employee(head);
    wait_enter();
}
void show_my_salary()
{
    char name[20];
    FILE *tmp = fopen("../data/temporary_buffer.txt", "r");
    if (tmp == NULL||match_myself()==0)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("未检测到登录信息！\n");
        RESET_COLOR();
        Sleep(1500);
        return;
    }
    fscanf(tmp, "%s", name);
    fclose(tmp);

    FILE *fp = fopen("../data/salary.txt", "r");
    if (fp == NULL)
    {
        printf("暂无工资条记录！\n");
        wait_enter();
        return;
    }

    SalaryRecord rec;
    int has = 0;
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("==================== 我的工资条 ====================\n");
    printf("%-10s%-10s%-10s%-10s%-10s%-10s\n",
           "年月", "基本工资", "出勤天", "早退扣", "加班补", "实发工资");
    printf("------------------------------------------------------------\n");
    while (fscanf(fp, "%d %d %d %s %s %f %d %f %f %f",
                  &rec.year, &rec.month, &rec.id, rec.name, rec.department,
                  &rec.base_salary, &rec.work_days, &rec.real_salary,
                  &rec.early_deduct, &rec.overtime_bonus) == 10)
    {
        if (strcmp(rec.name, name) == 0)
        {
            has = 1;
            char buf[20];
            sprintf(buf, "%d年%d月", rec.year, rec.month);
            printf("%-10s%-10.2f%-10d%-10.2f%-10.2f%-10.2f\n",
                   buf, rec.base_salary, rec.work_days,
                   rec.early_deduct, rec.overtime_bonus, rec.real_salary);
        }
    }
    if (!has)
    {
        printf("暂无你的工资条记录\n");
    }
    printf("============================================================\n");
    RESET_COLOR();
    fclose(fp);
    wait_enter();
}

void admin_salary_menu()
{
    int op;
    while (1)
    {
        SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        system("cls");
        printf("===== 工资管理 =====\n");
        printf("1. 调整员工基本工资\n");
        printf("2. 生成并打印当月全员工资条\n");
        printf("3. 调整员工实发工资\n");
        printf("4. 返回上一级\n");
        RESET_COLOR();
        printf("请选择：");

        if (scanf("%d", &op) != 1)
        {
            while (getchar() != '\n');
            printf("请输入数字选项！\n");
            Sleep(800);
            continue;
        }

        if (op == 1)
        {
            system("cls");
            adjust_salary();
        }
        else if (op == 2)
        {
            system("cls");
            generate_all_salary();
        }
        else if (op == 3)
        {
            system("cls");
            adjust_salary_record();
        }
        else if (op == 4)
        {
            break;
        }
        else
        {
            SET_COLOR(FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("无效选项！\n");
            RESET_COLOR();
            Sleep(800);
        }
    }
}
static SalaryNode* read_all_salary()
{
    SalaryNode *head = (SalaryNode*)malloc(sizeof(SalaryNode));
    if(head == NULL)
    {
        perror("初始化工资链表失败");
        return NULL;
    }
    head->next = NULL;
    FILE *fp = fopen("../data/salary.txt", "r");
    if(fp == NULL)
    {
        return head;
    }
    SalaryRecord rec;
    SalaryNode *tail = head;
    while(fscanf(fp, "%d %d %d %s %s %f %d %f %f %f",
                 &rec.year, &rec.month, &rec.id, rec.name, rec.department,
                 &rec.base_salary, &rec.work_days, &rec.real_salary,
                 &rec.early_deduct, &rec.overtime_bonus) == 10)
    {
        SalaryNode *new_node = (SalaryNode*)malloc(sizeof(SalaryNode));
        if(new_node == NULL) continue;
        new_node->data = rec;
        new_node->next = NULL;
        tail->next = new_node;
        tail = new_node;
    }
    fclose(fp);
    return head;
}

static void destory_salary_list(SalaryNode *head)
{
    if(head == NULL) return;
    SalaryNode *cur = head, *tmp;
    while(cur != NULL)
    {
        tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}

static void write_all_salary(SalaryNode *head)
{
    FILE *fp = fopen("../data/salary.txt", "w");
    if(fp == NULL)
    {
        perror("写入工资文件失败");
        return;
    }
    SalaryNode *cur = head->next;
    while(cur != NULL)
    {
        fprintf(fp, "%d %d %d %s %s %.2f %d %.2f %.2f %.2f\n",
                cur->data.year, cur->data.month, cur->data.id,
                cur->data.name, cur->data.department,
                cur->data.base_salary, cur->data.work_days,
                cur->data.real_salary, cur->data.early_deduct, cur->data.overtime_bonus);
        cur = cur->next;
    }
    fclose(fp);
}

void adjust_salary_record()
{
    // 自动获取系统当前年月
    time_t t;
    struct tm tm_now;
    time(&t);
    localtime_s(&tm_now, &t);
    int year = tm_now.tm_year + 1900;
    int month = tm_now.tm_mon + 1;

    SalaryNode *head = read_all_salary();
    if(head == NULL)
    {
        wait_enter();
        return;
    }

    int tar_id;
    printf("===== 调整当月工资 =====\n");
    printf("当前结算月份：%d年%d月\n", year, month);
    printf("请输入员工工号：");
    scanf("%d", &tar_id);

    // 按当月+工号定位工资记录
    SalaryNode *find = head->next;
    while(find != NULL)
    {
        if(find->data.year == year 
        && find->data.month == month 
        && find->data.id == tar_id)
        {
            break;
        }
        find = find->next;
    }

    if(find == NULL)
    {
        printf("未找到该员工当月工资记录！\n");
        printf("请先执行【生成并打印当月全员工资条】后再调整。\n");
        destory_salary_list(head);
        wait_enter();
        return;
    }

    system("cls");
    SET_COLOR(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("===== 当月工资记录 =====\n");
    printf("年月：%d年%d月\n", year, month);
    printf("工号：%d\n", find->data.id);
    printf("姓名：%s\n", find->data.name);
    printf("部门：%s\n", find->data.department);
    printf("基本工资：%.2f\n", find->data.base_salary);
    printf("出勤天数：%d\n", find->data.work_days);
    printf("早退扣款：%.2f\n", find->data.early_deduct);
    printf("加班补贴：%.2f\n", find->data.overtime_bonus);
    printf("当前实发：%.2f\n", find->data.real_salary);
    printf("------------------------\n");
    RESET_COLOR();
    float new_salary;
    printf("请输入调整后的实发工资：");
    while(scanf("%f", &new_salary) != 1)
    {
        while(getchar() != '\n');
        printf("输入无效，请输入数字：");
    }

    find->data.real_salary = new_salary;
    write_all_salary(head);
    SET_COLOR(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("当月实发工资调整成功！\n");
    RESET_COLOR();
    destory_salary_list(head);
    wait_enter();
}