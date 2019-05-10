#ifndef SHELL
#define SHELL
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#define BOOLEAN char
#define FALSE 0
#define TRUE 1


/*
 * commandSplitter()
 * splits a command into its commponents, by whitespace, while keeping words within quotes (i.e. "hello there") as one argument
 *
 * Params
 * 	commandLine
 * 	pointer to a string. It's contents are changed and referenced by the return value, so it shouldn't be altered outside of this function for the return values's duration of use (done so each *output need not be free()'d)
 *	
 *	maxArgs
 *	value to determine the maximum amount of args checked for and returned
 *
 * Return
 * 	Pointer to pointers of strings, up to maxArgs
 */
char** commandSplitter(char* commandLine, int maxArgs){
	while (isspace(*commandLine)) commandLine++;
	if (*commandLine == '\0'){
		return NULL;
	}
	char** output = malloc((maxArgs+1)*sizeof(char *));
	int i;
	for (i=0; i<(maxArgs+1); i++)
		output[i] = NULL;
	int index = 0;
	output[index] = commandLine;
	
	while (*++commandLine != '\0'){
		if (isspace(*commandLine)){
			if (*commandLine == '\n'){
				*commandLine = '\0';
				return output;
			}
			*commandLine = '\0';
			if (index >= maxArgs-1){
				char** temp = output;
				output = malloc(2*(maxArgs+1) * sizeof(char*));
				for (i=0; i<maxArgs; i++)
					output[i] = temp[i];
				free(temp);
				maxArgs = maxArgs * 2;
				while (i < maxArgs+1)
					output[++i] = NULL;
			}
			while (isspace(*++commandLine));
			if (*commandLine == '\0')
				return output;
			if (*commandLine == '\"'){
				output[++index] = ++commandLine;
				while (*commandLine != '\"'){
					if (*commandLine == '\0'){
						free(output);
						return NULL;
					}
					commandLine++;
				}
				*commandLine = '\0';
				commandLine++;
			}
			output[++index] = commandLine;
		}
	}
	return output;
}

/*
 * executeCommand()
 * must be defined separately for each program
 *
 * Params:
 * 	arguments
 * 	list of string pointers for each argument
 *
 * Return:
 * 	BOOLEAN (char)
 * 	TRUE if shell should keep executing
 * 	FALSE if shell should terminate
 */
BOOLEAN executeCommand(char** args);
/* for testing purposes on executeCommand()
BOOLEAN executeCommand(char** args){
	for(int i=0; arguments[i]; i++)
		printf("%d) %s\t", i+1, arguments[i]);
	printf("\n");
	if (!strcmp(*arguments, "exit")){
		return FALSE;
	}
	else return TRUE;
}*/

/*
 * shell_loop()
 * runs the loop of the main program to ask for input from the user and execute accordingly
 * ends when executeCommand returns false
 *
 * Params
 * 	maxArgs
 * 	value for the maximum amount of arguments (including the command itself) correct input can have
 *
 * Return
 * 	void
 */
void shell_loop(int maxArgs){
	char* commandLine = NULL;
	char** args;
	size_t bufsize = 32;
	BOOLEAN notExit = TRUE;
	
	while(notExit){
		printf("> ");
		getline(&commandLine, &bufsize, stdin);
		args = commandSplitter(commandLine, maxArgs);
		notExit = executeCommand(args);
		if (args)
			free(args);
		if (commandLine)
			free(commandLine);
		commandLine = NULL;
	}
}
#endif
