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

#define DATA "Hello Server!"
#define FILENAME "received.txt"

void child_func(int childnum);

int main(int argc, char *argv[])
{
	int nc = 1;
	int pid;
	int i;
	
	if (argc > 1) {
		nc = atoi(argv[1]);
	}
	
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
	//sem_t sem;
	
	
	//sem_init(&sem, 0, 0);
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
	
	//memset(buffer, 0, sizeof buffer);
	//snprintf(buffer, 128, "Data from client #%i", childnum);
	//sleep(1);
	//printf("child #%i sent %zu chars\n", childnum, send(sock, buffer, strlen(buffer), 0));
	//sleep(1);
	//printf("child #%i received %zu chars\n", childnum, recv(sock, buffer, 25, 0));
	//memset(buffer, 0, sizeof buffer);
	//strcpy(buffer, DATA);
	//if ( send(sock, buffer, sizeof(buffer), 0) < 0) {
		//printf("Can't send a message\n");
	//close(sock);
	//exit(1);
	//}
	//else {
		//printf("Message sent from client #%i: %s\n", childnum, buffer);
	//}
	
	
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
	//printf("Size = %d\n", fs);
	
	received_file = fopen(FILENAME, "w");
        if (received_file == NULL)
        {
                printf("Failed to open file for writing '%s'\n", FILENAME);
                exit(1);
        }
        
        remain_data = fs;
        
        //sem_post(&sem);
        printf("Test1\n");
        //sem_wait(&sem);
        printf("Test2\n");
        //sem_post(&sem);
        printf("Test3\n");
        
        while (((len = recv(sock, buffer, sizeof(buffer), 0)) > 0) && (remain_data > 0))
        {
                fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                printf("Receive %ld bytes. Remaining data = %d\n", len, remain_data);
        }
        //sem_post(&sem);
        
        fclose(received_file);
	
	//memset(buffer, 0, sizeof buffer);
	//strcpy(buffer, DATA);
	//if ( send(sock, buffer, sizeof(buffer), 0) < 0) {
	//printf("Can't send a message\n");
	//close(sock);
	//exit(1);
	//}
	//else {
	//printf("Message sentfrom client #%i: %s\n", childnum, buffer);
	//}
	
	//sleep(1);
	close(sock);
}
