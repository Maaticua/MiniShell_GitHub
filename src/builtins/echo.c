/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:12:24 by macaruan          #+#    #+#             */
/*   Updated: 2025/09/27 13:13:03 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	only_ns_option(char *flag)
{
	int	i;

	i = 1;
	if (flag[0] != '-')
		return (EXIT_FAILURE);
	if (flag[i] == '\0')
		return (EXIT_FAILURE);
	while (flag[i])
		if (flag[i++] != 'n')
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	builtin_echo(char **args)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (args[i] && (only_ns_option(args[i]) == EXIT_SUCCESS))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!n_flag)
		ft_printf("\n");
	return (0);
}
