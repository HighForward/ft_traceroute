#include "../includes/ft_traceroute.h"

int create_udp_socket(traceroute *traceroute, int ttl)
{
    traceroute->udp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (traceroute->udp_socket < 0)
        return str_error("Error: udp socket", -1);

    if (setsockopt(traceroute->udp_socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
        return str_error("Setting socket options to TTL failed!", -1);

    return (1);
}

int create_icmp_socket(traceroute *traceroute)
{
    struct timeval tv_out;

    tv_out.tv_sec = 0;
    tv_out.tv_usec = 50000;

    traceroute->icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    int on = IP_PMTUDISC_DO;

    if (traceroute->icmp_socket < 0)
        return str_error("permission denied, contact your administrator", -1);

    if (setsockopt(traceroute->icmp_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out)) != 0)
        return str_error("Setting socket timeout option failed!", -1);
    setsockopt(traceroute->icmp_socket, SOL_IP, IP_MTU_DISCOVER, &on, sizeof on);
    setsockopt(traceroute->icmp_socket, SOL_IP, IP_RECVERR, &on, sizeof on);
    setsockopt(traceroute->icmp_socket, SOL_IP, IP_RECVTTL, &on, sizeof on);

    return (1);
}