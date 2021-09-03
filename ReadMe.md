Project Created by: Dillon Earl Jones, Wesley Glover, Bikram Dahal, Aman Dhakal

Responsibilities:
Dillon Earl Jones
- cd
- SIG Catching
Wesley Glover
- history
- piping
Aman Dhakal
- exit
- alias
Bikram Dahal
- path
- I/O Redirection


How to Compile the Program

    1. Ensure you are in the directory of all the c files
    2. Use the make command to compile the project

How to Run the Program (Interactive Mode)

    1. Type "./major2" in the terminal while in the directory after compiling

How to Run the Program (Batch Mode)

    1. Type "./major2 [batch file name]" in the terminal while in the directory after compiling

    or

    1. While within the programs interactive mode type "newshell [batch file name]"
          - If no path is given with the batch file name then its assumed to be in
          the same directory as all the c files.
          - The newshell command does not require the shell to be in the same directory
          as the c files or the batch file

How to Clean the Program

    1. Type make clean in the terminal while in the directory after compiling


What Works
    * cd functionality is implemented properly (Dillon Jones)
        * Directories separated by spaces just need to be passed to the cd command with no quotes
        * cd when passed alone sends user to home directory
    * Sig handling handles all signals (Dillon Jones)
        * Parent process ignores ^C, ^Z
        * ^D will exit the process
    * History (Wesley Glover)
        * Called by the myhistory command
        * -c will clear command history
        * -e will execute a command from the history
            * will print usage if you do not put something after the flag
    * Piping works for up to 2 pipes with 3 commands (Wesley Glover)
        * EX: cat out.txt | sort -r | uniq
        * EX: cat out.txt | sort -r
        * Allows commands to have argumnets surrounded by apostrophe
            * EX: ps -ef | grep systemd | awk '{ print $2 }'
            * Note: If there is a command with apostrphes then the rest of the arguments in that command need apostrophes.
            * EX: ps -ef | grep systemd | awk '{ print $2 }' '{ print $3 }' // This may not be an actual function that works, just an example.
    * The shell will eat empty commands (Dillon Jones)
        * EX: $ ; ; ;
        * EX: $        
            * Will just print the prompt again and continue
    * Works with commands separated by semicolons (Dillon Jones)
        * EX: $ echo hello; ls; cat file1;
    * Exit works using the exit command (Aman Dhakal)
    * Alias command works only for one iteration AFTER a command has be aliased (Aman Dhakal)
        * EX: alias zom='ls'
        * This creates an alias for ls called zom
    * Added Features (Wesley Glover)
        * See added features section

What Doesn't Work
    * Exit does not support multi-commands (Aman Dhakal)
        * $ exit; ls; echo hello;
        * This command will exit before running the commands afterwards
    * Alias (Aman Dhakal)
        * Doesn't allow for multiple set aliases
        * Only allows for removal of a alias if it already existed
    * PATH (Bikram Dahal)
    * In/Out Direction (Bikram Dahal)

Known Problems
    * Alias (Aman Dhakal)
        * Will segfault if there are no current alias set and you try to remove one
        * Will segfault if a set alias is used once and then a remove of the alias is attempted


Added Features
    * Color Custimaziation of the Prompt (Wesley Glover)
        * Colors Supported
            * RED
            * GREEN
            * BLUE
            * MAGENTA
            * CYAN
            * YELLOW
        * EX: color user red
            * This makes the username in the prompt red
        * EX: color directory magenta
            * This makes the directory in the prompt magenta
    * Custimization of the Username (Wesley Glover)
        * You can change the username of the prompt
        * EX: username group10
            * This command would change the prompt username to group10
