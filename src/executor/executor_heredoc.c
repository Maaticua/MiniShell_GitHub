/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:32:18 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 15:51:03 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*hd_read_line(void)
{
	char	*line;
	size_t	len;

	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			len = ft_strlen(line);
			if (len > 0 && line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
	}
	return (line);
}

t_tokenlist	*hd_creat_token(char *line)
{
	t_tokenlist	*line_token;
	int			i;

	i = 0;
	line_token = gc_malloc(sizeof(t_tokenlist));
	if (!line_token)
		return (NULL);
	line_token->is_delim = 0;
	line_token->is_empty_quoted = 0;
	line_token->next = NULL;
	line_token->token = gc_strdup(line);
	line_token->qmask = gc_strdup("");
	while (line_token->token[i])
	{
		line_token->qmask = gc_strjoin_char(line_token->qmask, '2');
		i++;
	}
	line_token->token_heredoc_quoted = 0;
	line_token->token_type = WORD;
	return (line_token);
}

char	*hd_process_line(char *line, int is_delim_quoted)
{
	t_tokenlist	*line_token;
	char		*processed;

	if (is_delim_quoted)
		return (line);
	line_token = hd_creat_token(line);
	free(line);
	expand_one(line_token);
	processed = gc_strdup(line_token->token);
	return (processed);
}

void	hd_child_process(int fdw, const char *delim, int is_delim_quoted)
{
	char	*line;
	char	*processed_line;
	int		ret;

	if (dup2(fdw, HD_FD) == -1)
		return (close(fdw), cleanup_and_exit(0, 1));
	close(fdw);
	init_sig_heredoc();
	while (1)
	{
		line = hd_read_line();
		if (line == NULL)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		processed_line = hd_process_line(line, is_delim_quoted);
		ret = hd_write_line(HD_FD, processed_line);
		if (ret == -1)
			cleanup_and_exit(1, 1);
	}
	close(HD_FD);
	cleanup_and_exit(0, 1);
}

int	hd_fill_tmp(int fdw, const char *delim, int is_delim_quoted)
{
	const char	*safe_delim;

	safe_delim = delim;
	if (!safe_delim)
		safe_delim = "";
	return (hd_process_input(fdw, safe_delim, is_delim_quoted));
}
