#include "../includes/ft_traceroute.h"

void traceroute_loop(traceroute *traceroute)
{
//    struct ICMP_pckt *tmp_icmp;
    unsigned char pck_reply[(sizeof(struct ip) + 4 + 150 + sizeof(struct ip) + sizeof(struct ICMP_pkt))];
    ft_bzero(&traceroute->send_pkt, sizeof(struct ICMP_pkt));

    int reply = 0;
    int hints = 1;
    while (reply == 0)
    {
        ft_bzero(pck_reply, sizeof(pck_reply));

        fill_icmp_packet(&traceroute->send_pkt);

        send_data(traceroute);

        struct sockaddr_in from;
        unsigned int size = sizeof(struct sockaddr_in);


        if (recvfrom(traceroute->socket_fd, pck_reply, sizeof(pck_reply), 0, (struct sockaddr *) &from, &size) >= 0)
        {
            reply = 1;
        } else
        {
            printf("%s\n", strerror(errno));
        }

        if (reply)
        {
            struct ICMP_pkt *tmp = (ICMP_pkt *) (pck_reply + sizeof(struct ip));
            printf("bytes from %s (%s): icmp_seq=%d ttl=%d\n", traceroute->arg_target, traceroute->ip,
                   tmp->hdr.un.echo.sequence, 64);
            printf("%d\n", tmp->hdr.un.echo.id);
        }

        hints++;

        setsockopt(traceroute->socket_fd, SOL_IP, IP_TTL, &hints, sizeof(hints));
//            return str_error("Setting socket options to TTL failed!", -1);

        if (hints >= 20)
        {
            printf("break");
            break;
        }
    }


}

int main(int argc, char **argv)
{
    traceroute traceroute;
    traceroute.arg_target = argv[1];

    if (create_socket(&traceroute.socket_fd, 7) < 0)
        return (1);

    if (resolve_dns(&traceroute))
        return (1);

    traceroute_loop(&traceroute);

    printf("ip hit: %s for %s\n", traceroute.ip, traceroute.arg_target);
}