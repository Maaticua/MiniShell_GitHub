/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:33:13 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 14:10:55 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pipe_error(const char *msg, int code)
{
	ft_putstr_fd((char *)msg, 2);
	return (code);
}

static void	setup_child_fds(int in_fd, int out_fd)
{
	if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
		cleanup_and_exit(1, 1);
	if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
		cleanup_and_exit(1, 1);
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
}

static void	exec_command(t_cmd *cmd, char **envp)
{
	char	*path;

	if (has_cmd(cmd) && is_builtin(cmd->args[0]) == EXIT_SUCCESS)
		cleanup_and_exit(exec_builtin(cmd->args), 1);
	path = get_cmd_path(cmd->args[0], envp);
	if (!path)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		cleanup_and_exit(127, 1);
	}
	execve(path, cmd->args, envp);
	perror("execve");
	cleanup_and_exit(1, 1);
}

static void	do_child_exec(t_cmd *cmd, int in_fd, int out_fd, char **envp)
{
	int	st;

	st = 0;
	setup_child_fds(in_fd, out_fd);
	if (apply_redirection(cmd, 1) == EXIT_FAILURE)
		cleanup_and_exit(1, 1);
	if (has_cmd(cmd) && is_builtin(cmd->args[0]) == EXIT_SUCCESS)
	{
		signal(SIGPIPE, SIG_IGN);
		st = exec_builtin(cmd->args);
		cleanup_and_exit(st, 1);
	}
	else
	{
		signal(SIGPIPE, SIG_DFL);
		exec_command(cmd, envp);
	}
}

int	spawn_child_and_close(t_cmd *cmd, int in_fd, int out_fd,
		t_pipe_ctx *ctx)
{
	pid_t	child;

	child = fork();
	if (child == -1)
	{
		perror("fork");
		if (out_fd != -1)
			close(out_fd);
		if (*(ctx->next_in) != -1)
			close(*(ctx->next_in));
		return (1);
	}
	if (child == 0)
	{
		if (*(ctx->next_in) != -1 && *(ctx->next_in) != in_fd)
			close(*(ctx->next_in));
		do_child_exec(cmd, in_fd, out_fd, ctx->envp);
	}
	if (out_fd != -1)
		close(out_fd);
	if (in_fd != -1)
		close(in_fd);
	*(ctx->pid) = child;
	return (0);
}
