/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # json_server.c
  # Functions that create a server-client connection to a json server
*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
//#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


//Break this up into get, http, and content
char *header = "GET /advice HTTP/1.1\r\nHost: api.adviceslip.com\r\nConnection:close\r\n\r\n";
 
 //Client to JSON - Socket, Connect, Close
//Write then read 
//https://github.com/zserge/jsmn/blob/master/example/simple.c
//Create second hint with ai_protocol = IPPROT_TCP; ai_family = AF_INET6
//getaddrinfo(host, 80, &hints, &server_info);
//snprintf(buffer, size, "GET / HTTP/1.1\r\nHost: date.jsontest.com\r\nConnection:close\r\n\r\n"
//write(socket, buff, size)
//int b = read(socket, buff, size); //b is status



char *receive_json(int sock_fd, char *path, char *hostname){
	int err;
	
	//Create GET Reqest
	//char *mystr = "GET /advice HTTP/1.1\r\nHost: api.adviceslip.com\r\nConnection:close\r\n\r\n";
	char buf[500];
	snprintf(buf, 500, "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection:close\r\n\r\n", path, hostname);
	
	//CHANGE THIS TO MALLOC LATER!!!!!
	
	
	//Send GET Request
	//write(sock_fd, mystr, strlen(mystr));
	err =	write(sock_fd, buf, strlen(buf));
	if (err == -1) {
		fprintf(stderr, "partial/failed write\n");
		exit(1);
	}
	
	//Read recieved JSON
	char *buffRead = malloc(100000 * sizeof(char));
	err = read(sock_fd, buffRead, 100000);
	if (err == -1) {
		perror("read");
		exit(1);
	}
	
	//printf("%s\n", buffRead);
	
	close(sock_fd);
	
	return buffRead;
}



//Creates new socket for json server connection
//INCLUDE PORT VARIABLE LATER FOR DIFFERNET PORT
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
	//hints.ai_flags = AI_PASSIVE;
	//hints.ai_protocol = IPPROTO_TCP;

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
	
	//receive_json(sock_fd, path, hostname);
	
	return sock_fd;	
}
	
	
	
	
	
	
	
	
	