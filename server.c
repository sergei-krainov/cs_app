#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

//void sigchld_handler(int signo) {
    //while(waitpid(-1, NULL, WHOHANG) > 0) {
        //printf("Child terminated\n");
    //}
//}

int main() {
    int def_sock, new_sock;
    struct sockaddr_in s_addr;
    struct sockaddr_in n_addr;
    socklen_t addr_size;
    char buffer[1024];
    int pid;
    int nread;
    
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    def_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if  (def_sock == -1) {
        printf("Socket can't be created, exit script");
        exit(1);
    }
    
    
    /*---- Configure settings of the server address struct ----*/
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(5001);
    s_addr.sin_addr.s_addr = INADDR_ANY;
    
    memset(s_addr.sin_zero, '\0', sizeof s_addr.sin_zero);
    printf("Port = '%d'\n", s_addr.sin_port);
    
    /*---- Bind the address struct to the socket ----*/
    bind(def_sock, (struct sockaddr *) &s_addr, sizeof(s_addr));
    
    
    
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(def_sock,5)==0)
        printf("Listening\n");
    else
        printf("Listening can't be started\n");
    
    
    /*---- Accept call creates a new socket for the incoming connection ----*/
    //do {
        //addr_size = sizeof n_addr;
        //new_sock = accept(def_sock, (struct sockaddr *) &n_addr, &addr_size);
        
        //if ( new_sock == -1 ) {
            //printf("Can't create socket for connection\n");
            //exit(1);
        //}    
    
        ////*---- Send message to the socket of the incoming connection ----*/
        //memset(buffer, 0, sizeof buffer);
        //strcpy(buffer,"Hello World!\n");
        //send(new_sock,buffer,13,0);
        
        ////*---- Read the message from the client into the buffer ----*/
        ////memset(buffer, 0, sizeof buffer);
        //if (recv(new_sock, buffer, sizeof(buffer), 0) < 0) {
            //printf("Can't get message from client\n");
        //}
        //else {
            //printf("Client said %s\n", buffer);
        //}
        
        
    //} while(1);
    //signal(SIGCHLD, sigchld_handler);
    
    while(1) {
        memset(buffer, 0, sizeof buffer);
        new_sock = accept(def_sock, (struct sockaddr *) &n_addr, &addr_size);
        
        if (new_sock == -1) {
            printf("Can't create socket for connection\n");
            exit(1);
        }
        
        if ((pid = fork()) == -1) {
            printf("Cannot fork a process, exiting parent\n");
            exit(1);
        }
        else if (pid == 0) {
            printf("Child created with PID '%i'\n", getpid());
            printf("Port = '%d'\n", s_addr.sin_port);
            //close(new_sock);
            
            //nread = recv(new_sock, buffer, sizeof(buffer), 0);
            //if (nread < 0) {
            //    printf("Can't get client message\n");
            //}
            printf("Test5\n");
            
            memset(buffer, 0, sizeof buffer);
            strcpy(buffer,"Hello World!\n");
            
            if(send(new_sock,buffer,13,0) < 0) {
                printf("Message has not been sent\n");
            }
            else {
                printf("Message to client has been sent\n");
            }
            
            memset(buffer, 0, sizeof buffer);
            nread = recv(new_sock, buffer, sizeof(buffer), 0);
            if (nread < 0) {
                printf("Can't get client message\n");
            }
            else {
                printf("Received from client: %s\n", buffer);
            }
            
            close(new_sock);
            
            //sleep(20);
            printf("Child terminated with PID '%i'\n", getpid());
                        
            exit(0);
            //waitpid(-1, NULL, WHOHANG);
        }
        else if (pid > 0) {
            //wait(0);
            
            printf("Parent = %i\n", getpid());
            close(new_sock);
            //wait(0);
        }
        
        wait(0);
    }
    
    /* Code below working somehow */
    //int counter;
    
    //for(;;)
    //{
        //new_sock = accept(def_sock, (struct sockaddr *)&n_addr, &addr_size);

        //if ((pid = fork()) == -1)
        //{
            //close(new_sock);
            //continue;
        //}
        //else if(pid > 0)
        //{
            //close(new_sock);
            //counter++;
            //printf("here2\n");
            ////continue;
        //}
        //else if(pid == 0)
        //{
            //char buf[100];

            //counter++;
            //printf("here 1\n");
            //snprintf(buf, sizeof buf, "hi %d", counter);
            //send(new_sock, buf, strlen(buf), 0);
            //close(new_sock);
            //break;
        //}
//}
  
    
    return 0;
}
