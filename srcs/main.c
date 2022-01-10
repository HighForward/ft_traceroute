#include "../includes/ft_traceroute.h"

int send_pkt_probes(traceroute *traceroute, tr_options *tr_options, int ttl)
{

    probe_info probe_info;
    ft_bzero(&probe_info, sizeof(probe_info));

    int reply = 0;
    double time[3] = {0, 0, 0};

    gettimeofday(&probe_info.start, NULL);
    gettimeofday(&probe_info.end, NULL);

    if (tr_options->ICMP)
        fill_icmp_packet(&traceroute->icmp_send_pkt);

    printf("%*d  ", get_nb_len(traceroute->hops), ttl);

    int probes;
    for (probes = 3; probes != 0; probes--)
    {
        if (send_data(traceroute, tr_options) < 0)
            return (-1);

        gettimeofday(&probe_info.start, NULL);

        if (recv_data(traceroute, tr_options, &probe_info) < 0)
            return (-1);
    }

    printf("\n");

    if (probe_info.reached_host)
        exit(0);

    return (1);
}

int traceroute_loop(traceroute *traceroute, tr_options *tr_options)
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
        if (setsockopt(traceroute->send_socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
            str_error("Setting socket options to TTL failed!", -1);

        //send three hops and recv icmp response
        send_pkt_probes(traceroute, tr_options, ttl);

        loop_index--;
    }

    return (0);
}

int main(int argc, char **argv)
{
    traceroute traceroute;
    tr_options tr_options;
    bzero(&traceroute, sizeof(traceroute));
    bzero(&tr_options, sizeof(tr_options));

    if (parse_args(argc, argv + 1, &traceroute, &tr_options) < 0)
        return (1);

    //create UDP socket for sending packet
    if (create_send_socket(&traceroute, &tr_options, 1) < 0)
        return (1);

    //create ICMP socket for receiving packet
    if (create_icmp_socket(&traceroute) < 0)
        return (1);

    //resolve host address
    if (resolve_dns(&traceroute))
        return (1);

    //60 byte (IP HEADER + UDP DATAGRAM + 32 BYTE DATA)
    printf("traceroute to %s (%s), 30 hops max, %lu byte packets\n", traceroute.arg_target, traceroute.ip, sizeof(struct ip) + 8 + 32);

    traceroute_loop(&traceroute, &tr_options);

}