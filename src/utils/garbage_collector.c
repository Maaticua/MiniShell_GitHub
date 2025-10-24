/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:36:59 by macaruan          #+#    #+#             */
/*   Updated: 2025/09/29 18:43:15 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_gc	*get_gc(void)
{
	static t_gc	gc = {NULL};

	return (&gc);
}

void	*gc_malloc(size_t size)
{
	t_gc_node	*node;
	void		*mem;

	mem = malloc(size);
	if (!mem)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(mem);
		return (NULL);
	}
	node->ptr = mem;
	node->next = get_gc()->head;
	get_gc()->head = node;
	return (mem);
}

char	*gc_strdup(const char *s)
{
	size_t	len;
	char	*copy;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	copy = gc_malloc(len + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (s[i])
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	gc_free_all(void)
{
	t_gc_node	*tmp;
	t_gc_node	*node;

	node = get_gc()->head;
	while (node)
	{
		tmp = node->next;
		free(node->ptr);
		free(node);
		node = tmp;
	}
	get_gc()->head = NULL;
}

char	*gc_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (gc_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	sub = (char *)gc_malloc(len + 1);
	if (!sub)
		return (NULL);
	ft_strlcpy(sub, s + start, len + 1);
	return (sub);
}
