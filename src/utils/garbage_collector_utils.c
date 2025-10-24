/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:41:26 by macaruan          #+#    #+#             */
/*   Updated: 2025/09/29 18:41:55 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*gc_strjoin_char(char const *s1, char const c)
{
	char	s2[2];
	char	*s3;

	s2[0] = c;
	s2[1] = '\0';
	s3 = (char *)gc_malloc(ft_strlen(s1) + 2);
	if (!s3)
		return (NULL);
	ft_strlcpy(s3, s1, ft_strlen(s1) + 1);
	ft_strlcat(s3, s2, ft_strlen(s1) + 2);
	return (s3);
}

char	*gc_strjoin_len(char *s1, const char *s2, size_t len)
{
	char	*part;
	char	*joined;

	part = ft_substr(s2, 0, len);
	joined = gc_strjoin(s1, part);
	free(part);
	return (joined);
}

char	*gc_strdup_until_charset(const char *src, const char *charset)
{
	int		i;
	char	*res;

	i = 0;
	while (src[i] && !ft_strchr(charset, src[i]))
		i++;
	res = gc_malloc(i + 1);
	ft_strlcpy(res, src, i + 1);
	return (res);
}

static size_t	nbr_len(int nb)
{
	long	nbr;
	size_t	c;

	nbr = nb;
	c = 0;
	if (nbr < 0)
		nbr = nbr * (-1);
	if (nbr >= 10)
		c = nbr_len(nbr / 10);
	c++;
	return (c);
}

char	*gc_itoa(int n)
{
	size_t	i;
	size_t	len;
	long	nbr;
	char	*cn;

	nbr = n;
	len = nbr_len(n);
	if (n < 0)
		len += 1;
	i = len;
	cn = gc_malloc(len + 1);
	if (!cn)
		return (NULL);
	if (nbr < 0)
		nbr = -nbr;
	while (i--)
	{
		cn[i] = nbr % 10 + '0';
		nbr = nbr / 10;
	}
	if (n < 0)
		cn[0] = '-';
	cn[len] = '\0';
	return (cn);
}
