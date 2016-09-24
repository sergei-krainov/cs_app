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
#include "functions.h"


#define FILE_TO_SEND "cs_test.txt"

//void sigchld_handler(int signo);

void sendfile_fork(void)
{
	int pid;
	char buffer[1024];
	//int result;
	off_t offset;
        int remain_data;
        int sent_bytes = 0;
        //sem_t sem;
	
	signal(SIGCHLD, sigchld_handler);
	
	while(1) {
		ns = accept(ls, NULL, NULL);
		printf("BEGINNING = %d\n", fcntl(ns, F_GETFD)); 
		if ((pid = fork()) == 0) {
			close(ls);
			
			memset(buffer, 0, sizeof buffer);
			printf("Child process %i created\n", getpid());
			//close(ls);
			
			//nread = recv(ns, buffer, sizeof(buffer), 0);
			//buffer[nread] = '\0';
			//printf("Received from client: %s\n", buffer);
			
			memset(buffer, 0, sizeof buffer);			
			sprintf(buffer, "%zu", file_stat.st_size);
			
			if (send(ns, buffer, sizeof(buffer), 0) < 0) {
				printf("Can't send message to client\n");
			}
			else {
				printf("Sending file size = %s\n", buffer);
			}
			
			offset = 0;
		        remain_data = file_stat.st_size;
		        memset(buffer, 0, sizeof buffer);
		        sent_bytes = 0;
		        //sem_init(&sem, 0, 0);
		        
        		/* Sending file data */
        		//sem_wait(&sem);
        		printf("Test2, %d, %d\n", sent_bytes, remain_data);
        		printf("fd = %d\n", fcntl(ns, F_GETFD)); 
        		while (((sent_bytes = sendfile(ns, fd, &offset, sizeof(buffer))) > 0) && (remain_data > 0)) {
				printf("Test3\n");
				remain_data -= sent_bytes;
				printf("Server sent %d bytes from file's data, offset is now : %zu and remaining data = %d\n", sent_bytes, offset, remain_data);
			}
			printf("Test4 => %d\n", sent_bytes);
			//sem_post(&sem);
			
			close(fd);
			close(ns);
			printf("Child %i terminated\n", getpid());
			exit(0);
		}
		close(ns);
		//close(fd);
	}
	
	return;
}
