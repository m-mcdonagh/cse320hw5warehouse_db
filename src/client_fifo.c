#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "client.h"
#include "error_checking.h"
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

BOOLEAN started = FALSE;
int conn_est;
int conn_conf;
int outFifo;
int inFifo;

void start(char* port){
	if (started){
		fprintf(stderr, "Error: connection already established\n");
		return;
	}
	started = TRUE;
	conn_est = Open("tmp/conn_est", O_WRONLY);
	conn_conf = Open("tmp/conn_conf", O_RDONLY);
	write(conn_est, port, 1);
	sleep(1);
	char c;
	read(conn_conf, &c, 1);
	if (c == '0'){
		printf("%c\n", c);
		printf("Error: there exists a client with connection number %s. Please try a different number (0-3).\n", port);
		exit(EXIT_FAILURE);
	}

	char in[16] = "tmp/server_out ";
	char out[16] = "tmp/server_in ";
	in[14]  = port[0];
	out[13] = port[0];
	outFifo = Open(out, O_WRONLY);
	inFifo = Open(in, O_RDONLY);
}

void alloc(void){
	write(outFifo, "Greetings from the client!", 32);
	sleep(1);
	char output[256];
	read(inFifo, output, 32);
	printf("Message from server: %s\n", output);
}

void dealloc(int id){}

void readId(int id){}

void store(int id, char* art){}

void infotab(void){}

void closeConnection(void){}
