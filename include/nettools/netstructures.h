#ifndef NETSTRUCTURES_H
#define NETSTRUCTURES_H

#include <inttypes.h>
#include <stdbool.h>

struct ipv4_t {
  uint8_t A;
  uint8_t B;
  uint8_t C;
  uint8_t D;
};

struct ipv6_t {
  uint16_t A;
  uint16_t B;
  uint16_t C;
  uint16_t D;
  uint16_t E;
  uint16_t F;
  uint16_t G;
  uint16_t H;
};

struct ip_t {
  bool type;  // false -> IPv4, true -> IPv6

  union {
    struct ipv4_t ipv4;
    struct ipv6_t ipv6;
  };
};

typedef uint16_t port_t;

#endif