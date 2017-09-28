/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # qotdd.c
  # Description here later
*-----------------------------------------------------------------------*/
//#define _POSIX_C_SORUCE >= 1
//#define _XOPEN_SOURCE
//#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <signal.h>

#define BACKLOG 20 //20 Pending connections allowed 

//Prints usage message of the program
void usage (char *progname){
  printf("usage: %s Usage message here\n", progname);
}

//Have main on bottom. Break up code later
//http://beej.us/guide/bgnet/output/html/multipage/index.html

int main(int argc, char *argv[]) {
  //Standard port if no port is specified
  char *port = "1700";
  
  struct addrinfo hints, *server_info, *current;
  int option, sock_fd, new_fd;
  int val = 1;
  /* while ((option = getopt(argc, argv,":p")) != -1){
    switch(option){
    case 'p' : port = optarg;
      break;
    default: usage(basename[argv[0]]);
      exit(1);
    }
  }*/


  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int err = getaddrinfo(NULL, port, &hints, &server_info);
  printf("Err: %d\n", err);
  
  if (err != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    return 1; //Return anything but 0 means error. Might change later
  }
  for (current = server_info; current != NULL; current = current->ai_next){
	//Socket
    if ((sock_fd = socket(current->ai_family, current->ai_socktype, 
			  current->ai_protocol)) == -1){
      perror("Server: Socket");
      continue;
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &val, 
		     sizeof(int)) == -1){
      perror("Server: setsockopt");
      exit(1);
    }
	//Bind
    if (bind(sock_fd, current->ai_addr, current->ai_addrlen) == -1){
      close(sock_fd);
      perror("Server: bind");
      continue;
    } 
	//Listen
    if (listen(sock_fd, BACKLOG) == -1){
      perror("Server: listen:");
      exit(1);
    }
 }
  
  printf("It's working!!!\n");

  freeaddrinfo(server_info);  //Done with Struct

  if (current == NULL){
    fprintf(stderr, "Could not create server\n");
    exit(1);
  }

  //Add stuff for client connection (Accept loop)
  return 0;
}
