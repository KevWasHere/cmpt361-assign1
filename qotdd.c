/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # qotdd.c
  # Description here later
*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <sys/wait.h>

//#include <signal.h>

#define BACKLOG 20 //20 Pending connections allowed 

#include "qotdd_server.h"

//Parse usage (qotdd)
//Server Setup. Socket. Running (Accept) with fork
//GET HTTP 1.1 for JSON
//JSMN Parse

//Client to JSON - Socket, Connect, Close
//Write then read 

//Break this up into get, http, and content
char *header = "GET / HTTP/1.1\r\nHost: date.jsontest.com\r\nConnection:close\r\n\r\n";

/*Reference
Used for SIGCLD Handler
http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
*/

//Prints usage message of the program
void usage(char *progname){
  printf("usage: %s Usage message here\n", progname);
}

//Have main on bottom. Break up code later
//http://beej.us/guide/bgnet/output/html/multipage/index.html


/*
//Signal handler for SIGCHLD. Code sampled from microhow.info and CMPT360
void sigchld_handler(int signal){ 	
	//Chance of waitpid() overwriting errno. Save and restore later
  int saved_errno = errno;
  while((waitpid(-1, NULL, WNOHANG)) > 0){}
  errno = saved_errno;
}
*/

int main(int argc, char *argv[]){
  //Standard port if no port is specified
  /*char *port = "7890";  //CHANGE THIS LATER TO 1700
  
  struct addrinfo hints, *server_info, *current;
  int sock_fd, new_fd;
  int val = 1;
	*/
	
	int sock_fd;
	
	sock_fd = new_server();
	
	clean_chld();
	
	accept_loop(sock_fd);
	
	return 0;
	
	//Just use argc > 3 or something
  /*while ((option = getopt(argc, argv,":p")) != -1){
    switch(option){
    case 'p' : port = optarg;
      break;
    default: usage(basename[argv[0]]);
      exit(1);
    }
  }*/

  /*memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int err = getaddrinfo(NULL, port, &hints, &server_info);
  //printf("Err: %d\n", err);
  
  if (err != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    return 1; //Return anything but 0 means error. Might change later
  }
  for (current = server_info; current != NULL; current = current->ai_next){
    //Socket 
    sock_fd = socket(current->ai_family, current->ai_socktype, 
				current->ai_protocol);
    if (sock_fd == -1){
      perror("Server: socket");
      continue;
    }
    err = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
    if (err == -1){
      perror("Server: setsockopt");
      exit(1);
    }
    //Bind
    err = bind(sock_fd, current->ai_addr, current->ai_addrlen); 
    if (err == -1){
      close(sock_fd);
      perror("Server: bind");
      continue;
    } 
    //Listen
    err = listen(sock_fd, BACKLOG);
    if (err == -1){
      perror("Server: listen:");
      exit(1);  
    }
    
    //Working Server
    break;
  }
  
  printf("Server: Waiting for Connections...\n");
  
  freeaddrinfo(server_info);  //Done with Struct
  
  if (current == NULL){
    fprintf(stderr, "Could not create server\n");
    exit(1);
  }*/
	
	/*
	//Cleans dead processes. Code sampled from microhowto.info and CMPT360
  struct sigaction sa;
	sa.sa_handler = &sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGCHLD, &sa, 0) == -1) {
		perror("Sigaction");
  exit(1);
	}
	*/
	
	/*
  //Accept loop for connections
  while(1){
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    
    new_fd = accept(sock_fd, (struct sockaddr *)&client_addr,
		    &client_addr_size);
    if (new_fd == -1){
      perror("Server: accept:");
			continue;	//Don't want to close connection from one bad accept
    }
		
		char buff_size[INET6_ADDRSTRLEN];
		inet_ntop(client_addr.ss_family, 
				get_in_addr((struct sockaddr *)&client_addr),
				buff_size, sizeof buff_size);

		printf("Accepted Connection...\n");
		
		//Fork for accept connection
		int pid = fork();
		if (pid < 0) {
			//Fork failed. student server allowed for 50 processes
			fprintf(stderr, "Fork failed");
			exit(1);
		} else if (pid == 0) {
			//Currently child
			close(sock_fd); //Close becuase not listening for new connections
				
			char *msg = "Hello world!\n";
			send(new_fd, msg, strlen(msg), 0);
			
			close(new_fd);
			
			exit(0);
		} else {
			//Currently Parrent
			close(new_fd); //Close client because not interating
		}
		
		printf("Closed Connection...\n");
  }
	*/
  return 0;
}
