/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # qotdd_server.c
  # Functions that create a server for server-client connection
*-----------------------------------------------------------------------*/

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

#define BACKLOG 20 //20 Pending connections allowed 

/*Reference
Used for SIGCLD Handler
http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
*/

//Signal handler for SIGCHLD. Code sampled from microhow.info and CMPT360
void sigchld_handler(int signal){ 	
	//Chance of waitpid() overwriting errno. Save and restore later
  int saved_errno = errno;
  while((waitpid(-1, NULL, WNOHANG)) > 0){}
  errno = saved_errno;
}

//Cleans dead processes. Code sampled from microhowto.info and CMPT360
void clean_chld(){
  struct sigaction sa;
	sa.sa_handler = &sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGCHLD, &sa, 0) == -1) {
		perror("Sigaction");
		exit(1);
	}
}

//Sockaddr for IPv4 or IPv6:
void *get_in_addr(struct sockaddr *current_addr){
	if (current_addr->sa_family == PF_INET){
		return &(((struct sockaddr_in*)current_addr)->sin_addr);
	} else if (current_addr->sa_family == PF_INET6){
		return &(((struct sockaddr_in6*)current_addr)->sin6_addr);
	} else {
		fprintf(stderr, "Unexpected address family for client\n");
		exit(EXIT_FAILURE);
	}	
}

//Creates new server for client-server connection
int new_qotdd_server(){
	char *port = "7890";   ///Change this before submission
	struct addrinfo hints, *server_info, *current;
	int sock_fd;
	int val = 1;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	int err = getaddrinfo(NULL, port, &hints, &server_info);
	
	if (err != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		exit(1);
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
	}
	
	return sock_fd;
}

//Fork for accept connection
int accept_fork(int sock_fd, int new_fd, char *port, char *hostname,
			char *path, char *key){
	int pid = fork();
	int json_fd;
	char *buffRead, *statusCode; 
	char *statusLine = malloc(1000 * sizeof(char));
	char *keyValue = malloc(1000 * sizeof(char));
	
	if (statusLine == NULL){
		fprintf(stderr, "Malloc Failed\n");
		exit(1);
	}
	if (keyValue == NULL){
		fprintf(stderr, "Malloc Failed\n");
		exit(1);
	}
	
	if (pid < 0){
		//Fork failed. student server allowed for 20 processes
		fprintf(stderr, "Fork failed");
		exit(1);
	} else if (pid == 0) {
		//Currently child
		close(sock_fd); //Close becuase not listening for new connections
	
		json_fd = sock_json(hostname, port);
		buffRead = receive_json(json_fd, path, hostname);
		
		//Send parsed JSON to client
		//Correct request was sent
		if (check_request(buffRead) == 0){
			keyValue = json_data(buffRead, key);;
			//Key was not found in JSON data
			if ((strcmp(keyValue, "\0")) == 0) {
				keyValue = "Key was not found! Please change key and try again!\n";
				send(new_fd, keyValue, strlen(keyValue), 0);
				//Key Found
			} else {
				send(new_fd, keyValue, strlen(keyValue), 0);
				send(new_fd, "\n", 1, 0);
			}
		}
		//Request was wrong/other errors
		else {
			//Token first line which is status line
			statusLine = "Cannot Obtain Quote: ";
			statusCode = strtok(buffRead, "\r\n");
			strcat(statusCode, "\n\0");
			send(new_fd, statusLine, strlen(statusLine), 0);
			send(new_fd, statusCode, strlen(statusCode), 0);
		}
		
		free(buffRead);
		close(new_fd);
		
		exit(0);
	} else {
		//Currently Parrent
		close(new_fd); //Close client because not interating
	}
	
	free(keyValue);
	free(statusLine);
		
	return 0;
}

//Accept loop for server
int accept_loop(int sock_fd, char *port, char *hostname, 
		char *path, char * key){
	int new_fd;
	char buff_size[INET6_ADDRSTRLEN];
	
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
		
		inet_ntop(client_addr.ss_family, 
				get_in_addr((struct sockaddr *)&client_addr),
				buff_size, sizeof buff_size);

		printf("Accepted Connection...\n");
		
		accept_fork(sock_fd, new_fd, port, hostname, path, key);
		
		printf("Closed Connection...\n");
  }
	return 0;
}