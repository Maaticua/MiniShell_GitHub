/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:49:44 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/10 11:20:45 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_cmd	*new_cmd(int pipe_before)
{
	t_cmd	*cmdlist;

	cmdlist = gc_malloc(sizeof(t_cmd));
	if (!cmdlist)
		return (NULL);
	ft_bzero(cmdlist, sizeof(*cmdlist));
	cmdlist->pid = -1;
	cmdlist->pipe_in = (pipe_before == 1);
	return (cmdlist);
}

void	link_cmd_nodes(t_cmd **firstcmd, t_cmd **lastcmd, t_cmd *cmdlist)
{
	if (!*firstcmd)
		*firstcmd = cmdlist;
	else
		(*lastcmd)->next = cmdlist;
	*lastcmd = cmdlist;
}

int	init_redir_arrays(t_cmd *cmdlist, t_tokenlist *tk)
{
	int			outfile_counter;
	int			infile_counter;

	outfile_counter = 0;
	infile_counter = 0;
	while (tk && tk->token_type != PIPE && tk->token[0] != '\0')
	{
		if (tk->token_type == REDIR_OUT || tk->token_type == APPEND)
			outfile_counter++;
		if (tk->token_type == REDIR_IN || tk->token_type == HEREDOC)
			infile_counter++;
		tk = tk->next;
	}
	cmdlist->outfile = gc_malloc(sizeof(char *) * (outfile_counter + 1));
	if (!cmdlist->outfile)
		return (EXIT_FAILURE);
	cmdlist->infile = gc_malloc(sizeof(char *) * (infile_counter + 1));
	if (!cmdlist->infile)
		return (EXIT_FAILURE);
	cmdlist->in_is_tmp = gc_malloc(sizeof(int) * (infile_counter));
	if (!cmdlist->in_is_tmp)
		return (EXIT_FAILURE);
	cmdlist->outfile[0] = NULL;
	cmdlist->infile[0] = NULL;
	return (EXIT_SUCCESS);
}

int	fill_args_and_redirs(t_cmd *cmdlist, t_tokenlist **ptk)
{
	t_tokenlist		*tk;
	int				i;
	t_in_out_ctrs	in_out_ctrs;

	in_out_ctrs.out_ctr = 0;
	in_out_ctrs.in_ctr = 0;
	tk = *ptk;
	if (init_args_array(cmdlist, tk) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_redir_arrays(cmdlist, tk) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = 0;
	while (tk && tk->token_type != PIPE && tk->token[0] != '\0')
	{
		if (handle_token_step(cmdlist, &tk, &i,
				&in_out_ctrs) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	*ptk = tk;
	return (EXIT_SUCCESS);
}

t_cmd	*parse_tokenlist(t_tokenlist *tokenlist)
{
	t_cmd		*cmdlist;
	t_cmd		*firstcmd;
	t_cmd		*lastcmd;
	int			pipe_before;

	firstcmd = NULL;
	lastcmd = NULL;
	pipe_before = 0;
	while (tokenlist && tokenlist->token[0] != '\0')
	{
		cmdlist = new_cmd(pipe_before);
		if (!cmdlist)
			return (NULL);
		link_cmd_nodes(&firstcmd, &lastcmd, cmdlist);
		if (fill_args_and_redirs(cmdlist, &tokenlist) == EXIT_FAILURE)
			return (NULL);
		if (validate_segment_headless(cmdlist, pipe_before) == EXIT_FAILURE)
			return (NULL);
		if (handle_pipe_token(cmdlist, &tokenlist
				, &pipe_before) == EXIT_FAILURE)
			return (NULL);
	}
	if (pipe_at_end(pipe_before) == EXIT_FAILURE)
		return (NULL);
	return (firstcmd);
}
