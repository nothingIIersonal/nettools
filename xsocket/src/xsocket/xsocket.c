#include <xsocket/xsocket.h>

/*
 * WIN32 specified initialization
 */
int __xsocket_init(void) {
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
int __xsocket_cleanup(void) {
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
xsocket_t xsocket(int domain, int type, int protocol) {
  return socket(domain, type, protocol);
}

/*
 * Socket connect
 */
int xsocket_connect(xsocket_t sockfd, const struct sockaddr *addr,
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
int xsocket_close(xsocket_t sockfd) {
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
