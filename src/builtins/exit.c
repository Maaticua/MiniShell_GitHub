/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:12:29 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/06 18:45:54 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_number(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (EXIT_FAILURE);
	i = 0;
	while (str[i])
		if (!ft_isdigit(str[i++]))
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	builtin_exit(char **args)
{
	int	code;

	code = 0;
	if (!args || !args[0])
		return (EXIT_FAILURE);
	if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	if (args[1])
	{
		if (ft_strlen(args[1]) == 0 || is_number(args[1]) == EXIT_FAILURE)
		{
			ft_printf("exit: numeric argument required\n");
			code = 2;
		}
		else
			code = ft_atoi(args[1]) % 256;
		if (args[2])
		{
			ft_printf("exit: too many arguments\n");
			return (EXIT_FAILURE);
		}
	}
	cleanup_and_exit(code, 1);
	return (EXIT_SUCCESS);
}
