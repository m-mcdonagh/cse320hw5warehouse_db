#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#define equals(s1, s2) !strcmp(s1, s2)
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

BOOLEAN executeCommand(char** args){
	if (!args || !*args)
		fprintf(stderr, "Invalid Command\n");
	
	else if (equals(*args, "start")){
		
	}

	else if (equals(*args, "alloc")){
			
	}

	else if (equals(*args, "dealloc")){
		if (++args && *args){

		}
		else
			fprintf(stderr, "Invalid Arguments for \"dealloc\"\n");
	}

	else if (equals(*args, "read")){
		if (++args && *args){

		}
		else 
			fprintf(stderr, "Invalid Arguments for \"read\"\n");
	}

	else if (equals(*args, "store")){
		if (++args && *args && ++args && *args){
		
		}
		else 
			fprintf(stderr, "Invalid Arguments for \"store\"\n");
	}

	else if (equals(*args, "close")){
		
	}

	else if (equals(*args, "infotab")){
		
	}

	else if (equals(*args, "exit"))
		return FALSE;

	else 
		fprintf(stderr, "Invalid Command\n");

	return TRUE;
}

int main(void){
	shell_loop(16);
}

