#ifndef QOTDD_SERVER_H_
#define QOTDD_SERVER_H_

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <sys/wait.h>

#include "json_server.h"
#include "json_parse.h"

/*Prototypes*/
int new_qotdd_server();
//int new_socket(int sock_fd, int val, struct addrinfo current, int err);
void clean_chld();
int accept_loop(int sock_fd, char *port, char *hostname, 
		char *path, char * key);
void *get_in_addr(struct sockaddr *current_addr);
int accept_fork(int sock_fd, int new_fd, char *port, char *hostname, 
		char *path, char * key);
#endif