/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_run.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:23:48 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 13:28:10 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_builtin_or_fork(t_cmd *cmd, char **envp, int has_redir)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!has_redir && has_cmd(cmd) && is_builtin(cmd->args[0]) == EXIT_SUCCESS)
		return (exec_builtin(cmd->args));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		child_exec_builtin_or_execve(cmd, envp);
	init_sig_exec();
	status = wait_status(pid);
	init_sig();
	return (status);
}

void	child_exec_builtin_or_execve(t_cmd *cmd, char **envp)
{
	int	code;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	code = 0;
	if (apply_redirection(cmd, 1) == EXIT_FAILURE)
		cleanup_and_exit(1, 1);
	if (!has_cmd(cmd))
		cleanup_and_exit(1, 1);
	if (is_builtin(cmd->args[0]) == EXIT_SUCCESS)
	{
		code = exec_builtin(cmd->args);
		cleanup_and_exit(code, 1);
	}
	handle_cmd_path(cmd, envp);
}

static void	handle_cmd_path_error(t_cmd *cmd, char *path)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": Permission denied\n", 2);
	free(path);
	cleanup_and_exit(126, 1);
}

static void	check_path_exists(t_cmd *cmd, char **path)
{
	if (!*path && ft_strchr(cmd->args[0], '/'))
	{
		if (access(cmd->args[0], F_OK) == 0)
			*path = ft_strdup(cmd->args[0]);
	}
	if (!*path)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cleanup_and_exit(127, 1);
	}
}

void	handle_cmd_path(t_cmd *cmd, char **envp)
{
	char		*path;
	struct stat	path_stat;

	if (!cmd || !has_cmd(cmd))
		cleanup_and_exit(0, 1);
	path = get_cmd_path(cmd->args[0], envp);
	check_path_exists(cmd, &path);
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		cleanup_and_exit(126, 1);
	}
	execve(path, cmd->args, envp);
	if (errno == EACCES)
		handle_cmd_path_error(cmd, path);
	free(path);
	perror("execve");
	cleanup_and_exit(126, 1);
}
