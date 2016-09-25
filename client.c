#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <fcntl.h>

#define DATA "Hello Server!"
#define FILENAME "received.txt"

void child_func(int childnum);

int main(int argc, char *argv[])
{
	int nc = 1;
	int pid;
	int i;
	
	if (argc > 1)
		nc = atoi(argv[1]);
	
	for (i = 0; i < nc; i++) {
		if ((pid = fork()) == 0) {
			child_func(i + 1);
			exit(0);
		}
	}
	
	wait(NULL);
	
	return 0;
}

void child_func(int childnum)
{
	int sock;
	struct sockaddr_in s_addr;
	char buffer[1024];
	int fs;
	FILE *received_file;
	ssize_t len;
	int remain_data = 0;
	
	memset((void *) &s_addr, 0, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = INADDR_ANY;
	s_addr.sin_port = 0;
	
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(sock, (const struct sockaddr *) &s_addr, sizeof(s_addr));
	
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	s_addr.sin_port = htons(9002);
	
	if(connect(sock, (const struct sockaddr *) &s_addr, sizeof(s_addr)) != 0) {
		printf("Client fail\n");
		return;
	}
	
	memset(buffer, 0, sizeof buffer);
	if (recv(sock, buffer, sizeof(buffer), 0) < 0) {		
		printf("Can't get message from server\n");
		close(sock);
		exit(1);
	}
	else {    
		printf("Client #%i. Message received from server: %s\n", childnum, buffer);
	}
	
	fs = atoi(buffer);
	
	
	received_file = fopen(FILENAME, "w");
	if (received_file == NULL)
	{
		printf("Failed to open file for writing '%s'\n", FILENAME);
		exit(1);
	}
	
	remain_data = fs;
	
	len = 0;
	memset(buffer, 0, sizeof buffer);
	while (((len = recv(sock, buffer, sizeof(buffer), 0)) > 0) && (remain_data > 0))
	{
		fwrite(buffer, sizeof(char), len, received_file);
		remain_data -= len;
		printf("Client #%i. Received %ld bytes. Remaining data = %d\n", childnum, len, remain_data);
	}
	if (len < 0)
		printf("Client #%i failed to receive file\n", childnum);
	
	fclose(received_file);
	sleep(1);
	
	close(sock);
	return;
}
