#include "major2.h"

#define HOME "/home"

char* setUpChangeDir(char** path)
{
    char* rPath;
    rPath = malloc(500);

    for(int i = 1; i < 500; i++)
    {
        if(path[i] == NULL)
            break;

        strcat(rPath, path[i]);
        if(path[i+1] == NULL)
            break;
        
        strcat(rPath, " ");
    }

    return rPath;
}

void changeDirectory(char* path)
{
    if(strcmp(path, "") == 0)
    {
        chdir(HOME);
    }
    else
    {
        if(chdir(path) == -1)
        {
            printf("cd: %s: does not exist\n", path);
        }
    }
};