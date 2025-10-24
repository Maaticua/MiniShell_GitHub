/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:12:33 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 17:08:39 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// void	print_export(t_env *env)
// {
// 	while (env != NULL)
// 	{
// 		if (env->value != NULL)
// 			ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
// 		else
// 			ft_printf("declare -x %s\n", env->key);
// 		env = env->next;
// 	}
// }

int	is_valid_identifier(char *str)
{
	char	*key;
	char	*sep;
	int		i;

	i = 1;
	sep = ft_strchr(str, '=');
	if (sep)
		key = gc_substr(str, 0, sep - str);
	else
		key = str;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (EXIT_FAILURE);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	do_export(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	char	*sep;

	value = NULL;
	sep = ft_strchr(arg, '=');
	if (!sep)
		set_env_value(env, arg, NULL);
	else
	{
		key = gc_substr(arg, 0, sep - arg);
		value = gc_strdup(sep + 1);
		set_env_value(env, key, value);
	}
}

int	builtin_export(char **args, t_env **env)
{
	int	i;

	i = 1;
	if (!args[1])
	{
		print_export(*env);
		return (EXIT_SUCCESS);
	}
	while (args[i])
	{
		if (is_valid_identifier(args[i]) == EXIT_FAILURE)
		{
			ft_printf("export: `%s`: not a valid identifier\n", args[i]);
			return (EXIT_FAILURE);
		}
		else
			do_export(args[i++], env);
	}
	return (EXIT_SUCCESS);
}
