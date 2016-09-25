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
#include <pthread.h>

#define FILE_TO_SEND "cs_test.txt"

//void sigchld_handler(int signo);

void *sendfile_threads(void *nso)
{
	char buffer[1024];
	off_t offset;
        int remain_data;
        int sent_bytes = 0;
        int sock;
        sock = (intptr_t) nso;
        int fd;
	struct stat file_stat;
	
	printf("Child thread %lu with pid %i created\n", pthread_self(), getpid());
	
	fd = open(FILE_TO_SEND, O_RDONLY);
	if (fd == -1)
	{
		printf("Error opening file %s\n", FILE_TO_SEND);
		exit(1);
	}
	
	if (fstat(fd, &file_stat) < 0)
	{
		printf("Can't get file_stat for file %s\n", FILE_TO_SEND);
		exit(1);
	}
	
	fprintf(stdout, "File Size: \n%zu bytes\n", file_stat.st_size);
	
	memset(buffer, 0, sizeof buffer);			
	sprintf(buffer, "%zu", file_stat.st_size);
	
	if (send(sock, buffer, sizeof(buffer), 0) < 0) {
		printf("Can't send message to client\n");
	}
	else {
		printf("Sending file size = %s\n", buffer);
	}
	
	offset = 0;
        remain_data = file_stat.st_size;
        memset(buffer, 0, sizeof buffer);
        sent_bytes = 0;
        
	//* Sending file data */
	printf("Test2, %d, %d\n", sent_bytes, remain_data);
	printf("fd = %d\n", fcntl(sock, F_GETFD)); 
	while (((sent_bytes = sendfile(sock, fd, &offset, sizeof(buffer))) > 0) && (remain_data > 0)) {
		printf("Test3\n");
		remain_data -= sent_bytes;
		printf("Server sent %d bytes from file's data, offset is now : %zu and remaining data = %d\n", sent_bytes, offset, remain_data);
	}
	printf("Test4 => %d\n", sent_bytes);
	//sem_post(&sem);
	
	close(fd);
	close(sock);
	printf("Child thread %lu with pid %i terminated\n", pthread_self(), getpid());
	pthread_exit(0);
}
