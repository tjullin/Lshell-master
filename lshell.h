#include <sys/stat.h>//获得文件的属性的函数库
#include <sys/types.h>//提供基本的系统数据类型，如CPU时钟周期等
#include <sys/wait.h>//调用系统的阻塞等待函数
#include <stdio.h>//C语言输入输出库
#include <stdlib.h>//C语言标准库
#include <errno.h>//定义通过错误码来汇报错误资讯宏
#include <fcntl.h>//文件操作
#include <pwd.h>//提供了passwd这个用户基本信息的结构体
#include <string.h>//C语言字符串操作库
#include <unistd.h>//类UNIX操作系统POSIX的API原句
#include <signal.h>
#define MAX_ARGS 20
#define MAX_CMD 1000
#define MAX_GRP 100
#define BUFSIZE 1024
#define BACK_SIZE 50
#define TRUE 1

//利用二进制位记录一个命令的属性（状态压缩）
#define TYPE
#ifdef TYPE
#define PIPE                 1
#define BACKGROUND           2
#define IN_REDIRECT          4
#define OUT_REDIRECT         8
#define OUT_REDIRECT_APPEND 16
#endif

void print_prompt ( char* );
//parse一族代表分析命令的函数
void parse_group ( char* );
void parse_command ( char* , int mode );
void parse_token ( char* );
//制作提示信息的函数
void make_prompt( char* );
//读取命令的函数
void read_command( char* , char* );
//执行命令的函数
void run_shell(void);
void run_command ( int,int);
//管理内存池的函数
void clear_cmd(void);
int init_cmd(void);
int cnt_cmd;
int cnt_group;
int islegal;
int cmd_id[MAX_CMD];
char buf[BUFSIZE];
char prompt[BUFSIZE];
//管理进程池的函数
void clear_back(void);
int add_back ( pid_t );
//执行内建命令的函数
char** buitin_command;
int run_builtin_command ( char* , char** );

struct command_info
{
    int type;//记录命令的属性，每个二进制位表示一种属性
    char* input;//记录输入重定向
    char* output;//记录输出重定向
    char* cmd;//记录命令
    char** param;//记录命令的参数
};

struct command_info cmd[MAX_CMD];//命令的内存池

struct command_group
{
    int first,last;//记录这个命令群的区间
};

struct command_group group[MAX_GRP];//命令群的内存池

int back_table[BACK_SIZE];//后台进程池

