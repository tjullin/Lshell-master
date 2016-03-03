#include "lshell.h"
extern int cmd_group;
extern int cmd_command;
const char PIPE_FILE[]="pipe";
int fd[2],l,r;

void run_shell (  )
{
    int i,j;
    fd[0] = open ( PIPE_FILE , O_CREAT|O_RDONLY , 0666 );//作为从管道中读出数据的端口
    fd[1] = open ( PIPE_FILE , O_CREAT|O_WRONLY|O_TRUNC , 0666 );//作为向管道中写入数据的端口
    for ( i = 0 ; i < cnt_group ; i++ )//遍历所有的命令组
    {
        l = group[i].first;
        r = group[i].last;
        int pid = fork();//创建一个子线程执行命令
	//fork会有两个返回值,在父线程中返回值为非0，子线程返回值为0
        if ( pid == 0 )
            run_command ( l , r-1 ); //执行指令(我采用的递归的方法实现)
        else 
        {
            if ( cmd[l].type & BACKGROUND );
            else 
                waitpid ( 0 , NULL , 0 );//父线程阻塞，等待子线程执行完毕(不提供后台执行的解决方案)
        }
    }
}

void run_command ( int l , int x )
{
    if ( cmd[x].type & BACKGROUND )
        printf ( "background : %d\n" , getpid() );
    //printf ( "%d %d\n" , l , x );
    pid_t pid;
    if ( x != l )//如果不是最后一条指令，那么继续创建新的进程
    {
        pid = fork();
        if ( pid==0 )//儿子进程跑前一条指令
            run_command ( l , x-1 );
        else waitpid ( 0 , NULL , 0 );//阻塞父亲进程等待儿子进程
    }
    //printf ( "where am i %d\n" , x );
    //根据管道重定向输入输出a
    if ( cmd[x].input != NULL )
    {
        int temp = open ( cmd[x].input , O_CREAT|O_RDONLY , 0666 );
        dup2 ( temp , fileno(stdin));
    }
    if ( cmd[x].output != NULL )
    {
        if ( cmd[x].type & OUT_REDIRECT )
        {
            int temp = open ( cmd[x].output , O_CREAT|O_TRUNC|O_WRONLY , 0666 );
            dup2 ( temp , fileno(stdout));
        }
        if ( cmd[x].type & OUT_REDIRECT_APPEND )
        {
            int temp = open ( cmd[x].output , O_CREAT|O_WRONLY|O_APPEND , 0666 );
            dup2 ( temp , fileno(stdout));
        }
    }
    if ( x != l ) dup2 ( fd[0] , fileno(stdin) );
    if ( x != r-1 ) dup2 ( fd[1] , fileno(stdout) );//执行命令
    execvp ( cmd[x].cmd , cmd[x].param );
}

int check ( char* buf )
{
    if ( strcmp( buf , "cd" ) )
        return 1;
    if ( strcmp( buf , "quit" ) )
        return 1;
    if ( strcmp( buf , "exit" ) )
        return 1;
    if ( strcmp( buf , "about") )
        return 1;
    return 0;
}

int main ( )
{
    while ( TRUE )
    {
        cnt_cmd = cnt_group = 0;
        make_prompt ( prompt );
        read_command ( buf , prompt );
        if ( check(buf) )
            run_builtin_command (cmd[0].cmd , cmd[0].param );
        run_shell();
    }
}

