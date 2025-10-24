/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:33:04 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 15:48:21 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	apply_infile(t_cmd *cmd)
{
	int	fd;
	int	infile_counter;

	fd = -1;
	infile_counter = 0;
	while (cmd->infile[infile_counter])
	{
		fd = open(cmd->infile[infile_counter], O_RDONLY);
		if (fd == -1)
			return (perror(cmd->infile[infile_counter]), EXIT_FAILURE);
		if (cmd->in_is_tmp[infile_counter])
			unlink(cmd->infile[infile_counter]);
		if (cmd->infile[++infile_counter])
			close(fd);
	}
	if (fd == -1)
		return (perror(cmd->infile[0]), EXIT_FAILURE);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2 infile"), close(fd), EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}

int	apply_outfile(t_cmd *cmd, int with_pipe)
{
	int	fd;
	int	outfile_counter;

	fd = -1;
	outfile_counter = 0;
	while (cmd->outfile[outfile_counter])
	{
		if (cmd->append)
			fd = open(cmd->outfile[outfile_counter], O_WRONLY
					| O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile[outfile_counter], O_WRONLY
					| O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (perror(cmd->outfile[outfile_counter]), EXIT_FAILURE);
		if (cmd->outfile[++outfile_counter])
			close(fd);
	}
	if (fd == -1)
		return (perror(cmd->outfile[0]), EXIT_FAILURE);
	if (with_pipe && dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2 outfile"), close(fd), EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}

int	apply_redirection(t_cmd *cmd, int with_pipe)
{
	if (cmd->infile[0] && apply_infile(cmd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (cmd->outfile[0] && apply_outfile(cmd, with_pipe) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	hd_write_line(int fd, const char *s)
{
	size_t	len;
	ssize_t	wr;

	if (!s)
		return (0);
	len = ft_strlen(s);
	wr = write(fd, s, len);
	if (wr != (ssize_t)len)
	{
		close(fd);
		return (-1);
	}
	if (len == 0 || s[len - 1] != '\n')
	{
		wr = write(fd, "\n", 1);
		if (wr != 1)
		{
			close(fd);
			return (-1);
		}
	}
	return (0);
}

int	prepare_hd_tmp(t_tokenlist *tk)
{
	char	*tmp_tpl;

	tmp_tpl = NULL;
	tmp_tpl = generate_tmp_name();
	if (!tmp_tpl)
	{
		perror("generate_tmp_name");
		return (EXIT_FAILURE);
	}
	if (open_and_fill_tmp(tmp_tpl, tk) == -1)
	{
		unlink(tmp_tpl);
		return (EXIT_FAILURE);
	}
	tk->token = gc_strdup(tmp_tpl);
	return (EXIT_SUCCESS);
}

// int	prepare_hd_tmp(t_cmd *cmd)
// {
// 	char	tmp_tpl[32];
// 	int		fdtmp;
// 	int		ret;

// 	if (!cmd->heredoc)
// 		return (EXIT_SUCCESS);
// 	ft_strlcpy(tmp_tpl, "/tmp/minishell_hdXXXXXX", 32);
// 	fdtmp = mkstemp(tmp_tpl);
// 	if (fdtmp == -1)
// 		return (perror("mkstemp"), EXIT_FAILURE);
// 	ret = hd_fill_tmp(fdtmp, cmd->heredoc_delim, cmd->is_heredoc_delim_quoted);
// 	if (ret == -1)
// 	{
// 		perror("heredoc write");
// 		close(fdtmp);
// 		unlink(tmp_tpl);
// 		return (EXIT_FAILURE);
// 	}
// 	cmd->infile = gc_strdup(tmp_tpl);
// 	cmd->in_is_file = 1;
// 	cmd->in_is_tmp = 1;
// 	close(fdtmp);
// 	return (EXIT_SUCCESS);
// }
