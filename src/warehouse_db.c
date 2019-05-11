#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "error_checking.h"

int conn_est;
int conn_conf;
int inFifo[4] =    {-1,-1,-1,-1};
int outFifo[4] =   {-1,-1,-1,-1};
char filein[16] =  "tmp/server_in ";
char fileout[16] = "tmp/server_out ";

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
}

void newConnectionCheck(void){
	char c;
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

void processRequest(void){
	char msg[32];
	int i;
	for (i=0; i<4; i++){
		if (inFifo[i] != -1 && outFifo[i] != -1){
			//Test Code
			write(outFifo[i], "Greetings from the Server", 32);
			sleep(1);
			read(inFifo[i], msg, 32);
			if (msg[0] != EOF)
				printf("Message from %d: %s\n", i, msg);
		}
	}
}
