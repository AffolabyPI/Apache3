#include <stdio.h>
#include <string.h>
#include "socket.h"

int main(void){
  
  if(create_server(8080) == -1){
    perror("Impossible de créer le serveur");
    return -1;
  }
  
  return 0;
}
