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
#include "fork_functions.h"
#include "threads_functions.h"

/* Behavior switch, you should define at least one.
 * If both defined - server will work using THREADS
 */

#define FORK
#define THREADS

#define FILE_TO_SEND "cs_test.txt"

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addr;
	int ls;
	int ns;
	int result;
	int var;
	int bh = 0;
	
	#ifdef FORK
		bh = 1;
	#endif
	#ifdef THREADS
		bh = 2;
	#endif
	
	if (bh == 0) {
		printf("Please set behavior(FORK or THREADS) in DEFINE section. It's not done yet, exiting\n");
		exit(0);
	}
		
	
	
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
		
		switch(bh) {
		case 1:
			printf("Fork initialized\n");
			int pid;
			
			if ((pid = fork()) == 0) {
				close(ls);
			
				sendfile_fork((void *) (intptr_t) ns);
			}
			close(ns);
			break;
		case 2:
			printf("Threads initialized\n");
			pthread_t thread_id;
			
			pthread_create(&thread_id, NULL, sendfile_threads, (void *) (intptr_t) ns);
			break;
		default :
			printf("Impossible to come here\n");
			break;
		}
	}
	
	return 0;
}
