#ifndef NETTOOLS_H
#define NETTOOLS_H

#include <nettools/netstructures.h>
#include <xsocket/xsocket.h>

// #define __NETTOOLS_PRINT

void chperror(int code, int indicator, bool type, char *str);
int get_ext_ipv4_by_stun(const char *stun_ipv4, uint16_t stun_port,
                         struct ipv4_t *ext_ipv4, port_t *local_port);
int get_ip_by_domain_name(char *domain_name, struct ip_t *ips, int max_ips);

#endif