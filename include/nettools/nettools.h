#ifndef NETTOOLS_H
#define NETTOOLS_H

#include "netstructures.h"
#include "xsocket.h"

// #define __NETTOOLS_PRINT

void cherror(int code, int indicator, bool type, char *str);
int get_ext_ipv4_by_stun(const char *stun_ipv4, uint16_t stun_port,
                         struct ipv4_t *ext_ipv4, port_t *local_port);
int get_ip_by_domain_name(char *domain_name, struct ip_t *ips, int max_ips);

/*
 *
 * Check and print error funtion
 * ---------------------------------------------------
 * code: any code to check and exit for error
 * indicator: code to compare the code being checked
 * type: comparison by inequality or equality
 * str: error string to print
 *
 */
void chperror(int code, int indicator, bool type, char *str) {
  if (type && code != indicator || !type && code == indicator) {
    perror(str);
    exit(code);
  }
}

/*
 *
 * Get external IPv4 address using STUN-server
 * ---------------------------------------------------
 * stun_ip: STUN server IPv4 address
 * stun_port: STUN server port
 * ext_ipv4: result IPv4 address
 * local_port: result local opened port
 *
 * return: error code (0 -> success, -1 -> error) and errno
 * ---------------------------------------------------
 * Based on RFC5389 (https://www.ietf.org/rfc/rfc5389.txt)
 *
 */
int get_ext_ipv4_by_stun(const char *stun_ipv4, uint16_t stun_port,
                         struct ipv4_t *ext_ipv4, port_t *local_port) {
#define STUN_HEADER_SIZE 20
#define STUN_RESPONSE_MAX_SIZE 256

  struct sockaddr_in stun_addr;
  const socklen_t addr_len = sizeof(stun_addr);

  memset(&stun_addr, 0, addr_len);
  stun_addr.sin_family = AF_INET;
  inet_pton(AF_INET, stun_ipv4, &stun_addr.sin_addr);
  stun_addr.sin_port = htons(stun_port);

  if (__xsocket_init()) return -1;

  xsocket_t sockfd = xsocket(stun_addr.sin_family, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd == -1) {
    __xsocket_cleanup();
    return -1;
  }

  int res = xsocket_connect(sockfd, (struct sockaddr *)&stun_addr, addr_len);
  if (res != 0) {
    __xsocket_cleanup();
    return -1;
  }

#ifdef __NETTOOLS_PRINT
  printf("Connection with \"%s:%d\" established\n", stun_ipv4, stun_port);
#endif

  char request_buf[STUN_HEADER_SIZE];
  memset(request_buf, '\0', STUN_HEADER_SIZE);

  // building STUN-request
  *(uint16_t *)request_buf = htons(0x0001);             // STUN Message Type
  *(uint16_t *)(request_buf + 2) = htons(0x0000);       // Message Lenght
  *(uint32_t *)(request_buf + 4) = htonl(0x2112A442);   // Magic Cookie
  *(uint32_t *)(request_buf + 8) = htonl(0x01234567);   // Transaction ID
  *(uint32_t *)(request_buf + 12) = htonl(0x01234567);  // Transaction ID cont.
  *(uint32_t *)(request_buf + 16) = htonl(0x01234567);  // Transaction ID cont.

  res = sendto(sockfd, request_buf, STUN_HEADER_SIZE, 0,
               (struct sockaddr *)&stun_addr, addr_len);
  if (res == -1) {
    xsocket_close(sockfd);
    __xsocket_cleanup();
    return -1;
  }

  char response_buf[STUN_RESPONSE_MAX_SIZE];
  memset(response_buf, '\0', STUN_RESPONSE_MAX_SIZE);
  res = recvfrom(sockfd, response_buf, STUN_RESPONSE_MAX_SIZE, 0, NULL, 0);
  if (res == -1) return -1;

  res = xsocket_close(sockfd);
  __xsocket_cleanup();
  if (res == -1) return -1;

#ifdef __NETTOOLS_PRINT
  printf("Connection closed\n");
#endif

  if (*(uint16_t *)response_buf == htons(0x0101))  // 0x0101 - success response
  {
    uint16_t attribute_type;
    uint16_t attribute_length;

    // viewing all response attributes for IPv4 and PORT (type 0x0020), and
    // skip unnecessary attributes (type = 2 byte, length = 2 byte and value of
    // length field)
    for (size_t i = STUN_HEADER_SIZE; i < STUN_RESPONSE_MAX_SIZE;
         i += attribute_length + 4) {
      attribute_type = htons(*(uint16_t *)(response_buf + i));
      attribute_length = htons(*(uint16_t *)(response_buf + i + 2));

      if (attribute_type == 0x0020)  // this type of attribute indicates that it
                                     // is an "XOR-MAPPED-ADDRESS"
      {
        // port -> "XOR'ing it with the most significant 16 bits of the magic
        // cookie,
        //          and then the converting the result to network byte order"
        *local_port = ntohs(*(uint16_t *)(response_buf + i + 6));
        *local_port ^= 0x2112;

        // ipv4 -> "is computed by taking the mapped IP address in host byte
        // order,
        //          XOR'ing it with the magic cookie, and converting
        //          the result to network byte order"
        ext_ipv4->A = response_buf[i + 8] ^ 0x21;
        ext_ipv4->B = response_buf[i + 9] ^ 0x12;
        ext_ipv4->C = response_buf[i + 10] ^ 0xA4;
        ext_ipv4->D = response_buf[i + 11] ^ 0x42;

        break;
      }
    }

    if (attribute_type != 0x0020) {
#ifdef __NETTOOLS_PRINT
      printf("The server did't provide an IP.\n");
#endif
      return -1;
    }
  } else {
#ifdef __NETTOOLS_PRINT
    printf("The server response is incorrect.\n");
#endif
    return -1;
  }

  return 0;
}

/*
 *
 * Get IP address by domain name
 * ---------------------------------------------------
 * domain_name: domain name
 * ip: result IP address
 * max_ips: maximum number of IP addresses
 *
 * return: number of IP addresses (or -1 -> error) and errno
 *
 */
int get_ip_by_domain_name(char *domain_name, struct ip_t *ips, int max_ips) {
  struct addrinfo hint, *result;
  memset(&hint, 0, sizeof(hint));

  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;

  if (__xsocket_init()) return -1;
  int res = getaddrinfo(domain_name, NULL, &hint, &result);
  if (res != 0) {
    __xsocket_cleanup();
    return -1;
  }

  struct addrinfo *addrinfo_i = result;
  void *tmp_addr = NULL;

  int ips_counter = 0;

  while (addrinfo_i != NULL && ips_counter < max_ips) {
    if (addrinfo_i->ai_family == AF_INET) {
      tmp_addr = &((struct sockaddr_in *)addrinfo_i->ai_addr)->sin_addr;

      ips[ips_counter].type = false;

      ips[ips_counter].ipv4.A = *((uint32_t *)tmp_addr);
      ips[ips_counter].ipv4.B = *((uint32_t *)tmp_addr) >> 8;
      ips[ips_counter].ipv4.C = *((uint32_t *)tmp_addr) >> 16;
      ips[ips_counter].ipv4.D = *((uint32_t *)tmp_addr) >> 24;

      ++ips_counter;
    } else if (addrinfo_i->ai_family == AF_INET6) {
      tmp_addr = &((struct sockaddr_in6 *)addrinfo_i->ai_addr)->sin6_addr;

      ips[ips_counter].type = true;

      ips[ips_counter].ipv6.A = htons(((uint16_t *)tmp_addr)[0]);
      ips[ips_counter].ipv6.B = htons(((uint16_t *)tmp_addr)[1]);
      ips[ips_counter].ipv6.C = htons(((uint16_t *)tmp_addr)[2]);
      ips[ips_counter].ipv6.D = htons(((uint16_t *)tmp_addr)[3]);
      ips[ips_counter].ipv6.E = htons(((uint16_t *)tmp_addr)[4]);
      ips[ips_counter].ipv6.F = htons(((uint16_t *)tmp_addr)[5]);
      ips[ips_counter].ipv6.G = htons(((uint16_t *)tmp_addr)[6]);
      ips[ips_counter].ipv6.H = htons(((uint16_t *)tmp_addr)[7]);

      ++ips_counter;
    }

    addrinfo_i = addrinfo_i->ai_next;
  }

  freeaddrinfo(result);
  __xsocket_cleanup();

  return ips_counter;
}

#endif