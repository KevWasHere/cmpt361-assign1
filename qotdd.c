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
#include <signal.h>

//Have main on bottom. Break up code later
//http://beej.us/guide/bgnet/output/html/multipage/index.html

int main() {
  //Standard port if no port is specified
  char *port = "1700";
  
  struct addrinfo hints, *servinfo;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int err = getaddrinfo(NULL, port, &hints, &servinfo);
  if (err != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    return 1; //Return anything but 0 means error. Might change later
  }

  return 0;
}
