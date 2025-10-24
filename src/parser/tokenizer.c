/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:32:34 by awaegaer          #+#    #+#             */
/*   Updated: 2025/09/30 17:55:37 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_tokenlist	*init_tokenlist(void)
{
	t_tokenlist	*tokenlist;

	tokenlist = gc_malloc(sizeof(t_tokenlist));
	if (!tokenlist)
		return (NULL);
	tokenlist->next = NULL;
	tokenlist->token = gc_strdup("");
	if (!tokenlist->token)
		return (NULL);
	tokenlist->qmask = gc_strdup("");
	if (!tokenlist->qmask)
		return (NULL);
	tokenlist->is_empty_quoted = 0;
	tokenlist->token_heredoc_quoted = 0;
	tokenlist->is_delim = 0;
	tokenlist->token_type = WORD;
	return (tokenlist);
}

int	new_token(t_tokenlist **tokenlist, int *tkl_size)
{
	(*tokenlist)->next = gc_malloc(sizeof(t_tokenlist));
	if (!(*tokenlist)->next)
		return (EXIT_FAILURE);
	*tokenlist = (*tokenlist)->next;
	(*tokenlist)->next = NULL;
	(*tokenlist)->token = gc_strdup("");
	if (!(*tokenlist)->token)
		return (EXIT_FAILURE);
	(*tokenlist)->qmask = gc_strdup("");
	if (!(*tokenlist)->qmask)
		return (EXIT_FAILURE);
	(*tokenlist)->is_empty_quoted = 0;
	(*tokenlist)->token_heredoc_quoted = 0;
	(*tokenlist)->is_delim = 0;
	(*tokenlist)->token_type = WORD;
	(*tkl_size)++;
	return (EXIT_SUCCESS);
}

int	tokenizer_loop(char *line, t_tokenlist *tokenlist, int *tkl_size)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			i = handle_space(&tokenlist, tkl_size, i);
		else if (line[i] == '"' || line[i] == '\'')
		{
			if (handle_quotes(line, tokenlist, &i) == -1)
				return (EXIT_FAILURE);
		}
		else if (line[i] == '>' || line[i] == '<' || line[i] == '|')
		{
			i = handle_pipe_n_redir(line, &tokenlist, tkl_size, i);
			handle_heredoc_delim(&tokenlist, tkl_size, line, i);
		}
		else
		{
			tokenlist->qmask = gc_strjoin_char(tokenlist->qmask, '0');
			tokenlist->token = gc_strjoin_char(tokenlist->token, line[i++]);
		}
	}
	return (EXIT_SUCCESS);
}

t_tokenlist	*tokenize_line(char *line, int *tkl_size)
{
	int			i;
	t_tokenlist	*tokenlist;
	t_tokenlist	*first_token;

	i = 0;
	tokenlist = init_tokenlist();
	first_token = tokenlist;
	(*tkl_size) = 1;
	if (tokenizer_loop(line, tokenlist, tkl_size) == EXIT_FAILURE)
		return (NULL);
	(*tkl_size)--;
	return (first_token);
}
