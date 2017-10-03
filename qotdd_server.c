/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # qotdd_server.c
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

//REMOVE SOME OF THESE INCLUDES LATER!!!!

#define BACKLOG 20 //20 Pending connections allowed 

//Creates a new socket
/*int new_socket(int sock_fd, int val, struct addrinfo current, int err){
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
	return sock_fd;
}*/

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
	if(current_addr->sa_family == PF_INET){
		return &(((struct sockaddr_in*)current_addr)->sin_addr);
	}else if(current_addr->sa_family == PF_INET6){
		return &(((struct sockaddr_in6*)current_addr)->sin6_addr);
	}else{
		fprintf(stderr, "Unexpected address family for client\n");
		exit(EXIT_FAILURE);
	}	
}

//Creates new server for client-server connection
int new_server(){
	//Standard port if no port is specified
  char *port = "7890";  //CHANGE THIS LATER TO 1700
	
	struct addrinfo hints, *server_info, *current;
	int sock_fd;
	int val = 1;

	memset(&hints, 0, sizeof(hints));

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
			
			//new_socket(sock_fd, val, *current, err);
			
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
int accept_fork(int sock_fd, int new_fd){
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
	return 0;
}

//Accept loop for server
int accept_loop(int sock_fd){
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
		
		//char buff_size[INET6_ADDRSTRLEN];
		inet_ntop(client_addr.ss_family, 
				get_in_addr((struct sockaddr *)&client_addr),
				buff_size, sizeof buff_size);

		printf("Accepted Connection...\n");
		
		accept_fork(sock_fd, new_fd);
		
		printf("Closed Connection...\n");
  }
	return 0;
}