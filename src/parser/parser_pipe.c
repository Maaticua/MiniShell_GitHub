/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:56:45 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/10 11:26:21 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	init_args_array(t_cmd *cmdlist, t_tokenlist *tk)
{
	int	nb;

	nb = count_words_segment(tk);
	if (nb < 0)
		return (EXIT_FAILURE);
	cmdlist->args = gc_malloc(sizeof(char *) * (nb + 1));
	if (!cmdlist->args)
		return (EXIT_FAILURE);
	cmdlist->args[nb] = NULL;
	return (EXIT_SUCCESS);
}

int	handle_pipe_token(t_cmd *cmdlist, t_tokenlist **ptk, int *pipe_before)
{
	if (*ptk && (*ptk)->token_type == PIPE)
	{
		if (!cmdlist->args[0] && !cmdlist->in_is_file && !cmdlist->heredoc)
		{
			ft_putstr_fd("bash: syntax error: missing args before `|'\n", 2);
			get_shell()->last_status = 2;
			return (EXIT_FAILURE);
		}
		if (cmdlist->outfile[0] && !cmdlist->append)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
			get_shell()->last_status = 2;
			return (EXIT_FAILURE);
		}
		cmdlist->pipe_out = 1;
		*ptk = (*ptk)->next;
		*pipe_before = 1;
	}
	else
		*pipe_before = 0;
	return (EXIT_SUCCESS);
}

int	pipe_at_end(int pipe_before)
{
	if (pipe_before == 1)
	{
		ft_putstr_fd("bash: syntax error: unexpected end of file\n", 2);
		get_shell()->last_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	syntax_error_redir(void)
{
	ft_putstr_fd("syntax error near unexpected token `newline\n", 2);
	get_shell()->last_status = 2;
	return (-1);
}

int	validate_segment_headless(t_cmd *cmdlist, int pipe_before)
{
	if (cmdlist->args[0] == NULL && !cmdlist->in_is_file && !cmdlist->heredoc
		&& !cmdlist->outfile[0] && pipe_before == 0)
	{
		ft_putstr_fd("bash: syntax error: missing args after `|'\n", 2);
		get_shell()->last_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
