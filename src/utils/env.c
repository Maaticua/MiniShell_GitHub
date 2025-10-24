/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:36:43 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/09 17:20:01 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_env(t_env **env)
{
	t_env	*current;
	t_env	*next;

	current = *env;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*env = NULL;
}

static t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

static t_env	*process_env_entry(char *envp_line, t_env **env, t_env **last)
{
	char	*sep;
	char	*key;
	char	*value;
	t_env	*new;

	sep = ft_strchr(envp_line, '=');
	if (!sep)
		return (*last);
	key = ft_substr(envp_line, 0, sep - envp_line);
	value = ft_strdup(sep + 1);
	new = new_env_node(key, value);
	free(key);
	free(value);
	if (!*env)
		*env = new;
	else
		(*last)->next = new;
	return (new);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*last;

	env = NULL;
	last = NULL;
	while (*envp)
	{
		last = process_env_entry(*envp, &env, &last);
		envp++;
	}
	return (env);
}

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key) + 1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
// t_env	*init_env(char **envp)
// {
// 	t_env	*env;
// 	t_env	*last;
// 	char	*sep;
// 	char	*key;
// 	char	*value;
// 	t_env	*new;

// 	env = NULL;
// 	last = NULL;
// 	while (*envp)
// 	{
// 		sep = ft_strchr(*envp, '=');
// 		if (sep)
// 		{
// 			key = ft_substr(*envp, 0, sep - *envp);
// 			value = ft_strdup(sep + 1);
// 			new = new_env_node(key, value);
// 			free(key);
// 			free(value);
// 			if (!env)
// 				env = new;
// 			else
// 				last->next = new;
// 			last = new;
// 		}
// 		envp++;
// 	}
// 	return (env);
// }
