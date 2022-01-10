#include "../includes/ft_traceroute.h"

void perform_h()
{
    printf("Usage:\n");
    printf("\ttraceroute [ -hI ] host\n");
    printf("Options:\n");
    printf("\t-h:\t\tRead this help and exit\n");
    printf("\t-I:\t\tUse ICMP ECHO for tracerouting\n");
    printf("\t-q nqueries:\t\tSet the number of probes per each hop. Default is 3\n");
    printf("\t-m max_ttl:\tSet the max number of hops (max TTL to be\n"
           "                              reached). Default is 30\n\n");
    printf("Arguments:\n");
    printf("+\thost\tThe host to traceroute to\n");
    exit(1);
}

int perform_I(tr_options *tr_options)
{
    tr_options->ICMP = 1;
    return (0);
}

int perform_m(tr_options *tr_options, char *val)
{
    if (!val || ft_strlen(val) == 0)
        exit(str_error("Option `-m' requires an argument: `-m max_ttl'", 1));

    int nb = ft_atoi(val);

    if (nb < 1 || nb > 255)
        exit(str_error("max hops cannot be less than 1 or more than 255", 1));

    tr_options->max_hops = nb;
    return (0);
}

int perform_q(tr_options *tr_options, char *val)
{
    if (!val || ft_strlen(val) == 0)
        exit(str_error("Option `-q' requires an argument: `-q nqueries'", 1));

    int nb = ft_atoi(val);

    if (nb < 1 || nb > 10)
        exit(str_error("no more than 10 probes per hop", 1));

    tr_options->nb_probes = nb;

    return (0);
}

int perform_f(tr_options *tr_options, char *val)
{
    if (!val || ft_strlen(val) == 0)
        exit(str_error("Option `-f' requires an argument: `-f first_ttl'", 1));

    int nb = ft_atoi(val);

    tr_options->first_ttl = nb;

    return (0);
}

int parse_args(int argc, char **argv, traceroute *traceroute, tr_options *tr_options)
{
    int i = 0;
    char *arg;
    while (argv[i])
    {
        arg = argv[i];

        if (ft_strlen(arg) == 0 || ft_strncmp(arg, "-h", ft_strlen(arg)) == 0)
            perform_h();
        else if (ft_strncmp(arg, "-I", ft_strlen(arg)) == 0)
            perform_I(tr_options);
        else if (ft_strncmp(arg, "-m", ft_strlen(arg)) == 0)
            perform_m(tr_options, argv[++i]);
        else if (ft_strncmp(arg, "-q", ft_strlen(arg)) == 0)
            perform_q(tr_options, argv[++i]);
        else if (ft_strncmp(arg, "-f", ft_strlen(arg)) == 0)
            perform_f(tr_options, argv[++i]);
        else if (!traceroute->arg_target)
            traceroute->arg_target = argv[i];
        else
            perform_h();
        i++;
    }

    if (!traceroute->arg_target)
    {
        exit(str_error("Specify \"host\" missing argument.", 1));
    }

    if (tr_options->first_ttl > tr_options->max_hops)
        exit(str_error("first hop out of range", 1));

    return (0);
}