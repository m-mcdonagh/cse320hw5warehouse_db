#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

pid_t Fork(){
	pid_t output;
	if ((output = fork()) < 0){
		fprintf(stderr, "Fork Error\n");
		exit(EXIT_FAILURE);
	}
	return output;
}

void Signal(int signum, __sighandler_t handler){
	if (signal(signum, handler) == SIG_ERR){
		fprintf(stderr, "Signal Handler Installation Error.\n");
		exit(EXIT_FAILURE);
	}
}

void Kill(pid_t pid, int sig){
	if (kill(pid, sig)){
		fprintf(stderr, "Error sending signal to %d.\n", (int)pid);
		exit(EXIT_FAILURE);
	}
}

void childReaper(int sig){
	while(waitpid(-1, NULL, WNOHANG) > 0);
	return;
}

int Open(const char* filename, int flags){
	int file;
	if ((file = open(filename, flags)) < 0){
		fprintf(stderr,"Error opening %s. Please read the README.md!\n", filename);
		exit(EXIT_FAILURE);
	}
	return file;
}

FILE* Fopen(const char* filename, const char* mode){
	FILE* output = fopen(filename, mode);
	if (output)
		return output;
	else{ 
		fprintf(stderr, "Error opening \"%s\".\n", filename);
		exit(EXIT_FAILURE);
	}
}

void Mkfifo(const char* pathname, mode_t mode){
	if (mkfifo(pathname, mode)){
		if (access(pathname, F_OK)){
			fprintf(stderr, "Error creating FIFOs\n");
		}
	}
}
