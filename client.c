#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DATA "Hello Server!"

int main(int argc, char *argv[]) {
    int c_socket;
    char buffer[1024];
    struct sockaddr_in s_addr;
    socklen_t addr_size;
    struct hostent *hostname;
    
    /*---- Checking input ----*/
    if ( argc != 2 ) {
        printf("Usage: %s $hostname\n", argv[0]);
        exit(1);
    }
    
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    c_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    
    /*---- Configure settings of the server address struct ----*/
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(5001);

    hostname = gethostbyname(argv[1]);
    if ( hostname == NULL ) {
        printf("Can't resolve hostname '%s'\n", argv[1]);
        exit(1);
    }

    memset(s_addr.sin_zero, '\0', sizeof s_addr.sin_zero);
    
    
    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof s_addr;
    if ( connect(c_socket, (struct sockaddr *) &s_addr, addr_size) < 0 ) {
        printf("Connection to server can't be established\n");
        exit(1);
    }
    
    
    /*---- Read the message from the server into the buffer ----*/
    if (recv(c_socket, buffer, sizeof(buffer), 0) < 0) {
        printf("Can't get message from server\n");
        exit(1);
    }
    else {    
        printf("Message received from server: %s\n", buffer);
    }
    
    strcpy(buffer, DATA);
    if ( send(c_socket, buffer, sizeof(buffer), 0) < 0) {
        printf("Can't send a message\n");
        close(c_socket);
        exit(1);
    }
    else {
        printf("Message sent: %s\n", buffer);
    }
    
    close(c_socket);
    
    return 0;
}
