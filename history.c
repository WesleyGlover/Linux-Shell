#include "major2.h"

char *history[20] = {0};
int position = 0;

void saveCommand(char** argv)
{
        // Removing newline character from the inputted command
        for (int i = 0; argv[i] != NULL; i++)
        {
                if(argv[i][strlen(argv[i]) -1] == '\n')
                        argv[i][strlen(argv[i])-1] = '\0'; // strip the new character \n off the end of command
        }


        // Checing if history is full
        if (position == 20)
        {
                // Shifting history array
                for (int i = 0; i < 19; i++)
                {
                        strcpy(history[i], history[i+1]);
                }
        }

        // Allocating memory to store the command
        if (position < 20)
                history[position] = malloc(sizeof(char*)*500);


        // Decrementing position to save last command in last spot of history
        if (position == 20)
                position--;

        // Storing next command into history
        strcpy(history[position], argv[0]);
        for (int i = 1; i < 500; i++)
        {
                if (argv[i] == NULL)
                        break;

                strcat(history[position], " ");
                strcat(history[position], argv[i]);
        }

        // Command is stored, incrementing to next command
        if(position < 20)
                position++;
}

void myHistory(char** argv)
{
        // Printing if there are no arguments
        if (argv[1] == NULL)
        {
                // Printing out history
                printf("History:\n\n");
                for (int i = 0; i < position; i++)
                {
                        printf("[%2d]\t%-5s%s\n", i, "-", history[i]);
                }
                printf("\n");
        }
        else if (argv[1][1] == 'c') // Clearing history flag
        {
                printf("Clearing the history array\n\n");
                for (int i = 0; i < position; i++)
                {
                        history[i] = NULL;
                }
                position = 0;
        }
        else if (argv[1][1] == 'e') // Execute command from history flag
        {
                if (argv[2] == NULL) // If there was no index provided
                {
                        printf("Usage: myhistory -e [index number]\n\n");
                }
                else
                {
                        // Converting char to int for index
                        int index;
                        index = atoi(argv[2]);

                        printf("Executing command %s\n\n", history[index]);

                        // Declaring vairables needed to execute command
                        char *command;
                        command =  history[index];
                        int counter;

                        // Taking off new line character if present
                        if(command[strlen(command) -1] == '\n')
                                command[strlen(command)-1] = '\0'; // strip the new character \n off the end of command

                        // Running command
                        if(strcmp(command, " ") != 0)
                        {
                                counter = pipeCheck(command);
                                if(counter == 0)
                                        shellRun(command);
                                else if(counter > 0)
                                        pipeRun(command, counter);
                                else if (counter == -1)
                                        printf("%s: Pipe and I/O redirection cant be handled together\n", command);

                        }

                }
        }
        else // If the argument is unknown
        {
          printf("Unknown argument: \'%s\'\n", argv[1]);
        }


}
