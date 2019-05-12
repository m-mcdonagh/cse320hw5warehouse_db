#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "warehouse_db.h"
#include "error_checking.h"
#include "shell.h"
#define equals(s1, s2) !strcmp(s1, s2)
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

BOOLEAN executeCommand(char** args){
	if (!args || !*args){
		fprintf(stderr, "Invalid Command\n");
		return TRUE;
	}
	
	else if (equals(*args, "list")){
		if (++args && *args)
			list(atoi(*args));
		else
			listAll();
	}

	else if (equals(*args, "dump")){
		dump();
	}

	else if (equals(*args, "exit")){
		cleanUp();
		exit(EXIT_SUCCESS);
	}

	else {
		fprintf(stderr, "Invalid Command\n");
		return TRUE;
	}

	return FALSE;
}

void startShell(int sig){
	printf("\nPlease Enter a Command for the Warehouse Database Server\n");
	shell_loop(16);
}

int main(int argc, char** argv){
	if (argc != 2){
		if (argc > 0)
			fprintf(stderr, "Invalid Arguments for %s\n", *argv);
		else 
			fprintf(stderr, "Invalid Arguments for program\n");
		exit(EXIT_FAILURE);
	}
	Signal(SIGINT, startShell);
	
	printf("Initializing Database\n");	
	initializeDatabase(atoi(*++argv));
	
	while(TRUE){
		newConnectionCheck();
		checkRequest();
	}
}
