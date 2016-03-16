#ifndef __SOCKET_H__
#define __SOCKET_H__

int create_server(int port);
int accept_client(int server_socket);
void close_client(int client_socket);

struct mime{
	const char * ext;
	const char * mime;
};

struct mime tm[]{
	{"png","image/png"},
	{"html","text/html"},
	{NULL,NULL}
};

enum http_method {
	HTTP_GET ,
	HTTP_UNSUPPORTED ,
};
typedef struct {
	enum http_method method ;
	int major_version ;
	int minor_version ;
	char * url ;
} http_request ;

#endif