/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:36:50 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 17:13:06 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	desole_alex(t_env *tmp)
{
	int	count;

	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**arr;
	char	*line;

	count = 0;
	tmp = env;
	count = desole_alex(tmp);
	arr = gc_malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	tmp = env;
	count = 0;
	while (tmp)
	{
		if (tmp->value)
			line = ft_strjoin_three(tmp->key, "=", tmp->value);
		else
			line = gc_strdup (tmp->key);
		arr[count++] = line;
		tmp = tmp->next;
	}
	arr[count] = NULL;
	return (arr);
}
