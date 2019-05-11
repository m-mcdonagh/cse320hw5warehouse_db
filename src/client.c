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

	// Establishes a connection with the server, tells it which FIFO to use, and makes sure another client isn't on that FIFO
	int conn_est = Open("tmp/conn_est", O_WRONLY);
	int conn_conf = Open("tmp/conn_conf", O_RDONLY);
	write(conn_est, *argv, 1);
	sleep(1);
	char c;
	read(conn_conf, &c, 1);
	if (c == '0'){
		printf("Error: there exists a client with connection number %s. Please try a different number (0-3).\n", *argv);
		exit(EXIT_FAILURE);
	}

	// Opens the Correct FIFO
	char in[16] = "tmp/server_out ";
	char out[16] = "tmp/server_in ";
	in[14]  = *argv[0];
	out[13] = *argv[0];
	int outFifo = Open(out, O_WRONLY);
	int inFifo = Open(in, O_RDONLY);

	// Test Code
	write(outFifo, "Greetings from the client!", 32);
	sleep(1);
	char output[256];
	read(inFifo, output, 32);
	printf("Message from server: %s\n", output);

	//shell_loop(16);
}

