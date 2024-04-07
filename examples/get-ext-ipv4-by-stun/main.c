#include <nettools/nettools.h>
#include <stdio.h>

#define STUN_DOMAIN "stun2.l.google.com"
#define STUN_IP "74.125.204.127"
#define STUN_PORT 19302

int main(int argc, char *argv[]) {
  struct ipv4_t ext_ipv4 = {0};
  port_t local_port = 0;

  {  // using STUN domain name
    printf("Using STUN domain name \"%s\":\n", STUN_DOMAIN);

    struct ip_t ip;
    memset(&ip, 0, sizeof(ip));

    int res = get_ip_by_domain_name(STUN_DOMAIN, &ip, 1);
    chperror(res, -1, false, "get_ip_by_domain_name()");

    char stun_ip[16];
    memset(stun_ip, 0, sizeof(stun_ip));

#ifdef _WIN32
    sprintf_s(stun_ip, sizeof(stun_ip),
              "%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8, ip.ipv4.A, ip.ipv4.B,
              ip.ipv4.C, ip.ipv4.D);
#else
    sprintf(stun_ip, "%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8, ip.ipv4.A,
            ip.ipv4.B, ip.ipv4.C, ip.ipv4.D);
#endif

    res = get_ext_ipv4_by_stun(stun_ip, STUN_PORT, &ext_ipv4, &local_port);
    chperror(res, 0, true, "get_ext_ipv4_by_stun()");

    printf("\tYour external IPv4 and local opened PORT is \"%" PRIu8 ".%" PRIu8
           ".%" PRIu8 ".%" PRIu8 ":%" PRIu16 "\"\n",
           ext_ipv4.A, ext_ipv4.B, ext_ipv4.C, ext_ipv4.D, local_port);
  }

  {  // using STUN IPv4
    printf("Using STUN IPv4 address \"%s\":\n", STUN_IP);

    int res = get_ext_ipv4_by_stun(STUN_IP, STUN_PORT, &ext_ipv4, &local_port);
    chperror(res, 0, true, "get_ext_ipv4_by_stun()");

    printf("\tYour external IPv4 and local opened PORT is \"%" PRIu8 ".%" PRIu8
           ".%" PRIu8 ".%" PRIu8 ":%" PRIu16 "\"\n",
           ext_ipv4.A, ext_ipv4.B, ext_ipv4.C, ext_ipv4.D, local_port);
  }

  return 0;
}