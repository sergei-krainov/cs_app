# Makefile for client-server application

SERVER = server server_fork
TARGETS := client
CFLAGS = -Wall -g -std=c99 -Werror

CC = gcc

all: clean ${TARGETS} ${SERVER}

${TARGETS}:
	${CC} ${CFLAGS} $@.c -o $@
	
${SERVER}:
	${CC} ${CFLAGS} server.c server_fork.c -o server
	
clean:
	rm -f ${TARGETS} ${SERVER} received.*
