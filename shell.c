#include "major2.h"

#define COLOR_RED     "\x1b[31m" // #315
#define COLOR_GREEN   "\x1b[32m" // #426
#define COLOR_YELLOW  "\x1b[33m" // #668
#define COLOR_BLUE    "\x1b[34m" // #424
#define COLOR_MAGENTA "\x1b[35m" // #733
#define COLOR_ACYAN    "\x1b[36m" // #427
#define RESET_COLOR   "\x1b[0m"

static int running = 1;
char* path;
pid_t pid; // pid to check if we are child process or parent
char* username = "user";
char originalDir[1024];
char* currentDirectory;
char* dirColor = COLOR_ACYAN; // Starting off as blue
char* userColor = COLOR_GREEN; // Starting off as green

void SIGHandler(int sig)
{
        switch(sig)
        {
        case SIGINT:
                running = sig;
                break;
        case SIGTSTP:
                running = sig;
                break;
        case SIGCHLD:
                waitpid(-1, NULL, WNOHANG);
                break;
        default:
                printf("Cannot handle Signal (%d)", sig);
                break;
        }
}

int commandToInt(char* s){
        int sum = 0;
        for (int i = 0; i < sizeof(s); i++) {
                sum+=(int)(s[i]);
                if ((int)(s[i]) == 0)
                        break;
        }
        return sum;
}

void changeColor(char** args)
{
        if(args[1] == NULL) // Checking for arugments
        {
                printf("Usage: color [user/directory] [color]\n\n");
                return;
        }

        // Converting the command to an int
        int argument1 = commandToInt(args[1]);

        // Selecting if the second argument is user or directory or list
        if(argument1 == 444 && args[2] == NULL) // Lists all possible color combinations
        {
                printf("Available Colors:\n");
                printf("%sRed\n", COLOR_RED);
                printf("%sYellow\n", COLOR_YELLOW);
                printf("%sGreen\n", COLOR_GREEN);
                printf("%sCyan\n", COLOR_ACYAN);
                printf("%sBlue\n", COLOR_BLUE);
                printf("%sMagenta\n\n", COLOR_MAGENTA);
        }
        else if(argument1 == 447 && args[2] != NULL) // Changes the user color
        {
                int temp1 = commandToInt(args[2]);
                switch (temp1)
                {
                case 315: // red
                case 283: // Red
                        userColor = COLOR_RED;
                        break;
                case 668: // yellow
                case 636: // Yellow
                        userColor = COLOR_YELLOW;
                        break;
                case 529: // green
                case 497: // Green
                        userColor = COLOR_GREEN;
                        break;
                case 427: // cyan
                case 395: // Cyan
                        userColor = COLOR_ACYAN;
                        break;
                case 424: // blue
                case 392: // Blue
                        userColor = COLOR_BLUE;
                        break;
                case 733: // magenta
                case 701: // Magenta
                        userColor = COLOR_MAGENTA;
                        break;
                default:
                        printf("%s: Color Not Found\n", args[2]);
                };
        }
        else if (argument1 == 860 && args[2] != NULL) // Changes the directory color
        {
                int temp2 = commandToInt(args[2]);
                switch (temp2)
                {
                case 315: // red
                case 283: // Red
                        dirColor = COLOR_RED;
                        break;
                case 668: // yellow
                case 636: // Yellow
                        dirColor = COLOR_YELLOW;
                        break;
                case 529: // green
                case 497: // Green
                        dirColor = COLOR_GREEN;
                        break;
                case 427: // cyan
                case 395: // Cyan
                        dirColor = COLOR_ACYAN;
                        break;
                case 424: // blue
                case 392: // Blue
                        dirColor = COLOR_BLUE;
                        break;
                case 733: // magenta
                case 701: // Magenta
                        dirColor = COLOR_MAGENTA;
                        break;
                default:
                        printf("%s: Color Not Found\n", args[2]);
                };
        }
        else
                printf("Usage: color [user/directory] [color]\n\n");

}
void printPrompt(char* user, char* curDir)
{
        printf("%s%s", userColor, user); // Setting color of the user prompt
        printf("%s@", RESET_COLOR); // Resetting color of the '@'
        printf("%s%s", dirColor, curDir); // Setting color of the directory prompt
        printf("%s$", RESET_COLOR); // Resetting color of '$' and everything else after
}

void shellRun(char* command)
{
        int isOnlyWhitespace; // boolean flag to signal if the command is just whitespace
        char* args[500] = { NULL };

        for(int i = 0; i < strlen(command) - 1; i++) // Looop to check if the command entered is just whitespaces
        {
                if(command[i] != ' ') // If we encounter a character that is not whitespace, break from this loop
                {
                        isOnlyWhitespace = 1;
                        break;
                }
                else // otherwise, signal the command is empty
                {
                        isOnlyWhitespace = 0;
                }
        }

        if(isOnlyWhitespace == 0) // if the command is empty, move to next loop iteration
        {
                return;
        }



        if(!(strcmp(command, "") == 0))
        {
                char* token; // Token for splitting the command into its pieces
                int i = 0; // iteration variable
                token = strtok(command, " "); // Get the first item of the command inside token
                while(token != NULL)
                {
                        args[i] = token; // Set the index i of args to be the current token

                        token = strtok(NULL, " "); // Go to the next token inside the command string
                        i++;
                }

                // Switch Statment to catch commands that need to apply to parent
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
                        if (args[1] == NULL)
                                myShellInteract();
                        else if (args[2] == NULL)
                                myShellScript(args[1]);
                        else
                                printf("\nInvalid Number of Arguments");
                        break;
                case 522: // alias
                        aliasNew(args);
                        break;
                case 543: // Change color
                        changeColor(args);
                        break;
                case 864: // Change Username
                        if(args[1] != NULL && args[2] == NULL)
                                username = strdup(args[1]);
                        else
                                printf("Usage: username [user inputed name]\n");
                        break;
                case 866: // newshell
                        args[0] = "./major2";
                        //  printf("Sending %s\n", originalDir);
                        changeDirectory(originalDir);
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
                                if(currentDirectory != originalDir)
                                {
                                        changeDirectory(currentDirectory);
                                }
                        }
                        else
                        {
                                printf("Fork Error\n");
                        }
                }
        }
}

int shell ()
{
        // Install signal handlers
        signal(SIGINT, SIGHandler);
        signal(SIGTSTP, SIGHandler);
        signal(SIGCHLD, SIGHandler);


        char command[512]; // the command to parse
        int isOnlyWhitespace; // boolean flag to signal if the command is just whitespace
        int wordCount = 0;
        int buffer = 512; // buffer to ensure we don't overflow the command cstring
        char directoryBuffer[500];
        getcwd(originalDir, 1024);

        while( running ) // While the command entered is not quit continue running
        {
                char* args[500] = { NULL };
                // Gets the current working directory
                currentDirectory = getcwd(directoryBuffer, 500);
                printPrompt(username, currentDirectory);
                if(fgets(command, buffer, stdin) == NULL)
                {
                        exit(1);
                } // Get the command from the user and see if cntrlD is entered, end the shell

                if(command[strlen(command) -1] != '\n') // Check if input is over buffer size
                {
                        fprintf(stderr, "Error: Command Too Large\n");
                        while(getchar() != '\n')
                        {
                                // DO nothing to clear input buffer
                        }
                        continue;
                }

                char* token1;
                int i = 0;
                token1 = strtok(command, ";");
                while(token1 != NULL)
                {
                        args[i] = token1;
                        token1 = strtok(NULL, ";");
                        i++;
                }

                // Saving inputted command into history
                saveCommand(args);

                int counter;

                int alias_flag = 0;
                char temp[30];
                for(int i = 0; args[i] != NULL; i++)
                {
                        for(int j = 0; j < 50; j++)
                        {
                                if(alias_name[j] != NULL && strcmp(args[i], alias_name[j]) == 0)
                                {
                                        strcpy(temp, real_name[j]);
                                        args[i] = temp;
                                        break;
                                }
                        }
                }

                for (int i = 0; args[i] != NULL; i++)
                {
                        if(args[i][strlen(args[i]) -1] == '\n')
                                args[i][strlen(args[i])-1] = '\0'; // strip the new character \n off the end of command
                        if(strcmp(args[i], " ") != 0)
                        {
                                counter = pipeCheck(args[i]);
                                if(counter == 0)
                                        shellRun(args[i]);
                                else if(counter > 0)
                                        pipeRun(args[i], counter);
                                else if (counter == -1)
                                        printf("%s: Pipe and I/O redirection can NOT be handled together\n", args[i]);

                        }
                }
        }
        return 0;
}
