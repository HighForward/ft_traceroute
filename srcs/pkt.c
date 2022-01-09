#include "../includes/ft_traceroute.h"

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;

    if ( len == 1 )
        sum += *(unsigned char*)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return (result);
}

void fill_icmp_packet(ICMP_pkt *ping_pkt)
{
    int i;

    (*ping_pkt).hdr.type = ICMP_ECHO;
    (*ping_pkt).hdr.code = 0;
    (*ping_pkt).hdr.un.echo.id = getpid();
    (*ping_pkt).hdr.un.echo.sequence++;

    for (i = 0; i < sizeof((*ping_pkt).msg) - 1; i++)
        (*ping_pkt).msg[i] = (char)(i + '0');
    (*ping_pkt).msg[i] = 0;

    (*ping_pkt).hdr.checksum = 0;
    (*ping_pkt).hdr.checksum = checksum(&(*ping_pkt), sizeof((*ping_pkt)));
}

int send_data(traceroute *traceroute)
{
//    if (sendto(traceroute->socket_fd, &traceroute->send_pkt, sizeof(ICMP_pkt), 0, (struct sockaddr *)&traceroute->addr_host, sizeof(struct sockaddr)) < 0)
//    {
//        printf("connect: Invalid argument\n");
//        exit(1);
//    }

//    printf("----------\ntype: %d\ncode: %d\nchecksum: %d\nid: %d\nsequence: %d\n----------\n",
//           ping_base->send_pckt.hdr.type, ping_base->send_pckt.hdr.code, ping_base->send_pckt.hdr.checksum, ping_base->send_pckt.hdr.un.echo.id, ping_base->send_pckt.hdr.un.echo.sequence);

//    stats->pck_send++;
    return (0);
}