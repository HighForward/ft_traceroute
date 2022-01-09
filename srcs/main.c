#include "../includes/ft_traceroute.h"

int get_nb_len(unsigned int nb)
{
    int len = 1;
    while (nb > 10)
    {
        nb /= 10;
        len++;
    }
    return len;
}

int send_udp_pkt_probes(traceroute *traceroute, int ttl)
{
    unsigned char pck_reply[(sizeof(struct ip) + 4 + 150 + sizeof(struct ip) + sizeof(struct ICMP_pkt))];
    ft_bzero(&traceroute->send_pkt, sizeof(struct ICMP_pkt));

    struct sockaddr_in from;
    unsigned int size = sizeof(struct sockaddr_in);

    int probes = 3;
    int reply = 0;
    double time[3] = {0, 0, 0};

    printf("%*d ", get_nb_len(traceroute->hops), ttl);

    char string[] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";

    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    gettimeofday(&end, NULL);

    while (probes != 0)
    {

        if (sendto(traceroute->udp_socket, string, strlen(string), 0,
                   (const struct sockaddr *) &traceroute->addr_host, sizeof(traceroute->addr_host)) < 0)
            return (-1);

        gettimeofday(&start, NULL);

        if (recvfrom(traceroute->icmp_socket, pck_reply, sizeof(pck_reply), 0, (struct sockaddr *) &from, &size) >= 0)
        {
            gettimeofday(&end, NULL);

            double before = (double)(((double)(start.tv_sec) * 1000) + ((double)(start.tv_usec) / 1000));
            double after = (double)(((double)(end.tv_sec) * 1000) + ((double)(end.tv_usec) / 1000));
            double now = (after - before) * 100;
            time[reply++] = now;
        }
        probes--;
    }

    if (!reply)
        printf("* * *\n");
    else
    {
        char hit_ip[INET_ADDRSTRLEN];
        inet_ntop(from.sin_family, &from.sin_addr, hit_ip, INET_ADDRSTRLEN);
        printf("%s (%s)  %.3f ms  %.3f ms  %.3f ms\n", hit_ip, hit_ip, time[0], time[1], time[2]);
    }

    return (1);
}

int traceroute_loop(traceroute *traceroute)
{

    struct sockaddr_in from;
    unsigned int size = sizeof(struct sockaddr_in);

    int reply = 0;
    int ttl = 0;

    traceroute->hops = 30;
    unsigned int loop_index = traceroute->hops;

    while (loop_index > 0)
    {
        //increment TTL and set ttl to socket opt
        ttl++;
        if (setsockopt(traceroute->udp_socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
            str_error("Setting socket options to TTL failed!", -1);

        //send three hops and recv icmp response
        send_udp_pkt_probes(traceroute, ttl);

        loop_index--;
    }

    return (0);
}

int main(int argc, char **argv)
{
    traceroute traceroute;
    bzero(&traceroute, sizeof(traceroute));
    traceroute.arg_target = argv[1];

    //create UDP socket for sending packet
    if (create_udp_socket(&traceroute, 80) < 0)
        return (1);

    //create ICMP socket for receiving packet
    if (create_icmp_socket(&traceroute) < 0)
        return (1);

    //resolve host address
    if (resolve_dns(&traceroute))
        return (1);

    //60 byte (IP HEADER + UDP DATAGRAM + 32 BYTE DATA)
    printf("traceroute to %s (%s), 30 hops max, %lu byte packets\n", traceroute.arg_target, traceroute.ip, sizeof(struct ip) + 8 + 32);

    traceroute_loop(&traceroute);

}