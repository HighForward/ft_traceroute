#include "../includes/ft_traceroute.h"

int resolve_dns(traceroute *traceroute)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_CANONNAME;

    struct addrinfo *result;

    if (getaddrinfo(traceroute->arg_target, "33434", &hints, &result) != 0)
    {
        printf("ft_ping: %s: Name or service not known\n", traceroute->arg_target);
        return (1);
    }

    char *ip = malloc(sizeof(char) * (INET_ADDRSTRLEN + 1));
    ft_bzero(ip, INET_ADDRSTRLEN + 1);

    //HOST FOUND
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;

    //change this for autorized function
    void *addr = &(ipv4->sin_addr);
    inet_ntop(result->ai_family, addr, traceroute->ip, INET_ADDRSTRLEN);
    freeaddrinfo(result);

    //save host sockaddr_in
    traceroute->addr_host = (*ipv4);
    return (0);
}
