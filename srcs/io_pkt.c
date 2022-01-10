#include "../includes/ft_traceroute.h"

int send_data(traceroute *traceroute, tr_options *tr_options)
{
    char data[] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";

    if (tr_options->ICMP)
    {
        if (sendto(traceroute->send_socket, &traceroute->icmp_send_pkt, sizeof(ICMP_pkt), 0, (struct sockaddr *)&traceroute->addr_host, sizeof(struct sockaddr)) < 0)
            return (-1);
    }
    else
    {
        if (sendto(traceroute->send_socket, data, strlen(data), 0,
                   (const struct sockaddr *) &traceroute->addr_host, sizeof(traceroute->addr_host)) < 0)
            return (-1);
    }

    return (1);
}

int recv_data(traceroute *traceroute, tr_options *tr_options, probe_info *probe_info)
{
    unsigned char pck_reply[(sizeof(struct ip) + 4 + 150 + sizeof(struct ip) + sizeof(struct ICMP_pkt))];
    unsigned int size = sizeof(struct sockaddr_in);

    if (recvfrom(traceroute->icmp_socket, pck_reply, sizeof(pck_reply), 0, (struct sockaddr *) &probe_info->from, &size) >= 0)
    {
        char ip[INET_ADDRSTRLEN];
        bzero(ip, sizeof(ip));

        inet_ntop(probe_info->from.sin_family, &probe_info->from.sin_addr, ip, INET_ADDRSTRLEN);

        //reprint ip if pkt routing change on probes
        if (strcmp(probe_info->ip, ip) != 0)
        {
            strcpy(probe_info->ip, ip);
            printf("%s (%s)  ", probe_info->ip, probe_info->ip);
        }

        //print timestamp
        gettimeofday(&probe_info->end, NULL);

        printf("%.3lf ms  ", (double )(probe_info->end.tv_sec - probe_info->start.tv_sec) + (double )((double )(probe_info->end.tv_usec - probe_info->start.tv_usec)/1000.0));
        if (tr_options->ICMP)
        {
            ICMP_pkt *tmp_ICMP;
            tmp_ICMP = (ICMP_pkt *) (pck_reply + sizeof(struct ip));
            if (tmp_ICMP->hdr.code == 0 && tmp_ICMP->hdr.type == 0)
                probe_info->reached_host = 1;
        }

    }
    else if (errno == EAGAIN)
        printf("* ");

    return (1);
}