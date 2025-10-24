/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:37:11 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 16:28:46 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	size_t	len1;
	size_t	len2;
	size_t	len3;
	size_t	i;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	i = 0;
	res = gc_malloc(len1 + len2 + len3 + 1);
	if (!res)
		return (NULL);
	while (*s1)
		res[i++] = *s1++;
	while (*s2)
		res[i++] = *s2++;
	while (*s3)
		res[i++] = *s3++;
	res[i] = '\0';
	return (res);
}

void	cleanup_and_exit(int code, int do_exit)
{
	rl_clear_history();
	gc_free_all();
	free_env(&get_shell()->env);
	if (do_exit == 1)
		exit(code);
}
