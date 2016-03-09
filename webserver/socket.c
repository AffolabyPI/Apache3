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
#include "socket.h"

const char *welcome_message = "Welcome on Apache 3 !!! Le serveur du futur !\r\n Il a ete entierement realiser suite a des \r\netudes francaise que j'ai realise moi-meme car je sui \r\nfrancais, prenant en compte l'influence d'apache dans le \r\nmonde des devellopeurs de l'IUT de Lille A au seins de la \r\npromo Promo N4P2. Ce serveur est concus par les meilleurs \r\netudiants de la promotion, a savoir : Paul-Ivan Affolaby, \r\nexpert monetaire doue d'un esprit de chef d'equipe, Kevin \r\nMessien, technicien de pointe capable de realiser des chmod \r\nincongru (notemment pour se retirer ses propres droits de \r\nfacon recursif), son genie reste inegale, et enfin Florian \r\nMardon, futur ingenieur expert en lardon et en pate-raclette\r\n";
const char *apache3 = "<Apache3>";
const char *error_400 = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 19\r\n\r\n400 Bad request\r\n";
const char *error_404 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: 15\r\n\r\n404 Not Found\r\n";
const char *message_200 = "HTTP/1.1 200 Ok\r\n";
const char *message_size = "Content-Length: ";
const char *http_version = "HTTP-1.1";
const char *content_length = "Content-Length:";
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
}

int parse_http_request(char *request_line, http_request *request){
  char* token = strtok(request_line, " ");
  int line = 0;
  while(token != NULL) {
    if (line == 0 && strcmp("GET", token) == 0) {
      request->method = HTTP_GET;
    } else if (line == 0) {
      request->method = HTTP_UNSUPPORTED;
    } else if (line == 1) {
      request->url = token;
    } else if (line == 2) {
      if (strcmp(" HTTP/1.0\r\n", token) == 0) {
       request->major_version = 1;
       request->minor_version = 0;
     } else if (strcmp(" HTTP/1.1\r\n", token) != 0) {
       request->major_version = 1;
       request->minor_version = 1;
     } else {
       return 0;
     }
   } else {
    return 0;
  }
  line++;
  token = strtok(NULL, " ");
}
return line == 3;
}

void skip_headers(FILE *file){
  char buffer[1024];           

  while (1) {
    fgets_or_exit(buffer, 1024, file);
    if (strcmp(buffer, "\r\n") == 0 || strcmp(buffer, "\n") == 0) {
      return;
    }
  }
}

void send_status (FILE *client, int code, const char *reason_phrase){
   char buf[128];

  sprintf(buf, "%d %s\r\n", code, reason_phrase);
  fprintf(client, "%s %d %s\r\nConnection: close\r\n%s %d\r\n\r\n%s", http_version, code, reason_phrase, content_length, (int) (strlen(buf) + 1), buf);
}

int check_client_header(FILE *file) {
  char buffer[1024];
  http_request request;

  fgets_or_exit(buffer, 1024, file);

  if (parse_http_request(buffer, &request) == 1) {
    if (request.method != HTTP_GET) {
      skip_headers(file);
      send_status(file, 400, "Bad request");
      return 400;
    } else if (strcmp(request.url, "/")) {
      skip_headers(file);
      send_status(file, 404, "Not Found");
      return 404;
    } else {
      skip_headers(file);
      //send_status(file, 200, "Ok");
      return 200;
    }
  } else {
    skip_headers(file);
    send_status(file, 400, "Bad Request");
    return 400;
  }
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

    if (headerError == 200) {
      fprintf(file, "%s%s%d\r\n\r\n%s %s", message_200, message_size, (int) (strlen(apache3) + strlen(welcome_message) + 1), apache3, welcome_message);
    }

    fflush(file);   
    close(client_socket);
    close(server_socket);
    exit(1);
  }
  close(client_socket);

  return 0;
}

void close_client(int client_socket) {
  close(client_socket);
}
