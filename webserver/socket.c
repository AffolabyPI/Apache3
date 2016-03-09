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
#include <errno.h>

const char *welcome_message = "Welcome on Apache 3 !!! Le serveur du futur !\n Il a ete entierement realiser suite a des \netudes francaise que j'ai realise moi-meme car je sui \nfrancais, prenant en compte l'influence d'apache dans le \nmonde des devellopeurs de l'IUT de Lille A au seins de la \npromo Promo N4P2. Ce serveur est concus par les meilleurs \netudiants de la promotion, a savoir : Paul-Ivan Affolaby, \nexpert monetaire doue d'un esprit de chef d'equipe, Kevin \nMessien, technicien de pointe capable de realiser des chmod \nincongru (notemment pour se retirer ses propres droits de \nfacon recursif), son genie reste inegale, et enfin Florian \nMardon, futur ingenieur expert en lardon et en pate-raclette";
const char *apache3 = "<Apache3>";
const char *error_400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\r\n400 Bad request";
const char *error_404 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 13\r\n\r\n404 Not Found";
const char *message_200 = "HTTP/1.1 200 Ok\r\n";
const char *message_size = "Content-Length: ";

void deal_signal(int sig){
  printf("Signal %d recu\n", sig);
  wait(&sig);
  if(WIFSIGNALED(sig)) {
    printf("Tue par signal %d\n", WTERMSIG(sig));
  }
}

void init_signals(void) {
  struct sigaction sa;
  sa.sa_handler = deal_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction(SIGCHLD)");
  }
}

int create_server(int port) {
  int server_socket;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == -1) {
    perror("server_socket");
    return -1;
  }

  int optval = 1;
  if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1){
   perror("Can not set SO_REUSEADDR option");
 }

 init_signals();

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

return server_socket;
}

void fgets_or_exit(char *buffer, int size, FILE *stream){
  if(fgets(buffer, size, stream) == NULL){
    perror("Cannot read");
    exit(1);
  }
  printf("%s", buffer);
}

int check_client_header(FILE *file) {
  char buffer[1024];

  fgets_or_exit(buffer, 1024, file);

  char *c = buffer;
  char *words[2];

  if (strlen(c) < 3 && (c[0] != 'G' || c[1] != 'E' || c[2] != 'T')) {
    return 400;
  }

  int word = 0;

  while(c[0] != '\0') {
   if(c[0] == ' ') {
     if (word == 2) {
       return 400;
     }
     words[word] = c;
     word ++;
   }
   c++;
 }

 if (strcmp(" HTTP/1.0\r\n", words[1]) != 0 && strcmp(" HTTP/1.1\r\n", words[1]) != 0) {
  return 400;
}

if (words[0][1] != '/' || words[0][2] != ' ') {
 return 404;
}

return 200;
}

int accept_client(int server_socket) {
  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);
  
  if(client_socket == -1) {
    perror("accept");
    return -1;
  }

  if(fork() == 0){
    FILE *file = fdopen(client_socket, "w+");

    int headerError = 0;
    headerError = check_client_header(file);


    char buffer[1024];

    int fin = 0;

    while (fin == 0) {
      fgets_or_exit(buffer, 1024, file);
      if (strcmp(buffer, "\r\n") == 0 || strcmp(buffer, "\n") == 0) {
        fin = 1;
      }
    }

    if (headerError == 400) {
      fprintf(file, "%s", error_400);
    } else if (headerError == 404) {
      fprintf(file, "%s", error_404);
    } else if (headerError == 200) {
      fprintf(file, "%s%s%d\r\n\r\n%s %s", message_200, message_size, (int) (strlen(apache3) + strlen(welcome_message) + 3), apache3, welcome_message);
    } 

    fflush(file);   
    close(client_socket);
    close(server_socket);
    exit(1);
  }
  close(client_socket);

  return 0;
}

int parse_http_request(const char *request_line, http_request *request){
  int ok;
  token = strtok(request_line, " ");
  while(token){
      ++tkCounter;
      if(tkCounter==1 && strcmp(token,"GET")!=0){
        ok = 0;
      }
      if(tkCounter==2 && strcmp(token, "/")!=0){
        ok = 1;
      }       
      if(tkCounter > 3){
        ok = 0;
      }
      if(tkCounter == 3 && strcmp(token,"HTTP/1.0\r\n")!=0 && strcmp(token,"HTTP/1.1\r\n")!=0){
        ok = 0;
      }
      token=strtok(NULL," ");
  } 
  if(tkCounter < 3 ){
    ok = 0;
  }
  tkCounter=0;
  while(ok==1 && ligneVide==0){
    str = fgets_or_exit(buffer, 80, file);
    if(str!= NULL && (strcmp(str, "\r\n")==0 || strcmp(str, "\n")==0)){
      ligneVide=1;
    }
  }
  return ok;
}

void close_client(int client_socket) {
  close(client_socket);
}