#ifndef JSON_SERVER_H_
#define JSON_SERVER_H_

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*Prototypes*/
int sock_json(char *hostname, char *port);
char *receive_json(int sock_fd, char *path, char *hostname);
#endif