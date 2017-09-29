/* Program:     Shell
   Author:      Alexander Thoennes
   Date:        September 25, 2017
   File name:   asgn4-thoennesa2.c
   Compile:     cc -o asgn4-thoennesa2 asgn4-thoennesa2.c
   Run:         asng4-thoennesa2
   Description: This program is designed to mimick the process
                of how a shell works.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

// prototypes
void readLine(char *input);
void createArgs(char *inputString, char *argv[10]);
int isWhiteSpace(char c);
void execute(char *argv[10]);
void displayError(int exitCode, char * argv[]);
void quit();

int main()
{
    // char array that stores the command and
    // arguments passed in by the user
    char inputString[100];
    
    // get the users login info
    char *user = getlogin();
    
    // array for the arguments
    char *argv[10];
    
    // copy the string exit into a char array
    char exit[5];
    strcpy(exit, "exit");
    
    // run until the user wants to exit
    while (1)
    {
        // dislpay user log in info
        printf("\n%s: ",user);
        
        // read in a command and it's arguments from the user
        readLine(inputString);
        
        // adds the arguments to the argv array
        createArgs(inputString, argv);
        
        // determine if you have to exit the program or execute a commanad
        if (strcmp(argv[0], exit) != 0)
        {
            // execute the command with the arguments
            execute(argv);
        }
        else if (strcmp(argv[0], exit) == 0)
        {
            quit();
        }
    }
    
    return 0;
}

/**
 * This function only calls the exit function
 * because for some reason you are not allowed
 * to call exit from the main function
 */
void quit()
{
    exit(0);
}

/**
 * Function that executes the command and the
 * arguments by using the execvp() system call
 */
void execute(char *argv[])
{
    int exitCode;
    int cpid;
    int status;
    
    // remove the command from argv and store it
    // in command
    char *command = argv[0];
    
    // need to implement these two separately
    if (strcmp(command, "chdir") == 0 || strcmp(command, "cd") == 0)
    {
        char *dir = argv[1];
        chdir(dir);
    }
    // child process executes this
    else if ((cpid = fork()) == 0)
    {
        // execute instruction
        exitCode = execvp(command, argv);
        
        // if the instruction can not be executed,
        // then display the error
        displayError(exitCode, argv);
        
        // then exit the process. The only way this line will
        // ever be reached is if the child fails to execute
        // the user given command. Without this call to exit,
        // you would spawn multiple child processess every time
        // there is an error and then you would have to exit each
        //one of them individually
        exit(0);
    } // parent process executes this
    else if (cpid > 0)
    {
        cpid = waitpid(cpid, &status, 0);
    }
}

/**
 * Function that displays the error message
 * if execvp fails to execute the command
 **/
void displayError(int exitCode, char *argv[])
{
    // if execvp() failed
    if (exitCode == -1)
    {
        // print the error command with the associated
        // arguments passed to it
        printf("\nError: ");
        for (int i = 0; argv[i] != NULL; i ++)
        {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }
}

/**
 * Function that takes the string from input
 * and puts it into the argv array
 */
void createArgs(char *input, char *argv[10])
{
    char *tmp;
    tmp = input;
    int i = 0;
    
    // put first word into argv[0]
    argv[i] = strsep(&tmp, " \t");
    
    while ((i < 10) && (argv[i] != '\0'))
    {
        i++;
        argv[i] = strsep(&tmp, " \t");
    }
}

/**
 * Funciton that reads in a line of input from
 * the user and then replaces all white spaces
 * and \n with the null terminator \0
 */
void readLine(char *input)
{
    // read a char, skip extra white spaces, put it in input[]
    // and put a '\0' at the end
    int i = 0;
    int whiteSpace = 0;
    
    input[i] = getc(stdin);     // stdin is the keyboard
    while (input[i] != '\n')
    {
        if (whiteSpace)
        {
            if (!isWhiteSpace(input[i]))
            {
                i++;
                whiteSpace = 0;
            }
        }
        else
        {
            if (isWhiteSpace(input[i]))
            {
                whiteSpace = 1;
            }
            i++;
        }
        input[i] = getc(stdin);
    }
    
    // cut out white space before \n
    if (isWhiteSpace(input[i-1]))
    {
        i--;
    }
    
    // replace \n with the null terminator \0
    input[i] = '\0';
}

/**
 * funtion used to determine if the passed in char
 * is a white space or not
 */
int isWhiteSpace(char c)
{
    int i = 0;
    char whiteSpaces[] = {' ', '\t'};
    
    while (whiteSpaces[i] != '\0')
    {
        if (c == whiteSpaces[i++])
            return 1;
    }
    return 0;
}
