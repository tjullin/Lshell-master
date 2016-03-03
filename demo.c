#include "lshell.h"
extern int cmd_group;
extern int cmd_command;

void run_shell ( )
{
    int pipe_fd[2];
    int status;
    pipe(pipe_fd);
    pid_t child1,child2;
    if ((child1=fork())!=0)//父进程
    {
        if ( (child2 = fork()) == 0 )//子进程
        {
            close ( pipe_fd[1] );
            close ( fileno ( stdin ) );
            dup2 ( pipe_fd[0] , fileno(stdin));
            close ( pipe_fd[0] );
            execvp ( cmd[1].cmd , cmd[1].param );
        }
        else 
        {
            close ( pipe_fd[0]);
            close ( pipe_fd[1]);
            waitpid ( child2 , &status , 0 );
        }
        waitpid ( child1 , &status , 0 );
    }
    else
    {
        printf ( "subshell 3cmd %d\n" , getpid() );
        close ( pipe_fd[0] );
        close ( fileno(stdout));
        dup2 ( pipe_fd[1] , fileno(stdout));

        close ( pipe_fd[1] );
        execvp ( cmd[0].cmd , cmd[0].param );
    }
}

int main ( )
{
    //while ( TRUE )
    {
        cnt_cmd = cnt_group = 0;
        make_prompt ( prompt );
        read_command ( buf , prompt );
        printf ( "cnt_cmd : %d\n" , cnt_cmd );
        run_shell ( );   
        printf ( "%d : GREAT YES\n" , getpid() );
    }
}


