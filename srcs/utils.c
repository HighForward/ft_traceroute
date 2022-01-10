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

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
    unsigned int	i;
    unsigned char	*s;
    unsigned char	*d;

    s = (unsigned char*)s1;
    d = (unsigned char*)s2;
    i = 0;
    while (i < n - 1 && s[i] && d[i] && s[i] == d[i])
        i++;
    if (n > 0)
        return (s[i] - d[i]);
    else
        return (0);
}

int	ft_strlen(const char *str)
{
    int i;

    i = 0;
    if (!str)
        return (0);
    while (str[i])
        i++;
    return (i);
}

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