/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 15:07:31 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/09 15:06:21 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	spawn_one(t_cmd *cmd, int in_fd, t_pipe_ctx *ctx)
{
	int	pipefd[2];
	int	out_fd;

	out_fd = -1;
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		out_fd = pipefd[1];
		*(ctx->next_in) = pipefd[0];
	}
	else
		*(ctx->next_in) = -1;
	return (spawn_child_and_close(cmd, in_fd, out_fd, ctx));
}

static int	handle_pipeline_error(int in_fd)
{
	if (in_fd != -1)
		close(in_fd);
	return (1);
}

static void	cleanup_final_fd(int in_fd)
{
	if (in_fd != -1)
		close(in_fd);
}

int	run_pipeline(t_cmd *cmds, char **envp)
{
	pid_t		pids[1024];
	int			i;
	int			in_fd;
	t_pipe_ctx	ctx;

	i = 0;
	in_fd = -1;
	while (cmds)
	{
		if (!cmds->args || !cmds->args[0])
			return (pipe_error("minishell: empty command in pipe\n", 2));
		ctx.envp = envp;
		ctx.pid = &pids[i];
		ctx.next_in = &in_fd;
		if (spawn_one(cmds, in_fd, &ctx) != 0)
			return (handle_pipeline_error(in_fd));
		cmds = cmds->next;
		if (++i >= 1024)
			return (pipe_error("minishell: too many pipe cmds\n", 1));
	}
	cleanup_final_fd(in_fd);
	return (wait_all(pids, i));
}
