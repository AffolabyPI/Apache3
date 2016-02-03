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

  const char *welcome_message = "Welcome on Apache 3 !!! Le serveur du futur ! Il a été entierement realiser suite a des etudes francaise que j'ai realise moi-meme car je sui francais, prenant en compte l'influence d'apache dans le monde des devellopeurs de l'IUT de Lille A au seins de la promo Promo N4P2. Ce serveur est concus par les meilleurs etudiants de la promotion, a savoir : Paul-Ivan Affolaby, expert monetaire doue d'un esprit de chef d'equipe, Kevin Messien, technicien de pointe capable de realiser des chmod incongru (notemment pour se retirer ses propres droits de facon recursif), son genie reste inegale, et enfin Florian Mardon, futur ingenieur expert en lardon et en pate-raclette";

  while (1) {
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);


  write(client_socket, welcome_message, strlen(welcome_message));

    if(client_socket == -1) {
      perror("accept");
    }
    
    write(client_socket, welcome_message, strlen(welcome_message));
  }

  int optval = 1;
  if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1){
     perror("Can not set SO_REUSEADDR option");
  }
  
  return server_socket;
}
