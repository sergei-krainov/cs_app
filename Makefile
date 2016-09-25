# Makefile for client-server application

SERVER = server
TARGETS := client
CFLAGS = -Wall -g -std=c99 -Werror
CFLAGS_SERVER = -Wall -g -std=c99 -Werror -pthread

CC = gcc

all: clean ${TARGETS} ${SERVER}

${TARGETS}:
	${CC} ${CFLAGS} $@.c -o $@
	
${SERVER}:
	${CC} ${CFLAGS_SERVER} server.c server_fork.c server_threads.c -o server
	
clean:
	rm -f ${TARGETS} ${SERVER} received.*
