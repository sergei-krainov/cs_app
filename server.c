#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdint.h>
#include <pthread.h>
#include "functions.h"

//define FORK
#define THREADS

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addr;
	int ls;
	int ns;
	int result;
	int var;
	
	ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	var = 1;
	result = setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &var, sizeof(var));
	if (result < 0) {
		printf("Can't assign socket oprions\n");
		return 0;
	}
	
	memset((void *) &s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = INADDR_ANY;
	s_addr.sin_port = htons(9002);
	
	result = bind(ls, (struct sockaddr *) &s_addr, sizeof(s_addr));
	if (result < 0) {
		printf("Socket binding error\n");
		return 0;
	}
	
	result = listen(ls, 10);
	if (result < 0) {
		printf("Listening can't be started\n");
		return 0;
	}
	else {
		printf("Listening on port 9002\n");
	}
	
	signal(SIGCHLD, sigchld_handler);
	
	while(1) {
		ns = accept(ls, NULL, NULL);
		
		#ifdef FORK
            printf("Fork initialized\n");
            int pid;
            if ((pid = fork()) == 0) {
                close(ls);
                
                sendfile_mp((void *) (intptr_t) ns);
                exit(0);
            }
            close(ns);
        #else
            printf("Threads initialized\n");
			pthread_t thread_id;
            
            pthread_create(&thread_id, NULL, (void *) sendfile_mp, (void *) (intptr_t) ns);
        #endif
	}
	
	return 0;
}
