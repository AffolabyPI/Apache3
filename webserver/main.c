#include <stdio.h>
#include <string.h>
#include "socket.h"

int main(void){

  int server_socket = create_server(8082);
  if(server_socket == -1){
    perror("Impossible de creer le serveur");
    return -1;
  }

  while (1) {
    accept_client(server_socket);
    char[] string = fgets();
  }
  
  return 0;
}
