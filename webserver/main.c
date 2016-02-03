#include <stdio.h>
#include <string.h>
#include "socket.h"

int main(void){

  int server_socket = create_server(8080);
  if(server_socket == -1){
    perror("Impossible de creer le serveur");
    return -1;
  }

  while (1) {
    int client_socket = accept_client(server_socket);
    close_client(client_socket);
  }
  
  return 0;
}
