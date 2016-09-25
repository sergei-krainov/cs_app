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

//void sendfile_fork(void);
//void *sendfile_threads(void *nso);

//void sigchld_handler(int signo)
//{
	//while(waitpid(-1, NULL, WNOHANG) > 0);
//}

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addr;
	int ls;
	int ns;
	//char buffer[1024];
	int result;
	//int nread;
	//int pid;
	int var;
	//pthread_t thread_id;
	//socklen_t addr_size;
	//int fd;
	//struct stat file_stat;
	//off_t offset;
        //int remain_data;
        //int sent_bytes = 0;
        //int rc;
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
	
	//fd = open(FILE_TO_SEND, O_RDONLY);
	//if (fd == -1)
	//{
		//printf("Error opening file %s\n", FILE_TO_SEND);
		//exit(1);
	//}
	
	//if (fstat(fd, &file_stat) < 0)
	//{
		//printf("Can't get file_stat for file %s\n", FILE_TO_SEND);
		//exit(1);
	//}
	
	//fprintf(stdout, "File Size: \n%zu bytes\n", file_stat.st_size);

	//sendfile_fork();
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
		
		//#ifdef FORK
			//printf("Fork initialized\n");
			//int pid;
			
			//if ((pid = fork()) == 0) {
				//close(ls);
			
				//sendfile_fork((void *) (intptr_t) ns);
			//}
			//close(ns);
			//continue;
		//#endif
		
		//#ifdef THREADS
			//printf("Threads initialized\n");
			//pthread_t thread_id;
			
			//pthread_create(&thread_id, NULL, sendfile_threads, (void *) (intptr_t) ns);
			//continue;
		//#endif
		
		//printf("Please set behavior(FORK or THREADS) in DEFINE section. It's not done yet, exiting\n");
		//close(ls);
		//close(ns);
		//exit(0);
	}
	
	return 0;
	
	
		//result = pthread_create(&thread_id, NULL, sendfile_threads, (void *) (intptr_t) ns);
		//if (result != 0) {
			//printf("Can't create thread");
		//}
				
		//pthread_detach(thread_id);
		//sched_yield();
		
		//if ((pid = fork()) == 0) {
			//close(ls);
			
			////sendfile_fork((void *) (intptr_t) ns);
			
			
			
			////memset(buffer, 0, sizeof buffer);
			////printf("Child process %i created\n", getpid());
			////close(ls);
			
			////nread = recv(ns, buffer, sizeof(buffer), 0);
			////buffer[nread] = '\0';
			////printf("Received from client: %s\n", buffer);
			
			////memset(buffer, 0, sizeof buffer);			
			////sprintf(buffer, "%zu", file_stat.st_size);
			
			////if (send(ns, buffer, sizeof(buffer), 0) < 0) {
				////printf("Can't send message to client\n");
			////}
			////else {
				////printf("Sending file size = %s\n", buffer);
			////}
			
			////offset = 0;
		        ////remain_data = file_stat.st_size;
		        ////memset(buffer, 0, sizeof buffer);
        		//////* Sending file data */
        		////while (((sent_bytes = sendfile(ns, fd, &offset, sizeof(buffer))) > 0) && (remain_data > 0)) {
				////remain_data -= sent_bytes;
				////printf("Server sent %d bytes from file's data, offset is now : %zu and remaining data = %d\n", sent_bytes, offset, remain_data);
			////}
			
			////close(fd);
			////close(ns);
			////printf("Child %i terminated\n", getpid());
			////exit(0);
		//}
		//close(ns);
		//close(fd);
	//}
	
	
	//return 0;
}


//void sendfile_fork(void)
//{
	//int pid;
	//char buffer[1024];
	////int result;
	//off_t offset;
        //int remain_data;
        //int sent_bytes = 0;
        ////sem_t sem;
	
	//signal(SIGCHLD, sigchld_handler);
	
	//while(1) {
		//ns = accept(ls, NULL, NULL);
		//printf("BEGINNING = %d\n", fcntl(ns, F_GETFD)); 
		//if ((pid = fork()) == 0) {
			//close(ls);
			
			//memset(buffer, 0, sizeof buffer);
			//printf("Child process %i created\n", getpid());
			////close(ls);
			
			////nread = recv(ns, buffer, sizeof(buffer), 0);
			////buffer[nread] = '\0';
			////printf("Received from client: %s\n", buffer);
			
			//memset(buffer, 0, sizeof buffer);			
			//sprintf(buffer, "%zu", file_stat.st_size);
			
			//if (send(ns, buffer, sizeof(buffer), 0) < 0) {
				//printf("Can't send message to client\n");
			//}
			//else {
				//printf("Sending file size = %s\n", buffer);
			//}
			
			//offset = 0;
		        //remain_data = file_stat.st_size;
		        //memset(buffer, 0, sizeof buffer);
		        //sent_bytes = 0;
		        ////sem_init(&sem, 0, 0);
		        
        		////* Sending file data */
        		////sem_wait(&sem);
        		//printf("Test2, %d, %d\n", sent_bytes, remain_data);
        		//printf("fd = %d\n", fcntl(ns, F_GETFD)); 
        		//while (((sent_bytes = sendfile(ns, fd, &offset, sizeof(buffer))) > 0) && (remain_data > 0)) {
				//printf("Test3\n");
				//remain_data -= sent_bytes;
				//printf("Server sent %d bytes from file's data, offset is now : %zu and remaining data = %d\n", sent_bytes, offset, remain_data);
			//}
			//printf("Test4 => %d\n", sent_bytes);
			////sem_post(&sem);
			
			//close(fd);
			//close(ns);
			//printf("Child %i terminated\n", getpid());
			//exit(0);
		//}
		//close(ns);
		////close(fd);
	//}
	
	//return;
//}

//void sendfile_fork(void *nso, void *fdo, ssize_t f_size)
//{
	////int pid;
	//char buffer[1024];
	////int result;
	//off_t offset;
        //int remain_data;
        //int sent_bytes = 0;
        //int sock;
        //int fd;
        //fd = (intptr_t) fdo;
        //sock = (intptr_t) nso;
	
	//signal(SIGCHLD, sigchld_handler);
	
	////memset(buffer, 0, sizeof buffer);
	//printf("Child process %i created\n", getpid());
	////close(ls);
	
	////nread = recv(ns, buffer, sizeof(buffer), 0);
	////buffer[nread] = '\0';
	////printf("Received from client: %s\n", buffer);
	
	//memset(buffer, 0, sizeof buffer);			
	//sprintf(buffer, "%zu", f_size);
	
	//if (send(sock, buffer, sizeof(buffer), 0) < 0) {
		//printf("Can't send message to client\n");
	//}
	//else {
		//printf("Sending file size = %s\n", buffer);
	//}
	
	//offset = 0;
        //remain_data = f_size;
        //memset(buffer, 0, sizeof buffer);
        //sent_bytes = 0;
        ////sem_init(&sem, 0, 0);
        
	////* Sending file data */
	////sem_wait(&sem);
	//printf("Test2, %d, %d\n", sent_bytes, remain_data);
	//printf("fd = %d\n", fcntl(sock, F_GETFD)); 
	//while (((sent_bytes = sendfile(sock, fd, &offset, sizeof(buffer))) > 0) && (remain_data > 0)) {
		//printf("Test3\n");
		//remain_data -= sent_bytes;
		//printf("Server sent %d bytes from file's data, offset is now : %zu and remaining data = %d\n", sent_bytes, offset, remain_data);
	//}
	//printf("Test4 => %d\n", sent_bytes);
	////sem_post(&sem);
	
	//close(fd);
	//close(sock);
	//printf("Child %i terminated\n", getpid());
	//exit(0);
//}
