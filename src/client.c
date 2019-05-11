#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "shell.h"
#include "client.h"
#include "error_checking.h"
#define equals(s1, s2) !strcmp(s1, s2)
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

char port[2];

BOOLEAN executeCommand(char** args){
	if (!args || !*args)
		fprintf(stderr, "Invalid Command\n");
	
	else if (equals(*args, "start")){
		start(port);
	}

	else if (equals(*args, "alloc")){
		alloc();
	}

	else if (equals(*args, "dealloc")){
		if (++args && *args){
			dealloc(atoi(*args));
		}
		else
			fprintf(stderr, "Invalid Arguments for \"dealloc\"\n");
	}

	else if (equals(*args, "read")){
		if (++args && *args){
			readId(atoi(*args));
		}
		else 
			fprintf(stderr, "Invalid Arguments for \"read\"\n");
	}

	else if (equals(*args, "store")){
		if (++args && *args && ++args && *args){
			store(atoi(*--args), *++args);
		}
		else 
			fprintf(stderr, "Invalid Arguments for \"store\"\n");
	}

	else if (equals(*args, "close")){
		closeConnection();
	}

	else if (equals(*args, "infotab")){
		infotab();
	}

	else if (equals(*args, "exit")){
		closeConnection();
		return FALSE;
	}

	else 
		fprintf(stderr, "Invalid Command\n");

	return TRUE;
}

int main(int argc, char** argv){
	// Makes sures a correct argument was passed when client was executed (0-3).
	if (argc != 2){
		if (argc > 0)
			fprintf(stderr, "Invalid Arguments for %s. Please enter an integer between 0-3 inclusive\n", *argv);
		else
			fprintf(stderr, "Invalid Arguments for program. Please enter an integer between 0-3 inclusive\n");
		exit(EXIT_FAILURE);
	}
	int inputInt = atoi(*++argv);
	if (inputInt < 0 || inputInt > 3){
		fprintf(stderr, "Invalid Arguments for %s. Please enter an integer between 0-3 inclusive\n", *--argv);
		exit(EXIT_FAILURE);
	}
	
	port[0] = **argv;
	port[1] = '\0';

	shell_loop(16);
}

