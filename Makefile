# Makefile for client-server application

TARGETS := server
TARGETS += client

CFLAGS = -Wall -g -std=c99 -Werror -pthread

CC = gcc

all: clean ${TARGETS}

${TARGETS}:
	${CC} ${CFLAGS} $@.c -o $@
	
clean:
	rm -f ${TARGETS} received.*
