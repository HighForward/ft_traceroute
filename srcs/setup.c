#include "../includes/ft_traceroute.h"

int create_send_socket(traceroute *traceroute, tr_options *tr_options, int ttl)
{

    if (tr_options->ICMP)
        traceroute->send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    else
        traceroute->send_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (traceroute->send_socket < 0)
        return str_error("permission denied, contact your administrator", -1);

    if (setsockopt(traceroute->send_socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
        return str_error("Setting socket options to TTL failed!", -1);

    return (1);
}

int create_icmp_socket(traceroute *traceroute)
{
    struct timeval tv_out;

    tv_out.tv_sec = 0;
    tv_out.tv_usec = 50000;

    traceroute->icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (traceroute->icmp_socket < 0)
        return str_error("permission denied, contact your administrator", -1);

    if (setsockopt(traceroute->icmp_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out)) != 0)
        return str_error("Setting socket timeout option failed!", -1);

    return (1);
}