/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:33:01 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/09 17:10:26 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	has_cmd(const t_cmd *cmd)
{
	return (cmd && cmd->args && cmd->args[0] && cmd->args[0][0]);
}

int	is_builtin(char *cmd)
{
	if (!cmd || !*cmd)
		return (EXIT_FAILURE);
	if ((!ft_strncmp(cmd, "cd", 3) && ft_strlen(cmd) == 2) || (!ft_strncmp(cmd,
				"pwd", 4) && ft_strlen(cmd) == 3) || (!ft_strncmp(cmd, "echo",
				5) && ft_strlen(cmd) == 4) || (!ft_strncmp(cmd, "exit", 5)
			&& ft_strlen(cmd) == 4) || (!ft_strncmp(cmd, "env", 4)
			&& ft_strlen(cmd) == 3) || (!ft_strncmp(cmd, "export", 7)
			&& ft_strlen(cmd) == 6) || (!ft_strncmp(cmd, "unset", 6)
			&& ft_strlen(cmd) == 5))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	exec_builtin(char **args)
{
	if (!ft_strncmp(args[0], "cd", 3) && ft_strlen(args[0]) == 2)
		return (builtin_cd(args));
	if (!ft_strncmp(args[0], "pwd", 4) && ft_strlen(args[0]) == 3)
		return (builtin_pwd(args));
	if (!ft_strncmp(args[0], "echo", 5) && ft_strlen(args[0]) == 4)
		return (builtin_echo(args));
	if (!ft_strncmp(args[0], "exit", 5) && ft_strlen(args[0]) == 4)
		return (builtin_exit(args));
	if (!ft_strncmp(args[0], "env", 4) && ft_strlen(args[0]) == 3)
		return (builtin_env(args));
	if (!ft_strncmp(args[0], "export", 7) && ft_strlen(args[0]) == 6)
		return (builtin_export(args, &get_shell()->env));
	if (!ft_strncmp(args[0], "unset", 6) && ft_strlen(args[0]) == 5)
		return (builtin_unset(args, &get_shell()->env));
	return (EXIT_FAILURE);
}
