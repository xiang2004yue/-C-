#!/bin/bash
PROJECT_NAME="employee_manage"

# 创建目录结构
mkdir -p ${PROJECT_NAME}/{src,include,data,bin,obj,log}

# 函数：自动生成带头文件保护宏的 .h 文件
create_header() {
    local header_file=$1
    local guard_name=$(basename "$header_file" | tr '[:lower:]' '[:upper:]' | sed 's/\./_/g')_H
    cat > ${header_file} <<EOL
#ifndef ${guard_name}
#define ${guard_name}

// 【模块头文件】$(basename $header_file)

#endif // ${guard_name}
EOL
}

# ===================== 创建头文件 =====================
create_header "${PROJECT_NAME}/include/employee.h"    # 员工结构体、员工管理接口
create_header "${PROJECT_NAME}/include/attendance.h" # 考勤相关结构体与函数
create_header "${PROJECT_NAME}/include/salary.h"     # 工资计算模块
create_header "${PROJECT_NAME}/include/user.h"       # 用户登录、管理员权限
create_header "${PROJECT_NAME}/include/utils.h"      # 通用工具函数

# ===================== 创建源文件 =====================
touch ${PROJECT_NAME}/src/main.c          # 程序入口
touch ${PROJECT_NAME}/src/employee.c      # 员工信息增删改查
touch ${PROJECT_NAME}/src/attendance.c    # 考勤打卡、请假、统计
touch ${PROJECT_NAME}/src/salary.c        # 工资核算、工资报表
touch ${PROJECT_NAME}/src/user.c          # 用户注册登录、密码管理
touch ${PROJECT_NAME}/src/utils.c         # 文件读写、字符串工具

# ===================== 创建数据文件 & 文档 =====================
touch ${PROJECT_NAME}/data/employees.txt    # 员工信息持久化
touch ${PROJECT_NAME}/data/attendance.txt    # 考勤记录
touch ${PROJECT_NAME}/data/salary.txt        # 工资发放记录
touch ${PROJECT_NAME}/data/user.txt          # 账号密码存储
touch ${PROJECT_NAME}/log/runtime.log        # 运行日志
touch ${PROJECT_NAME}/README.md

# 函数：自动在.c文件写入基础头文件引入
add_include_to_c() {
    local c_file=$1
    cat > ${c_file} <<EOL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

EOL
}

# 给所有源码写入基础引用
add_include_to_c "${PROJECT_NAME}/src/main.c"
add_include_to_c "${PROJECT_NAME}/src/employee.c"
add_include_to_c "${PROJECT_NAME}/src/attendance.c"
add_include_to_c "${PROJECT_NAME}/src/salary.c"
add_include_to_c "${PROJECT_NAME}/src/user.c"
add_include_to_c "${PROJECT_NAME}/src/utils.c"

# 创建统一汇总头文件 head.h
cat > ${PROJECT_NAME}/include/head.h <<EOL
#ifndef _HEAD_H_
#define _HEAD_H_

// 导入所有模块头文件
#include "employee.h"
#include "attendance.h"
#include "salary.h"
#include "user.h"
#include "utils.h"

// 全局宏定义
#define LOG_FILE "log/runtime.log"
#define EMP_FILE "data/employees.txt"
#define ATT_FILE "data/attendance.txt"
#define SAL_FILE "data/salary.txt"
#define USER_FILE "data/user.txt"

#endif // _HEAD_H_
EOL

# 自动生成Makefile（MinGW/Linux通用）
cat > ${PROJECT_NAME}/Makefile <<EOL
CC = gcc
CFLAGS = -Wall -g -I./include
SRC = \$(wildcard src/*.c)
OBJ = \$(patsubst src/%.c,obj/%.o,\$(SRC))
TARGET = bin/employee_manage

all: \$(TARGET)

\$(TARGET): \$(OBJ)
	\$(CC) \$(CFLAGS) -o \$@ \$(OBJ)

obj/%.o: src/%.c
	\$(CC) \$(CFLAGS) -c \$< -o \$@

clean:
	rm -f obj/*.o \$(TARGET)
EOL

echo "员工考勤工资管理系统项目框架创建完成！"
echo "项目目录：./${PROJECT_NAME}"
echo "编译命令：cd ${PROJECT_NAME} && make"