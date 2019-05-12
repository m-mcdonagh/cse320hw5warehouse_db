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
			read(inFifo[i], msg, 256);
			if (msg[0] != '\0')
				processRequest(i, msg);
		}
	}
}

void processRequest(int client, char* msg){
	int id;
	switch (msg[0]){
		case 'a':
			for (id=0; entries[id].valid && id<max; id++);
			if (id == max)
				write(outFifo[client], "OUT OF MEMORY", 256);
			else{
				char out[256];
				sprintf(out, "%d", id);
				write(outFifo[client], out, 256);
				entries[id].valid = TRUE;
				entries[id].client = client;
			}
			break;
		case 'd':
			msg++;
			id = atoi(msg);
			if (id < max && id >= 0 && entries[id].client == client && entries[id].valid){
				entries[id].valid = FALSE;
				write(outFifo[client], "SUCCESS", 256);
			}
			else
				write(outFifo[client], "FAILURE", 256);
			break;
		case 'r':
			msg++;
			id = atoi(msg);
			if (id < max && id >= 0 && entries[id].client == client && entries[id].valid && entries[id].name)
				write(outFifo[client], entries[id].name, 256);
			else
				write(outFifo[client], "\0", 256);
			break;
		case 's':
			msg++;
			id = atoi(msg);
			sleep(1);
			read(inFifo[id], msg, 256);
			if (id < max && id >= 0 && entries[id].client == client && entries[id].valid){
				if (entries[id].name)
					free(entries[id].name);
				entries[id].name = malloc( (strlen(msg) + 1) * sizeof(char));
				strcpy(entries[id].name, msg);
				write(outFifo[client], "SUCCESS", 256);
			}
			else
				write(outFifo[client], "FAILURE", 256);
			break;
		
	}
}

void cleanUp(void){
	if (entries)
		free(entries);
}

void print_entry(int i){
	printf("Art Entry #%d from client %2d is%s valid.  Named %c%s%c\n", entries[i].id, entries[i].client, entries[i].valid ? "   " : "n't", entries[i].name? '\"' : '[', entries[i].name ? entries[i].name : "none", entries[i].name? '\"' : ']');
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

