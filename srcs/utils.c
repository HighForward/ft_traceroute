#include "../includes/ft_traceroute.h"

int str_error(char *str, int code)
{
    printf("%s\n", str);
    return (code);
}

void	ft_bzero(void *s, size_t n)
{
    unsigned char	*d;
    unsigned int	i;

    d = (unsigned char*)s;
    i = 0;
    while (i < n)
    {
        d[i] = 0;
        i++;
    }
    s = (void*)d;
}