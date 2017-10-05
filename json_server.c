/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # json_server.c
  # Functions that create a server-client connection to a json server
*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

 
//Connect to JSON server and receive data
char *receive_json(int sock_fd, char *path, char *hostname){
	int err;
	
	//Create GET Reqest
	char buf[500];
	snprintf(buf, 500, "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection:close\r\n\r\n",
			path, hostname);

	//Send GET Request
	err =	write(sock_fd, buf, strlen(buf));
	if (err == -1) {
		fprintf(stderr, "partial/failed write\n");
		exit(1);
	}
	
	//Read recieved JSON
	char *buffRead = malloc(100000 * sizeof(char));
	if (buffRead == NULL){
		fprintf(stderr, "Malloc Failed\n");
		exit(1);
	}
	err = read(sock_fd, buffRead, 100000);
	if (err == -1) {
		perror("read");
		exit(1);
	}
	
	close(sock_fd);
	
	return buffRead;
}

//Creates new socket for json server connection
int sock_json(char *hostname, char *port){ 
	struct addrinfo hints, *server_info, *current;
	int sock_fd;
	
	if (hostname == NULL){
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int err = getaddrinfo(hostname, port, &hints, &server_info);
	
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
		
		//Connect
		err = connect(sock_fd, current->ai_addr, current->ai_addrlen);
		if (err == -1){
			perror("Connect");
		} 
		
		//Working Connection
		break;
	}
		
	freeaddrinfo(server_info);  //Done with Struct
	
	if (current == NULL){		//No address succeeded 
		fprintf(stderr, "Could not connect to server\n");
		exit(1);
	}
	
	printf("Connection Established...\n");
	
	return sock_fd;	
}