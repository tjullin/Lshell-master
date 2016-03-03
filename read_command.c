#include "lshell.h"
extern int cnt_cmd;
extern int cnt_group;
extern int islegal;
#define BUFSIZE 1024

void clear_cmd ( void )
{
    int i;
    for ( i = 0 ; i < cnt_cmd ; i++ )
    {
        cmd[i].type = 0;
        if ( cmd[i].param != NULL )
            free ( cmd[i].param );
    }
    cnt_cmd = cnt_group = 0;
}

int init_cmd ( void )
{
    struct command_info* ptr = &cmd[cnt_cmd];
    ptr->input = NULL;
    ptr->output = NULL;
    ptr->cmd = NULL;
    ptr->param = NULL;
    return cnt_cmd++;
}

//遍历字符串以";"为分隔得到字符串组，用0取代;代表字符串结束，实现字符串的划分
void parse_token ( char* buf )
{
    int n = strlen ( buf ),i,j=0;
    n--;
    buf[n] = 0;
    for ( i = 0 ; i < n ; i++ )
        if ( buf[i] == ';'|| i == n-1 )
        {
            if ( buf[i] == ';' )
                buf[i] = 0;
            parse_group ( buf+j );
            j = i+1;
        }
}

//以“｜”为分隔得到字符串组，用0取代｜的位置代表字符串结束，实现字符串的划分
//mode代表得到的单条指令在组中的位置，只有两端的不通过管道的传递信息
//1代表中间位置
void parse_group ( char* buf )
{
    int n = strlen ( buf ),i,j=0;
    int x = cnt_cmd;
    for ( i = 0 ; i < n ; i++ )
        if ( buf[i] == '|' || i == n-1 )
        {
            int mode = 0;
            if ( buf[i] == '|' )
            {
                buf[i] = 0;
                mode = 1;
            }
            parse_command ( buf+j , mode );
            j = i+1;
        }
    group[cnt_group].first = x;//对指令进行分组
    group[cnt_group].last = cnt_cmd;
    cnt_group++;
}

//parse_command分析单条指令
void parse_command ( char* buf , int mode )
{
    int n = strlen ( buf ),i=0,j=0;
    while ( buf[i] ==' '|| buf[i] == '\t' ) i++,j++;//去掉没有用的空格
    int id = init_cmd ();//从内存池中取出一个命令的槽
    if ( mode == 1 ) cmd[id].type |= PIPE;//如果命令位于中间位置，那么设置为属性包含PIPE属性
    char* segment[MAX_ARGS];//存储划分段的位置的指针
    int x=0;
    for ( ; i < n ; i++ )
        if ( buf[i] == ' ' || i == n-1 )
        {
            if ( buf[i] == ' ' ) buf[i] = 0;
            segment[x++] = buf+j;
            j = i+1;
            while ( buf[j] == ' ' || buf[j] == '\t' )
                j++,i++;
        }
    //分析指令，查找文件的重定向和向参数列表中添加参数
    int temp = 0;
    cmd[id].cmd = segment[0];
    cmd[id].param = malloc(sizeof (char*)*(MAX_ARGS+2) );
    if ( x > 0 )
        cmd[id].param[temp++] = segment[0];
    for ( i = 1; i < x ; i++ )
    {
        int flag = 1;
        if ( strlen(segment[i]) == 1 )
        {
            if ( segment[i][0] == '<' )
            {
               flag = 0;
               cmd[id].type |= IN_REDIRECT; 
               cmd[id].input = segment[i+1];
               i++;
            }
            else if ( segment[i][0] == '>' )
            {
               flag = 0;
               cmd[id].type |= OUT_REDIRECT;
               cmd[id].output = segment[i+1];
               i++;
            }
            else if ( segment[i][0] == '&' )
            {
                flag = 0;
                cmd[id].type |= BACKGROUND;
            }
        }
        if ( strlen ( segment[i] ) == 2 )
        {
            if ( strcmp ( segment[i] , "<<" ) == 0 )
            {
                flag = 0;
                cmd[id].type |= IN_REDIRECT;
                cmd[id].input = segment[i+1];
                i++;
            }
            else if ( strcmp ( segment[i] , ">>" ) == 0 )
            {
                flag = 0;
                cmd[id].type |= OUT_REDIRECT_APPEND;
                cmd[id].output = segment[i+1];
                i++;
            } 
        }
        if ( flag )
        {
            cmd[id].param[temp++] = segment[i];
        }
    }
}

void read_command ( char* buf , char* prompt )
{
    printf ( "%s " , prompt );
    char* flag = fgets ( buf , BUFSIZE , stdin);
    //printf ( "\n%s\n" , buf );
    if ( flag == NULL )
    {
        puts("");
        exit(0);
    }
    int len = strlen ( buf );
    clear_cmd ();
    parse_token( buf );
}

/*int main ( )
{
    char buf[BUFSIZE];
    read_command ( buf , "hello word!! : " );
#define DEBUG
#ifdef DEBUG
    if ( cmd[0].cmd != NULL ) printf ( "%s\n" , cmd[0].cmd );
    if ( cmd[0].input != NULL ) printf ( "%s\n" , cmd[0].input );
    if ( cmd[0].output != NULL )  printf ( "%s\n" , cmd[0].output );
    printf ( "%s\n" , cmd[0].param[0]);
    printf ( "%d %d\n" , cnt_cmd , cnt_group );
#endif
}*/

