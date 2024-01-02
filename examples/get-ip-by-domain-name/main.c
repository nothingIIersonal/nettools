#include <stdio.h>

#include <nettools/nettools.h>

#define MAX_IPS 16
#define DOMAIN_NAME "stun2.l.google.com"

int main(int argc, char *argv[]) {
  struct ip_t ips[MAX_IPS];
  memset(ips, 0, sizeof(ips));

  int res = get_ip_by_domain_name(DOMAIN_NAME, ips, MAX_IPS);
  chperror(res, -1, false, "get_ip_by_domain_name()");

  if (!res) {
    printf("No IP found.\n");
  } else {
    for (int i = 0; i < res; ++i) {
      if (!ips[i].type)
        printf("IPv4 of \"%s\" is \"%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8
               "\"\n",
               DOMAIN_NAME, ips[i].ipv4.A, ips[i].ipv4.B, ips[i].ipv4.C,
               ips[i].ipv4.D);
      else
        printf("IPv6 of \"%s\" is \"%.4" PRIx16 ":%.4" PRIx16 ":%.4" PRIx16
               ":%.4" PRIx16 ":%.4" PRIx16 ":%.4" PRIx16 ":%.4" PRIx16
               ":%.4" PRIx16 "\"\n",
               DOMAIN_NAME, ips[i].ipv6.A, ips[i].ipv6.B, ips[i].ipv6.C,
               ips[i].ipv6.D, ips[i].ipv6.E, ips[i].ipv6.F, ips[i].ipv6.G,
               ips[i].ipv6.H);
    }
  }

  return 0;
}