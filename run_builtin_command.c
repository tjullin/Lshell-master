#include "lshell.h"

struct passwd* pwd;

int run_builtin_command ( char* cmd , char** param )
{
    pwd = getpwuid (getuid());
    char path[BUFSIZE];
    getcwd ( path , BUFSIZE );
    if ( strcmp (cmd ,"exit") == 0 || strcmp ( cmd , "quit" ) == 0 )
        exit(0);
    else if ( strcmp ( cmd , "about" ) == 0 )
    {
        printf ( "tjullin from tianjin university\n" );
        return;
    }
    else if ( strcmp ( cmd , "cd" )== 0 )
    {
        char * path=NULL;
        if ( param[1] == NULL )
        {
            chdir ( pwd->pw_dir );
        }
        if ( param[1][0] == '~' )
        {
            path = malloc ( BUFSIZE );
            strcpy(path,pwd->pw_dir);
            strncpy(path+strlen(pwd->pw_dir),param[1]+1,strlen(param[1]));
        }
        else
        {
            path = malloc (BUFSIZE);
            strcpy(path,param[1]);
        }
        if ( chdir(path)!= 0 )
            printf ( "-lshell: cd: %s:%s\n", path , strerror(errno));
        free(path);
    }
    return 0;
}
