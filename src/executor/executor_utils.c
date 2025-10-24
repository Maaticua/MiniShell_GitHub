/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:33:07 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 15:53:04 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_has_redir(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->in_is_file || cmd->outfile[0] || cmd->append
		|| cmd->heredoc || cmd->infile[0])
		return (1);
	return (0);
}

int	wait_all(pid_t *pids, int count)
{
	int	i;
	int	status;
	int	code;

	i = 0;
	code = 0;
	while (i < count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			code = 1;
		else if (WIFEXITED(status))
			code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			code = 128 + WTERMSIG(status);
		i++;
	}
	return (code);
}

int	hd_process_input(int fdw, const char *delim, int is_delim_quoted)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		(void)close(fdw);
		return (-1);
	}
	if (pid == 0)
		hd_child_process(fdw, delim, is_delim_quoted);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	close(fdw);
	if (WEXITSTATUS(status) == 130)
	{
		get_shell()->last_status = 130;
		return (-1);
	}
	if (WEXITSTATUS(status) != 0)
		return (-1);
	return (0);
}
