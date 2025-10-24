/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 13:59:24 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 17:04:37 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

t_env	**env_to_sorted_array(t_env *env)
{
	t_env	**arr;
	int		size;
	int		i;

	size = env_list_size(env);
	arr = gc_malloc(sizeof(t_env *) * (size + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	sort_env_array(t_env **arr, int size)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(arr[i]->key, arr[j]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_export(t_env *env)
{
	t_env	**sorted;
	int		size;
	int		i;

	sorted = env_to_sorted_array(env);
	if (!sorted)
		return ;
	size = env_list_size(env);
	sort_env_array(sorted, size);
	i = 0;
	while (i < size)
	{
		if (sorted[i]->value != NULL)
			ft_printf("declare -x %s=\"%s\"\n", sorted[i]->key,
				sorted[i]->value);
		else
			ft_printf("declare -x %s\n", sorted[i]->key);
		i++;
	}
}
