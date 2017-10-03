#ifndef QOTDD_SERVER_H_
#define QOTDD_SERVER_H_

/*Prototypes*/
int new_server();
int new_socket(int sock_fd, int val, struct addrinfo current, int err);
void clean_chld();
int accept_loop(int sock_fd);
void *get_in_addr(struct sockaddr *current_addr);
int accept_fork(int sock_fd, int new_fd);
#endif