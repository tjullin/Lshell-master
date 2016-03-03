#include "lshell.h"
/*************************************
uid_t getuid();//获得当期用户的用户id
void getcwd( char* , int ) //获取但你当前工作目录
passwd* getwuid ( uid_t id )//通过用户id获得用户信息
*************************************/

#define MAX_LEN 1024

void make_prompt ( char* prompt )
{
    char host_name[MAX_LEN];//用户信息
    char path_name[MAX_LEN];//当前工作的目录
    struct passwd* pwd = getpwuid (getuid());//通过getpwuid函数借助用户id获得用户信息
    getcwd ( path_name , MAX_LEN );//获得当前的工作目录
    if ( gethostname ( host_name , MAX_LEN ))//如果没有获得当前的用户信息，用unknown替代
        strcpy ( host_name , "unknown" );
    //如果当前工作的目录是当前用户的根目录的上层，那么直接打印
    if ( strlen ( path_name ) < strlen ( pwd->pw_dir ) || strncmp(path_name , pwd->pw_dir , strlen ( pwd->pw_dir)))
        sprintf ( prompt , "[tjullin-shell]@%s:%s:" , host_name , path_name );
    else//如果当前工作的目录是当前用户的根目录的一个子孙，那么用"~"替代当前用户的根目录
        sprintf ( prompt , "[tjullin-shell]@%s:~%s:" , host_name , path_name+strlen(pwd->pw_dir) );
    switch ( getuid() )//通过用户id判断当前用户是否为根用户
    {
        case 0:
            sprintf ( prompt+strlen(prompt) , "#" );
            break;
        default:
            sprintf ( prompt+strlen(prompt) , "$" ); 
            break;
    }
}

