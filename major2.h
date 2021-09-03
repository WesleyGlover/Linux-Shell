#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

  char *alias_name[50];
  char *real_name[50];

  // Function: Shell
  // Params: NULL
  // Usage: Runs the shell in interactive mode
  int shell();

  // Function: batchProcessor
  // Params: char* file
  // Usage: takes in a batch file that contains shell commands to be executed
  int batchProcesser(char* file);

  // Function: shellRun
  // Params: char*
  // Runner function of the shell
  void shellRun(char*);

  // Function: commandToInt(char*)
  // Params: char*
  // Usage: Helper function to determine built-in commands
  int commandToInt(char*);

  // Function: SIGHandler
  // Params: int sig
  // Usage: Handle signal SIG sent to the shell (I.E. cntrl-c), the parameter is the signal number
  void SIGHandler(int sig);

  // Function: CD
  // Params: char*
  // Usage: Takes in the path that we want to change the current directory
  void changeDirectory(char*);

  // Function: setUpChangeDir
  // Params: char** path
  // Usage: Takes the path and formats it to be used with the CD function
  char* setUpChangeDir(char** path);

  // Function: myHistory
  // Params: char**
  // Usage: myHistory : Prints out users history
  //        myHistory -c : Clears out users history
  //        myHistory -e [Number of Command] : runs a command saved into history, does have a usage if no number is provided
  void myHistory(char**);

  // Function: saveCommand
  // Params: char**
  // Usage: Saves the user inputted command, only saving previous 20 commands
  void saveCommand(char**);

  // Function: printPrompt
  // params: char* user, char* curDir
  // Usage: Prints out the prompt showing a user name and the current working directory
  void printPrompt(char* user, char* curDir);

  // exit
  void exitAtEndOfLine();

  // alias command
  void aliasNew(char**);

  // Function: pipeCheck
  // Params: char*
  // Usage: Returns how many pipes are involved within the command statement, returns -1 if there is I/O Direction as well as a pipe
  int pipeCheck(char *);

  // Function: pipeRun
  // Params: char*, int
  // Usage: Runs commands by piping the outputs to the next command 
  void pipeRun(char*, int);

  //PATH functions
  int myShellInteract();
  int myShellScript(char*);
  int execShell(char**);
  int myShellLaunch(char**);
  int myShell_path(char**);
  int numBuiltin();
  char** splitLine(char*);
  char* readLine();
