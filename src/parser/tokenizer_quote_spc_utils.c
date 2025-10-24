/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quote_spc_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:50:35 by awaegaer          #+#    #+#             */
/*   Updated: 2025/09/30 17:51:40 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_space(t_tokenlist **tokenlist, int *tkl_size, int i)
{
	if (ft_strlen((*tokenlist)->token) > 0
		|| (*tokenlist)->is_empty_quoted == 1)
		new_token(tokenlist, tkl_size);
	i++;
	return (i);
}

void	handle_single_quote(t_tokenlist *tkl, int *state, int *i, char *line)
{
	if (*state == NONE)
	{
		*state = SQ;
		if (tkl->is_delim)
			tkl->token_heredoc_quoted = 1;
	}
	else if (*state == SQ)
		*state = NONE;
	else if (*state == DQ)
	{
		tkl->token = gc_strjoin_char(tkl->token, line[*i]);
		tkl->qmask = gc_strjoin_char(tkl->qmask, '1');
	}
}

void	handle_double_quote(t_tokenlist *tkl, int *state, int *i, char *line)
{
	if (*state == NONE)
	{
		*state = DQ;
		if (tkl->is_delim)
			tkl->token_heredoc_quoted = 1;
	}
	else if (*state == DQ)
		*state = NONE;
	else if (*state == SQ)
	{
		tkl->token = gc_strjoin_char(tkl->token, line[*i]);
		tkl->qmask = gc_strjoin_char(tkl->qmask, '2');
	}
}

void	append_char_with_mask(t_tokenlist *tokenlist, char c, int state)
{
	tokenlist->token = gc_strjoin_char(tokenlist->token, c);
	if (state == NONE)
		tokenlist->qmask = gc_strjoin_char(tokenlist->qmask, '0');
	else if (state == SQ)
		tokenlist->qmask = gc_strjoin_char(tokenlist->qmask, '1');
	else if (state == DQ)
		tokenlist->qmask = gc_strjoin_char(tokenlist->qmask, '2');
}

int	process_quote_char(char *line, t_tokenlist *tokenlist, int *i, int *state)
{
	if (line[*i] == '\'')
		handle_single_quote(tokenlist, state, i, line);
	else if (line[*i] == '"')
		handle_double_quote(tokenlist, state, i, line);
	return (EXIT_SUCCESS);
}
