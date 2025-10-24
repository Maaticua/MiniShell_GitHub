/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:12:38 by macaruan          #+#    #+#             */
/*   Updated: 2025/09/27 13:13:49 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_unset(char **args, t_env **env)
{
	int	i;
	int	err;

	i = 1;
	err = 0;
	if (!args[1])
		return (EXIT_SUCCESS);
	while (args[i])
	{
		if (is_valid_identifier(args[i]) == EXIT_FAILURE)
		{
			ft_printf("unset: `%s`: not a valid identifier\n", args[i++]);
			err = 1;
		}
		else
			unset_env_var(env, args[i++]);
	}
	if (err)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}
