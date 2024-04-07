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

static int __xsocket_init();
static int __xsocket_cleanup();
static xsocket_t xsocket(int domain, int type, int protocol);
static __attribute_maybe_unused__ int xsocket_connect(xsocket_t sockfd, const struct sockaddr *addr,
                           socklen_t addrlen);
static int xsocket_close(xsocket_t sockfd);

/*
 * WIN32 specified initialization
 */
static int __xsocket_init() {
#ifdef _WIN32
  WSADATA wsa_data;
  return WSAStartup(MAKEWORD(2, 2), &wsa_data);
#else
  return 0;
#endif
}

/*
 * WIN32 specified cleanup
 */
static int __xsocket_cleanup() {
#ifdef _WIN32
  return WSACleanup();
#else
  return 0;
#endif
}

/*
 * Create and return socket
 * Arguments are standard
 */
static xsocket_t xsocket(int domain, int type, int protocol) {
  return socket(domain, type, protocol);
}

/*
 * Socket connect
 */
static int xsocket_connect(xsocket_t sockfd, const struct sockaddr *addr,
                           socklen_t addrlen) {
  int res = connect(sockfd, addr, addrlen);
#ifdef _WIN32
  if (res != NO_ERROR)
    return -1;
#endif
  return res;
}

/*
 * Close socket
 */
static int xsocket_close(xsocket_t sockfd) {
  int status = 0;
#ifdef _WIN32
  status = shutdown(sockfd, SD_BOTH);
  if (status != NO_ERROR)
    status = closesocket(sockfd);
#else
  status = shutdown(sockfd, SHUT_RDWR);
  if (!status)
    status = close(sockfd);
#endif
  return status;
}

#endif