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

void sendfile_mp(void *nso)
{
	char buffer[1024];
	off_t offset;
	int remain_data;
	int sent_bytes = 0;
	int sock;
	sock = (intptr_t) nso;
	int fd;
	struct stat file_stat;
	
	printf("Child process %i created\n", getpid());
	
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

	while (((sent_bytes = sendfile(sock, fd, &offset, sizeof(buffer))) > 0) && (remain_data > 0)) {
		remain_data -= sent_bytes;
		printf("Server sent %d bytes from file's data, offset is now : %zu and remaining data = %d\n", sent_bytes, offset, remain_data);
	}
	
	close(fd);
	close(sock);
	printf("Child %i terminated\n", getpid());
}

void sigchld_handler(int signo)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
