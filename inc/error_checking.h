#ifndef ERROR_CHECKING
#define ERROR_CHECKING
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

pid_t Fork();
void Signal(int signum, __sighandler_t handler);
void Kill(pid_t pid, int sig);
void childReaper(int sig);
int Open(const char* filename, int flags);
FILE* Fopen(const char* filename, const char* mode);
void Mkfifo(const char* pathname, mode_t mode);
#endif
