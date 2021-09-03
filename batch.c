#include "major2.h"

pid_t pid;
char* path;

int batchRunner(char* command)
{
        char* args[500] = { NULL };
        char* token; // Token for splitting the command into its pieces
        int i = 0; // iteration variable
        token = strtok(command, " "); // Get the first item of the command inside token
        while(token != NULL)
        {
                args[i] = token; // Set the index i of args to be the current token

                token = strtok(NULL, " "); // Go to the next token inside the command string
                i++;
        }


        switch(commandToInt(args[0]))
        {
        case 199: // cd
                path = setUpChangeDir(args);
                changeDirectory(path);
                break;
        case 442: // exit
                exitAtEndOfLine();
                break;
        case 895: // myhistory
                myHistory(args);
                break;
        case 429: // path
                break;
        case 522: // alias
                aliasNew(args);
                break;
        default:
                pid = fork();
                if(pid == 0) // child
                {
                        if(tcsetpgrp(0, getpgid(0)) == -1) // Send the child program to the foreground
                                exit(0);

                        if(execvp(args[0], args) == -1) // execute the command
                        {
                                fprintf(stdout, "%s: command not found\n", args[0]); // issue error if not found
                                exit(0);
                        }
                }
                else if(pid > 0) // parent
                {
                        pause(); // Wait for SIGCHLD
                }
                else
                {
                        fprintf(stderr, "Fork Error\n");
                }
        }
        return 0;
}

int batchProcesser(char* file)
{
        // Install signal handlers
        signal(SIGINT, SIGHandler);
        signal(SIGTSTP, SIGHandler);
        signal(SIGCHLD, SIGHandler);

        FILE* batchFile; // File pointer
        batchFile = fopen(file, "r"); // Open file to read
        if (batchFile == NULL) // Checking if file opened properly
        {
                fprintf(stderr, "Error: Batch File Not Found\n");
                exit(1);
        }
        char command[500] = {0}; // Sets the command to NULL

        while(fgets(command, 500, batchFile) != NULL)
        {
                char* args[500] = { NULL }; // Sets args to NULL
                char buf[500];
                printf("%s\n", command);

                char* token1;
                int i = 0;
                token1 = strtok(command, ";");
                while(token1 != NULL)
                {
                        args[i] = token1;
                        token1 = strtok(NULL, ";");
                        i++;
                }

                // Saving command
                saveCommand(args);

                int counter;
                for (int i = 0; args[i] != NULL; i++)
                {
                        if(args[i][strlen(args[i]) -1] == '\n')
                                args[i][strlen(args[i])-1] = '\0'; // strip the new character \n off the end of command
                        if(strcmp(args[i], " ") != 0)
                        {
                                counter = pipeCheck(args[i]);
                                if(counter == 0)
                                        batchRunner(args[i]);
                                else if(counter > 0)
                                        pipeRun(args[i], counter);
                                else if (counter == -1)
                                        printf("%s: Pipe and I/O redirection can NOT be handled together\n", args[i]);
                        }
                }

        }

        fclose(batchFile);
        return 0;
}
