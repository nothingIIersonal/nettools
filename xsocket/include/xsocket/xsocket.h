#ifndef CROSSSOCKET_H
#define CROSSSOCKET_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#endif
#include <inttypes.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef _WIN32
#include <Ws2tcpip.h>
#include <winsock2.h>

#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#endif

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "AdvApi32.lib")
#endif

#ifdef _WIN32
typedef SOCKET xsocket_t;
#else
typedef int xsocket_t;
#endif

int __xsocket_init(void);
int __xsocket_cleanup(void);
xsocket_t xsocket(int domain, int type, int protocol);
int xsocket_connect(xsocket_t sockfd, const struct sockaddr *addr,
                    socklen_t addrlen);
int xsocket_close(xsocket_t sockfd);

#endif