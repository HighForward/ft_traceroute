#include "../includes/ft_traceroute.h"

int create_socket(int *socket_fd, int ttl)
{
    struct timeval tv_out;

    tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;

    (*socket_fd) = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if ((*socket_fd) < 0)
        return str_error("permission denied, contact your administrator", -1);
    if (setsockopt((*socket_fd), SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
        return str_error("Setting socket options to TTL failed!", -1);
    if (setsockopt((*socket_fd), SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof(tv_out)) != 0)
        return str_error("Setting socket timeout option failed!", -1);

    return (1);
}