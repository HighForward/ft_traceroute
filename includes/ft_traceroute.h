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
#include <sys/select.h>

#define PING_PACKET_SIZE 56

typedef struct ICMP_pkt
{
    struct icmphdr hdr;
    char msg[PING_PACKET_SIZE - sizeof(struct icmphdr)];

} ICMP_pkt;

typedef struct traceroute {
    int send_socket;
    int icmp_socket;
    char *arg_target;
    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in addr_host;
    struct ICMP_pkt icmp_send_pkt;
} traceroute;

typedef struct tr_options {
    int ICMP;
    int max_hops;
    int nb_probes;
    int first_ttl;
} tr_options;

typedef struct probe_info {
    struct sockaddr_in from;
    struct timeval start;
    struct timeval end;
    char ip[INET_ADDRSTRLEN];
    int reached_host;
} probe_info;

//ARGS
int parse_args(int argc, char **argv, traceroute *traceroute, tr_options *tr_options);


//SOCKET UTILITY
int create_icmp_socket(traceroute *traceroute);
int create_send_socket(traceroute *traceroute, tr_options *tr_options, int ttl);


//DNS
int     resolve_dns(traceroute *traceroute);

//UTILS
int     str_error(char *str, int code);
void	ft_bzero(void *s, size_t n);
int 	ft_strncmp(const char *s1, const char *s2, size_t n);
int 	ft_strlen(const char *str);
int     get_nb_len(unsigned int nb);
int	    ft_atoi(const char *nptr);

//PACKETS
unsigned short checksum(void *b, int len);
void fill_icmp_packet(ICMP_pkt *ping_pkt);

//IO
int send_data(traceroute *traceroute, tr_options *tr_options);
int recv_data(traceroute *traceroute, tr_options *tr_options, probe_info *probe_info);


#endif