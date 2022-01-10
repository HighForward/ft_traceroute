#include "../includes/ft_traceroute.h"

void perform_h()
{
    printf("Usage:\n");
    printf("\ttraceroute [ -hI ] host\n");
    printf("Options:\n");
    printf("\t-h:\tRead this help and exit\n");
    printf("\t-I:\tUse ICMP ECHO for tracerouting\n\n");
    printf("Arguments:\n");
    printf("+\thost\tThe host to traceroute to\n");
    exit(1);
}

int perform_I(tr_options *tr_options)
{
    tr_options->ICMP = 1;
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
        if (ft_strncmp(arg, "-I", ft_strlen(arg)) == 0)
            perform_I(tr_options);
        else if (!traceroute->arg_target)
            traceroute->arg_target = argv[i];
        else
            perform_h();
        i++;
    }

    if (!traceroute->arg_target)
    {
        printf("target not found\n");
        perform_h();

    }

    return (0);
}