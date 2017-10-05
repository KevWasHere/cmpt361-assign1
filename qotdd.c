/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # qotdd.c
  # Calls functions to run qotdd
*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include "qotdd_server.h"

/*Reference
Used for SIGCLD Handler
http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html

Used to convert jsmn tokens
https://stackoverflow.com/questions/14388692/how-to-parse-a-small-json-file-with-jsmn-on-an-embedded-system
*/

//Prints usage message of the program and exits
void usage(char *progname){
	char *base;
	base = basename(progname);
  printf("usage: %s host[:port]/path key\nDefault JSON Server Port - 80\n"
			"Client Port - 1700\n", base);
	exit(1);
}

int main(int argc, char *argv[]){
	//Standard port if no port is specified
	char *port = "80";
	char *hostname = "";
	char *path = "";
	char *fullHost = argv[1];
	char *key = argv[2];
	
	if (argc !=3){
		usage(argv[0]);
		exit(0);
	}

	//Parse commandline argument 
	//Check if port exist. Token hostname, path and port (if exist)
	if (strstr(fullHost, ":") != NULL){
		hostname = strtok(fullHost, ":/");
		port = strtok(NULL, ":/");	
		path = strtok(NULL, ":/");
	} 
	//Port doesn't exist. Token hostname and path
	else {
		hostname = strtok(fullHost, "/");
		path = strtok(NULL, "/");
	}
	
  int sock_fd;
	sock_fd = new_qotdd_server();
	clean_chld();
	accept_loop(sock_fd, port, hostname, path, key);
	
  return 0;	
}