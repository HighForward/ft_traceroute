#ifndef FT_TRACEROUTE
#define FT_TRACEROUTE

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PING_PACKET_SIZE 56

typedef struct ICMP_pkt
{
    struct icmphdr hdr;
    char msg[PING_PACKET_SIZE - sizeof(struct icmphdr)];

} ICMP_pkt;

typedef struct traceroute {
    int socket_fd;
    char *arg_target;
    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in addr_host;
    struct ICMP_pkt send_pkt;
} traceroute;

//SOCKET UTILITY
int     create_socket(int *sockfd, int ttl);

//DNS
int     resolve_dns(traceroute *traceroute);

//PACKET
void fill_icmp_packet(ICMP_pkt *ping_pkt);
int send_data(traceroute *traceroute);

//UTILS
int     str_error(char *str, int code);
void	ft_bzero(void *s, size_t n);


#endif