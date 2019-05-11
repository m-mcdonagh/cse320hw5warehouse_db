#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "error_checking.h"

FILE* inFifo[4];
FILE* outFifo[4];

void initializeDatabase(int maxRecords){
	inFifo[0] = Fopen("tmp/server_in0", "r");
	printf("done\n");
	inFifo[1] = Fopen("tmp/server_in1", "r");
	inFifo[2] = Fopen("tmp/server_in2", "r");
	inFifo[3] = Fopen("tmp/server_in3", "r");
	outFifo[0] = Fopen("tmp/server_out0", "w");
	outFifo[1] = Fopen("tmp/server_out2", "w");
	outFifo[2] = Fopen("tmp/server_out3", "w");
	outFifo[3] = Fopen("tmp/server_out4", "w");
	
	printf("Server putted\n");	
	fputs("Greetings from the server!", outFifo[0]);
	sleep(10);
	char output[256];
	printf("Server slept\n");
	fgets(output, 256, inFifo[0]);
	printf("Message from client: %s\n", output);
}
