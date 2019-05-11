#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "shell.h"
#include "error_checking.h"
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

int main(int argc, char** argv){
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

	char in[16] = "tmp/server_out";
	char out[16] = "tmp/server_in";
	strcat(in, *argv);
	strcat(out, *argv);

	printf("%s %s\n", in, out);

	FILE* inFifo = Fopen(in, "r");
	FILE* outFifo = Fopen(out, "w");

	fputs("Greetings from the client!", outFifo);
	sleep(10);
	char output[256];
	fgets(output, 256, inFifo);
	printf("Message from server: %s\n", output);

	//shell_loop(16);
}

