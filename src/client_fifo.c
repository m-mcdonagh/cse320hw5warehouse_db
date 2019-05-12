#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
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
pthread_t tid;
struct art_entry_cache cache[4];
int count;

void* connect(void* voidptr){
	char* port = (char*)voidptr;
	conn_est = Open("tmp/conn_est", O_WRONLY);
	conn_conf = Open("tmp/conn_conf", O_RDONLY);
	
	char msg[256];
	sprintf(msg, "%s%ld", port, pthread_self());
	write(conn_est, msg, 256);
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

	printf("\nSuccessfully Connected!\n> ");
	fflush(stdout);
}

void start(char* port){
	pthread_create(&tid, NULL, connect, (void *)port);
	int i;
	for (i=0; i<4; i++){
		cache[i].valid = FALSE;
		cache[i].name = NULL;
		cache[i].id = 0;
	}
}

void cacheThis(int id){
	int i;
	for (i=0; i<4; i++){
		if (!cache[i].valid || cache[i].id == id){
			cache[i].id = id;
			if (cache[i].name)
				cache[i].name = NULL;
			cache[i].valid = TRUE;
			return;
		}
	}
	printf("eviction\n");
	int temp = count % 4;
	count++;
	if (cache[temp].name)
		free(cache[temp].name);
	cache[temp].id = id;
	cache[temp].name = NULL;
	cache[temp].valid = TRUE;
}

void nameThis(int id, char* name){
	int i;
	for (i=0; i<4; i++)
		if (cache[i].id == id){
			cache[i].name = malloc((strlen(name) + 1) * sizeof(char));
			strcpy(cache[i].name, name);
			return;
		}
}

void unCacheThis(int id){
	int i;
	for (i=0; i<4; i++)
		if (cache[i].id == id)
			cache[i].valid = FALSE;
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
	if (isdigit(*id)){
		printf("Art Allocated with ID %s\n", id);
		cacheThis(atoi(id));
	}
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
	if (equals(receipt, "SUCCESS")){
		printf("%d deallocated!\n", id);
		unCacheThis(id);
	}
	else
		fprintf(stderr, "Error: Deallocation Failed (make sure this client controls %d)\n", id);
}

void readId(int id){
	int i;
	for (i=0; i<4; i++)
		if (cache[i].id == id){
			printf("cache hit\n");
			printf("%s\n", cache[i].name);
			return;
		}
	printf("cache miss\n");

	char receipt[256];
	char msg[256];

	sprintf(msg, "r%d", id);
	write(outFifo, msg, 256);
	sleep(1);


	read(inFifo, receipt, 256);
	if (receipt[0] == '\0')
		fprintf(stderr, "Error: couldn't read name. Make sure %d has a name and this client has access to it!\n", id);
	else{
		printf("%s\n", receipt);
		cacheThis(id);
		nameThis(id, receipt);
	}
}

void store(int id, char* art){
	char receipt[256];
	char msg[256];

	sprintf(msg, "s%d", id);
	write(outFifo, msg, 256);
	write(outFifo, art, 256);
	sleep(1);

	read(inFifo, receipt, 256);
	if (equals(receipt, "SUCCESS")){
		printf("Entry %d successfully named %s\n", id, art);
		cacheThis(id);
		nameThis(id, art);
	}
	else
		fprintf(stderr, "Error: couldn't name %d %s. Make sure this client has access to it!\n", id, art);
}

void infotab(void){
	printf("the first level tables and prompt user to choose one of these tables. Then the user should be able to navigate from that first level table to the second level table\n");
}

void closeConnection(void){
	int i;
	for (i=0; i<4; i++)
		if (cache[i].name)
			free(cache[i].name);
	write(outFifo, "close", 256);
	pthread_join(tid, NULL);
	sleep(1);

	close(conn_est);
	close(conn_conf);
	close(outFifo);
	close(inFifo);

	printf("Successfully Disconnected\n");
}
