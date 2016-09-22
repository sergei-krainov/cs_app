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

void sigchld_handler(int signo)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addr;
	//struct sockaddr_in n_addr;
	int ls;
	int ns;
	char buffer[1024];
	int result;
	int nread;
	int pid;
	int var;
	//socklen_t addr_size;
	
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
		//addr_size = sizeof(s_addr);
		ns = accept(ls, NULL, NULL);
		if ((pid = fork()) == 0) {
			memset(buffer, 0, sizeof buffer);
			printf("Child process %i created\n", getpid());
			close(ls);
			
			nread = recv(ns, buffer, sizeof(buffer), 0);
			buffer[nread] = '\0';
			printf("Received from client: %s\n", buffer);
			
			memset(buffer, 0, sizeof buffer);
			strcpy(buffer,"Hello World!");
			if (send(ns, buffer, sizeof(buffer), 0) < 0) {
				printf("Can't send message to client\n");
			}
			else {
				printf("Said to client %s\n", buffer);
			}
			
			close(ns);
			printf("Child %i terminated\n", getpid());
			exit(0);
		}
		close(ns);
	}
	
	
	return 0;
}
