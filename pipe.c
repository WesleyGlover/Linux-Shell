#include "major2.h"

enum { READ, WRITE };

int pipeCheck(char * command)
{
        int i = 0;
        int counter = 0;

        // Looping through the command to find any '|'
        while (command[i] != '\0')
        {
                // If '|' is found, increment the counter
                if (command[i] == '|')
                {
                        counter++;
                }

                // Leaving loop if there is > or < involved. NOT COMPATABLE
                if ((command[i] == '>' || command[i] == '<') && counter > 0)
                {
                        return -1;
                }
                i++;
        }

        // return the counter
        return counter;
}

void pipeRun(char* input, int counter)
{
        // Splitting the command up by the '|' into its own commands
        char* argvs[500] = {NULL};
        char* token1;
        int i = 0;
        token1 = strtok(input, "|");
        while(token1 != NULL)
        {
                argvs[i] = token1;
                token1 = strtok(NULL, "|");
                i++;
        }


        int isOnlyWhitespace; // boolean flag to signal if the command is just whitespace
        char* args[500] = { NULL };
        char* command;
        int status;

        // If there are two pipes needed
        if (counter == 2)
        {
                char* command1[500] = {NULL};
                char* command2[500] = {NULL};
                char* command3[500] = {NULL};
                // Creating pipes
                int fd1[2], fd2[2];
                pipe(fd1);
                pipe(fd2);

                // Looping through commands to execute
                for (i = 0; i < counter+1; i++)
                {
                        // IF end of arguments, leave loop
                        if (argvs[i] == NULL)
                                break;

                        // Save the next argument into command
                        command = argvs[i];

                        // White space checking
                        for(int j = 0; j < strlen(command) - 1; j++) // Looop to check if the command entered is just whitespaces
                        {
                                if(command[j] != ' ') // If we encounter a character that is not whitespace, break from this loop
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


                        // Splitting up words to send the command properly to the console
                        if(!(strcmp(command, "") == 0))
                        {
                                char* token; // Token for splitting the command into its pieces
                                char character;
                                int hasMark = 0;
                                int k = 0; // iteration variable
                                int l = 0;
                                int temp1 = 0;
                                char* temp2;


                                for (int l = 0; l < sizeof(command); l++)
                                {
                                        character = command[l];
                                        if(character == '\'')
                                                hasMark = 1;
                                }

                                if (hasMark == 1)
                                {
                                        token = strtok(command, " "); // Get the first item of the command inside token
                                        //  printf("Printing Command from args: ");
                                        while(token != NULL)
                                        {
                                                switch (i)
                                                {
                                                case 0:
                                                        command1[k] = token;
                                                        k++;
                                                        token = strtok(NULL, "\'\'");
                                                        command1[k] = token;
                                                        break;
                                                case 1:
                                                        command2[k] = token;
                                                        k++;
                                                        token = strtok(NULL, "\'\'");
                                                        command2[k] = token;
                                                        break;
                                                case 2:
                                                        command3[k] = token;
                                                        k++;
                                                        token = strtok(NULL, "\'\'");
                                                        command3[k] = token;
                                                        break;
                                                default:
                                                        printf("Switch Not working\n");
                                                        break;
                                                }

                                                token = strtok(NULL, "\'");
                                                k++;
                                        }// END of while(token != NULL)
                                }
                                else
                                {

                                        token = strtok(command, " "); // Get the first item of the command inside token
                                        //  printf("Printing Command from args: ");
                                        while(token != NULL)
                                        {

                                                args[k] = token;   // Set the index i of args to be the current token
                                                switch (i)
                                                {
                                                case 0:
                                                        command1[k] = token;
                                                        break;
                                                case 1:
                                                        command2[k] = token;
                                                        break;
                                                case 2:
                                                        command3[k] = token;
                                                        break;
                                                }


                                                token = strtok(NULL, " "); // Go to the next token inside the command string
                                                //      printf("%s\n", args[k]);
                                                k++;
                                        } // END of while(token != NULL)
                                } // END OF else
                        } // END of if(!(strcmp(command, "") == 0)
                } // END of for (i = 0; i < 500; i++)

                // Forking into child to execute command
                if (fork() == 0) // CHILD 1
                {
                        if(tcsetpgrp(0, getpgid(0)) == -1) // Send the child program to the foreground
                                exit(0);

                        // Making WRITE end of fd1 the standard out
                        if(dup2(fd1[WRITE], WRITE) == -1)
                        {
                                printf("Error Opening pipe for writing\n");
                                exit(0);
                        }

                        // Closing pipes created
                        close(fd1[READ]);
                        close(fd1[WRITE]);
                        close(fd2[READ]);
                        close(fd2[WRITE]);

                        // Executing command
                        execvp(command1[0], command1);
                }
                else // PARENT
                {
                        if (fork() == 0) // CHILD 2
                        {
                                if(tcsetpgrp(0, getpgid(0)) == -1) // Send the child program to the foreground
                                        exit(0);

                                // Making the READ end of fd1 standard in
                                if(dup2(fd1[READ], READ) == -1)
                                {
                                        printf("Error Opening pipe for reading\n");
                                        exit(0);
                                }

                                // Making WRITE end of fd2 the standard out
                                if(dup2(fd2[WRITE], WRITE) == -1)
                                {
                                        printf("Error Opening pipe for writing\n");
                                        exit(0);
                                }

                                // Closing pipes created
                                close(fd1[READ]);
                                close(fd1[WRITE]);
                                close(fd2[READ]);
                                close(fd2[WRITE]);

                                // Executing command
                                execvp(command2[0], command2);
                        }
                        else // PARENT
                        {
                                if(fork() == 0) // CHILD 3
                                {
                                        if(tcsetpgrp(0, getpgid(0)) == -1) // Send the child program to the foreground
                                                exit(0);

                                        // Making the READ end of fd1 standard in
                                        if(dup2(fd2[READ], READ) == -1)
                                        {
                                                printf("Error Opening pipe for reading\n");
                                                exit(0);
                                        }

                                        // Closing pipes created
                                        close(fd1[READ]);
                                        close(fd1[WRITE]);
                                        close(fd2[READ]);
                                        close(fd2[WRITE]);


                                        // Executing command
                                        execvp(command3[0], command3);
                                } // END OF CHILD 3
                        } // PARENT FROM CHILD 2
                } // PARENT FROM CHILD 1

                // Closing pipes created
                close(fd1[READ]);
                close(fd1[WRITE]);
                close(fd2[READ]);
                close(fd2[WRITE]);

                // Having parent wait for children to finish
                for (i = 0; i < counter+1; i++)
                {
                        wait(&status);
                }

        } // END of if (counter == 2)
        else if (counter == 1)
        {
          char* command1[500] = {NULL};
          char* command2[500] = {NULL};
          // Craeting pipes
          int fd1[2];
          pipe(fd1);
          // Looping through commands to execute
          for (i = 0; i < counter+1; i++)
          {
                  // IF end of arguments, leave loop
                  if (argvs[i] == NULL)
                          break;

                  // Save the next argument into command
                  command = argvs[i];

                  // White space checking
                  for(int j = 0; j < strlen(command) - 1; j++) // Looop to check if the command entered is just whitespaces
                  {
                          if(command[j] != ' ') // If we encounter a character that is not whitespace, break from this loop
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


                  // Splitting up words to send the command properly to the console
                  if(!(strcmp(command, "") == 0))
                  {
                          char* token; // Token for splitting the command into its pieces
                          char character;
                          int hasMark = 0;
                          int k = 0; // iteration variable
                          int l = 0;
                          int temp1 = 0;
                          char* temp2;


                          for (int l = 0; l < sizeof(command); l++)
                          {
                                  character = command[l];
                                  if(character == '\'')
                                          hasMark = 1;
                          }

                          if (hasMark == 1)
                          {
                                  token = strtok(command, " "); // Get the first item of the command inside token
                                  //  printf("Printing Command from args: ");
                                  while(token != NULL)
                                  {
                                          switch (i)
                                          {
                                          case 0:
                                                  command1[k] = token;
                                                  k++;
                                                  token = strtok(NULL, "\'\'");
                                                  command1[k] = token;
                                                  break;
                                          case 1:
                                                  command2[k] = token;
                                                  k++;
                                                  token = strtok(NULL, "\'\'");
                                                  command2[k] = token;
                                                  break;
                                          default:
                                                  printf("Switch Not working\n");
                                                  break;
                                          }

                                          token = strtok(NULL, "\'");
                                          k++;
                                  }// END of while(token != NULL)
                          }
                          else
                          {

                                  token = strtok(command, " "); // Get the first item of the command inside token
                                  //  printf("Printing Command from args: ");
                                  while(token != NULL)
                                  {

                                          args[k] = token;   // Set the index i of args to be the current token
                                          switch (i)
                                          {
                                          case 0:
                                                  command1[k] = token;
                                                  break;
                                          case 1:
                                                  command2[k] = token;
                                                  break;
                                          }


                                          token = strtok(NULL, " "); // Go to the next token inside the command string
                                          //      printf("%s\n", args[k]);
                                          k++;
                                  } // END of while(token != NULL)
                          } // END OF else
                  } // END of if(!(strcmp(command, "") == 0)
          } // END of for (i = 0; i < 500; i++)

          // Forking into child to execute command
          if (fork() == 0) // CHILD 1
          {
                  if(tcsetpgrp(0, getpgid(0)) == -1) // Send the child program to the foreground
                          exit(0);

                  // Making WRITE end of fd1 the standard out
                  if(dup2(fd1[WRITE], WRITE) == -1)
                  {
                          printf("Error Opening pipe for writing\n");
                          exit(0);
                  }

                  // Closing pipes created
                  close(fd1[READ]);
                  close(fd1[WRITE]);
                  // Executing command
                  execvp(command1[0], command1);
          }
          else // PARENT
          {
                  if (fork() == 0) // CHILD 2
                  {
                          if(tcsetpgrp(0, getpgid(0)) == -1) // Send the child program to the foreground
                                  exit(0);

                          // Making the READ end of fd1 standard in
                          if(dup2(fd1[READ], READ) == -1)
                          {
                                  printf("Error Opening pipe for reading\n");
                                  exit(0);
                          }


                          // Closing pipes created
                          close(fd1[READ]);
                          close(fd1[WRITE]);

                          // Executing command
                          execvp(command2[0], command2);
                  }
          } // PARENT FROM CHILD 1

          // Closing pipes created
          close(fd1[READ]);
          close(fd1[WRITE]);
          // Having parent wait for children to finish
          for (i = 0; i < counter+1; i++)
          {
                  wait(&status);
          }

        } // END of else if (counter == 1)
        else // Error of too many pipes
        {
          fprintf(stderr, "Error: Too Many Pipes. Only Compatable Up to 2 Pipes.\n");
        }
} // END of void pipeRun(char* input, int counter)
