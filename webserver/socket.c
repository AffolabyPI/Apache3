#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <signal.h>

int create_server(int port) {
  int server_socket;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1) {
    perror("server_socket");
    return -1;
  }

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
    perror("bind server_socket");
  }

  if (listen(server_socket, 10) == -1) {
    perror("lister server_socket");
  }

  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);

  if(client_socket == -1) {
    perror("accept");
  }

  const char *welcome_message = "Welcome on Apache 3!";
  write(client_socket, welcome_message, strlen(welcome_message));
  
  return server_socket;
}
