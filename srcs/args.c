#include "../includes/ft_traceroute.h"

void perform_h()
{
    printf("Usage:\n");
    printf("\ttraceroute [ -h ] host\n");
    printf("Options:\n");
    printf("\t-h: Read this help and exit\n\n");
    printf("Arguments:\n");
    printf("+\thost\tThe host to traceroute to\n");
}

int parse_arg(int argc, char **argv)
{
    if (argc <= 1)
    {
        perform_h();
        return (-1);
    }

    return (1);
}