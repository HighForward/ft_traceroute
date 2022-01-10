#include "../includes/ft_traceroute.h"

int send_pkt_probes(traceroute *traceroute, tr_options *tr_options, int ttl)
{
    probe_info probe_info;
    ft_bzero(&probe_info, sizeof(probe_info));

    gettimeofday(&probe_info.start, NULL);
    gettimeofday(&probe_info.end, NULL);

    if (tr_options->ICMP)
        fill_icmp_packet(&traceroute->icmp_send_pkt);

    printf("%*d  ", get_nb_len(tr_options->max_hops), ttl);

    int probes;
    for (probes = tr_options->nb_probes; probes != 0; probes--)
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

    int ttl = tr_options->first_ttl;
    unsigned int loop_index = tr_options->max_hops;

    while (ttl <= tr_options->max_hops)
    {
        //increment TTL and set ttl to socket opt
        if (setsockopt(traceroute->send_socket, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
            str_error("Setting socket options to TTL failed!", -1);
        //send three hops and recv icmp response
        send_pkt_probes(traceroute, tr_options, ttl);
        ttl++;
    }

    return (0);
}

int main(int argc, char **argv)
{
    traceroute traceroute;
    tr_options tr_options;
    bzero(&traceroute, sizeof(traceroute));
    bzero(&tr_options, sizeof(tr_options));

    tr_options.max_hops = 30;
    tr_options.nb_probes = 3;
    tr_options.first_ttl = 1;

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
    printf("traceroute to %s (%s), %d hops max, %lu byte packets\n", traceroute.arg_target, traceroute.ip, tr_options.max_hops, sizeof(struct ip) + 8 + 32);

    traceroute_loop(&traceroute, &tr_options);

}