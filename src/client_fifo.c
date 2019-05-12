#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "client.h"
#include "error_checking.h"
#define equals(s1, s2) !strcmp(s1, s2)
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

int conn_est;
int conn_conf;
int outFifo;
int inFifo;

void start(char* port){
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

	printf("Successfull Connected!\n");
}

void alloc(void){
	char id[256];
	write(outFifo, "a", 256);
	sleep(1);
	read(inFifo, id, 256);
	if (equals(id, "OUT OF MEMORY")){
		fprintf(stderr, "Error: Server Out of Memory\n");
		return;
	}
	if (isdigit(*id))
		printf("Art Allocated with ID %s\n", id);
	else
		fprintf(stderr, "Error please try again\n");
}

void dealloc(int id){
	char receipt[256];
	char msg[256];

	sprintf(msg, "d%d",  id);	
	write(outFifo, msg, 256);
	sleep(1);
	
	read(inFifo, receipt, 256);
	if (equals(receipt, "SUCCESS"))
		printf("%d deallocated!\n", id);
	else
		fprintf(stderr, "Error: Deallocation Failed (make sure this client controls %d)\n", id);
}

void readId(int id){
	char receipt[256];
	char msg[256];

	sprintf(msg, "r%d", id);
	write(outFifo, msg, 256);
	sleep(1);

	read(inFifo, receipt, 256);
	if (receipt[0] == '\0')
		fprintf(stderr, "Error: couldn't read name. Make sure %d has a name and this client has access to it!\n", id);
	else
		printf("%s\n", receipt);
}

void store(int id, char* art){
	char receipt[256];
	char msg[256];

	sprintf(msg, "s%d", id);
	write(outFifo, msg, 256);
	write(outFifo, art, 256);
	sleep(1);

	read(inFifo, receipt, 256);
	if (equals(receipt, "SUCCESS"))
		printf("Entry %d successfully named %s\n", id, art);
	else
		fprintf(stderr, "Error: couldn't name %d %s. Make sure this client has access to it!\n", id, art);
}

void infotab(void){}

void closeConnection(void){
	write(outFifo, "close", 256);
	sleep(1);

	close(conn_est);
	close(conn_conf);
	close(outFifo);
	close(inFifo);
}
