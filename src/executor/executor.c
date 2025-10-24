/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:33:09 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/08 18:12:07 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	wait_status(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	run_cmdlist(t_cmd *cmds, char **envp)
{
	if (!cmds)
		return (0);
	if (cmds->next == NULL)
		return (exec_no_pipeline(cmds, envp));
	return (run_pipeline(cmds, envp));
}

int	exec_no_pipeline(t_cmd *cmd, char **envp)
{
	int	has_redir;

	has_redir = cmd_has_redir(cmd);
	return (exec_builtin_or_fork(cmd, envp, has_redir));
}
