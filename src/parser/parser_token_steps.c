/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_steps.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 18:52:48 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/10 11:26:14 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_words_segment(t_tokenlist *tmp)
{
	int	i;

	i = 0;
	while (tmp && (tmp->token[0] != '\0' || tmp->is_empty_quoted)
		&& tmp->token_type != PIPE)
	{
		if (tmp->token_type == WORD)
		{
			i++;
			tmp = tmp->next;
		}
		else if (tmp->token_type == REDIR_IN || tmp->token_type == REDIR_OUT
			|| tmp->token_type == APPEND || tmp->token_type == HEREDOC)
		{
			if (tmp->next && tmp->next->token_type == WORD
				&& (tmp->next->token[0] != '\0' || tmp->next->is_empty_quoted))
				tmp = tmp->next->next;
			else
			{
				return (syntax_error_redir());
			}
		}
	}
	return (i);
}

int	validate_token_to_comsume(t_tokenlist *tk)
{
	if (!tk->next || tk->next->token_type != WORD || tk->next->token[0] == '\0')
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		get_shell()->last_status = 2;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	consume_redir_in_out(t_cmd *cmdlist, t_tokenlist **ptk,
	int *outfile_counter, int *infile_counter)
{
	t_tokenlist	*tk;

	tk = *ptk;
	if (validate_token_to_comsume(tk) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (tk->token_type == REDIR_IN)
	{
		cmdlist->infile[(*infile_counter)] = gc_strdup(tk->next->token);
		cmdlist->infile[(*infile_counter) + 1] = NULL;
		cmdlist->in_is_tmp[*(infile_counter)] = 0;
		(*infile_counter)++;
		cmdlist->in_is_file = 1;
		cmdlist->heredoc = 0;
	}
	else
	{
		cmdlist->outfile[(*outfile_counter)] = gc_strdup(tk->next->token);
		cmdlist->outfile[(*outfile_counter) + 1] = NULL;
		(*outfile_counter)++;
		cmdlist->append = 0;
	}
	*ptk = tk->next->next;
	return (EXIT_SUCCESS);
}

int	consume_append_or_heredoc(t_cmd	*cmdlist, t_tokenlist **ptk,
	int *outfile_counter, int *infile_counter)
{
	t_tokenlist	*tk;

	tk = *ptk;
	if (validate_token_to_comsume(tk) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (tk->token_type == APPEND)
	{
		cmdlist->outfile[(*outfile_counter)] = gc_strdup(tk->next->token);
		cmdlist->outfile[(*outfile_counter) + 1] = NULL;
		(*outfile_counter)++;
		cmdlist->append = 1;
	}
	else
	{
		if (handle_heredocs(&tk->next) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		cmdlist->infile[(*infile_counter)] = gc_strdup(tk->next->token);
		cmdlist->infile[(*infile_counter) + 1] = NULL;
		cmdlist->in_is_tmp[*(infile_counter)] = 1;
		(*infile_counter)++;
		cmdlist->heredoc = 1;
		cmdlist->in_is_file = 0;
		cmdlist->is_heredoc_delim_quoted = tk->next->token_heredoc_quoted;
	}
	return (*ptk = tk->next->next, EXIT_SUCCESS);
}

int	handle_token_step(t_cmd *cmdlist, t_tokenlist **ptk, int *i,
	t_in_out_ctrs *in_out_ctrs)
{
	if ((*ptk)->token_type == WORD)
	{
		cmdlist->args[(*i)++] = gc_strdup((*ptk)->token);
		*ptk = (*ptk)->next;
	}
	else if ((*ptk)->token_type == REDIR_IN || (*ptk)->token_type == REDIR_OUT)
	{
		if (consume_redir_in_out(cmdlist, ptk,
				&in_out_ctrs->out_ctr, &in_out_ctrs->in_ctr) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if ((*ptk)->token_type == APPEND || (*ptk)->token_type == HEREDOC)
	{
		if (consume_append_or_heredoc(cmdlist, ptk,
				&in_out_ctrs->out_ctr, &in_out_ctrs->in_ctr) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
		*ptk = (*ptk)->next;
	return (EXIT_SUCCESS);
}
