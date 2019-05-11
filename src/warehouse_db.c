#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "warehouse_db.h"
#include "error_checking.h"
#define equals(s1, s2) !strcmp(s1, s2)
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

int max;
int conn_est;
int conn_conf;
int inFifo[4] =    {-1,-1,-1,-1};
int outFifo[4] =   {-1,-1,-1,-1};
char filein[16] =  "tmp/server_in ";
char fileout[16] = "tmp/server_out ";
struct artEntry* entries = NULL;

void reinitialize(int sig){
	conn_conf = Open("tmp/conn_conf", O_WRONLY);
	fcntl(conn_conf, F_SETFL, O_NONBLOCK);
}

void initializeDatabase(int maxRecords){
	conn_est =  Open("tmp/conn_est", O_RDONLY);
	conn_conf = Open("tmp/conn_conf", O_WRONLY);
	fcntl(conn_est, F_SETFL, O_NONBLOCK);
	fcntl(conn_conf, F_SETFL, O_NONBLOCK);
	Signal(SIGPIPE, reinitialize);

	max = maxRecords;
	entries = calloc(max, sizeof(struct artEntry));
	int i;
	for (i=0; i<max; i++){
		entries[i].id = i;
		entries[i].client = -1;
	}
}

void newConnectionCheck(void){
	char c = 0;
	int index;
	read(conn_est, &c, 1);
	if (isdigit(c)){
		index = c - '0';
		if (index > 3 || index < 0 || inFifo[index] != -1 || outFifo[index] != -1){
			write(conn_conf, "0", 1);
			sleep(1);
			return;
		}
		write(conn_conf, "1", 1);
		filein[13] = c;
		fileout[14] = c;
		inFifo[index]  = Open(filein, O_RDONLY);
		outFifo[index] = Open(fileout, O_WRONLY);
		fcntl(inFifo[index], F_SETFL, O_NONBLOCK);
		fcntl(outFifo[index], F_SETFL, O_NONBLOCK);
	}
	sleep(1);
}

char msg[32];

void checkRequest(void){
	msg[0] = '\0';
	int i;
	for (i=0; i<4; i++){
		if (inFifo[i] != -1 && outFifo[i] != -1){
			read(inFifo[i], msg, 32);
			if (msg[0] != '\0')
				processRequest(i, msg);
		}
	}
}

void processRequest(int client, char* msg){
	if (equals(msg, "alloc")){
		int i;
		for (i=0; entries[i].valid && i<max; i++);
		if (i == max)
			write(outFifo[client], "OUT OF MEMORY", 256);
		else{
			char out[256];
			sprintf(out, "%d", i);
			write(outFifo[client], out, 255);
			entries[i].valid = TRUE;
			entries[i].client = client;
		}
	}
}

void cleanUp(void){
	if (entries)
		free(entries);
}

void print_entry(int i){
	printf("Art Entry #%d: \"%s\" from client %d\n", entries[i].id, entries[i].name ? entries[i].name : "[none]", entries[i].client);
}	

void list(void){
	int i;
	for (i=0; i<max; i++)
		print_entry(i);
}

void print_fifos(void){
	printf("In Fifos:");
	int i;
	for (i=0; i<4; i++)
		printf(" %d", inFifo[i]);
	printf("\nOut Fifos:");
	for (i=0; i<4; i++)
		printf(" %d", outFifo[i]);
	printf("\n");
}

