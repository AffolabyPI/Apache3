#ifndef __SOCKET_H__
#define __SOCKET_H__

int create_server(int port);

#endif
<<<<<<< HEAD
=======

struct sockaddr_in saddr;
saddr.sin_family = AF_INET ; /* Socket ipv4 */
saddr.sin_port = htons(8080); /* Port d ’ écoute */
saddr.sin_addr.s_addr = INADDR_ANY ; /* écoute sur toutes les interfaces */
if(bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
perror ("bind socker_serveur");
/* traitement de l ’ erreur */
}
>>>>>>> refs/remotes/origin/master
