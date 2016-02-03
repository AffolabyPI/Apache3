#ifndef __SOCKET_H__
#define __SOCKET_H__

int create_server(int port);
int accept_client(int server_socket);
void close_client(int client_socket);

#endif
