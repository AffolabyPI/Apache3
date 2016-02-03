#ifndef __SOCKET_H__
#define __SOCKET_H__

int create_server(int port);

#endif

struct sockaddr_in { 
sa_family_t sin_family; /* address family: AF_INET */ 
in_port_t sin_port; /* port in network byte order */ 
struct in_addr sin_addr; /* internet address */ 
};

struct in_addr { 
uint32_t s_addr; /* address in network byte order */ 
}; 