/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_loop_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:55:16 by awaegaer          #+#    #+#             */
/*   Updated: 2025/09/30 17:55:29 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_error_no_mquote(int state)
{
	if (state == SQ || state == DQ)
	{
		ft_putstr_fd("Syntax error: unexpected end of file ", 2);
		ft_putstr_fd("while looking for matching quote\n", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_quotes(char *line, t_tokenlist *tokenlist, int *i)
{
	int	state;
	int	size_before;

	state = NONE;
	size_before = strlen(tokenlist->token);
	while (line[*i])
	{
		if (line[*i] == '\'' || line[*i] == '"')
			process_quote_char(line, tokenlist, i, &state);
		else
		{
			if ((line[*i] == ' ' || line[*i] == '|' || line[*i] == '>'
					|| line[*i] == '<') && state == 0)
				return (EXIT_SUCCESS);
			append_char_with_mask(tokenlist, line[*i], state);
		}
		(*i)++;
	}
	if (handle_error_no_mquote(state) == EXIT_FAILURE)
		return (-1);
	if ((int)ft_strlen(tokenlist->token) == size_before && size_before == 0)
		tokenlist->is_empty_quoted = 1;
	return (EXIT_SUCCESS);
}

void	consume_heredoc_append(t_tokenlist *tkl, char *line, int *i, char sep)
{
	(tkl)->token = gc_strjoin_char((tkl)->token, line[(*i)++]);
	(tkl)->qmask = gc_strjoin_char((tkl)->qmask, '0');
	(tkl)->token = gc_strjoin_char((tkl)->token, line[(*i)++]);
	(tkl)->qmask = gc_strjoin_char((tkl)->qmask, '0');
	if (sep == '<')
		(tkl)->token_type = HEREDOC;
	else
		(tkl)->token_type = APPEND;
}

int	handle_pipe_n_redir(char *line, t_tokenlist **tokenlist, int *tkl_size,
		int i)
{
	char	sep_type;

	if (ft_strlen((*tokenlist)->token) > 0
		|| (*tokenlist)->is_empty_quoted == 1)
		new_token(tokenlist, tkl_size);
	sep_type = line[i];
	if (sep_type == '|')
		(*tokenlist)->token_type = PIPE;
	else if (sep_type == '>')
		(*tokenlist)->token_type = REDIR_OUT;
	else if (sep_type == '<')
		(*tokenlist)->token_type = REDIR_IN;
	if (line[i] != '|' && line[i + 1] != '\0' && line[i + 1] == sep_type)
		consume_heredoc_append((*tokenlist), line, &i, sep_type);
	else
	{
		(*tokenlist)->token = gc_strjoin_char((*tokenlist)->token, line[i++]);
		(*tokenlist)->qmask = gc_strjoin_char((*tokenlist)->qmask, '0');
	}
	return (i);
}

void	handle_heredoc_delim(t_tokenlist **tkl, int *tkl_s, char *line, int i)
{
	if (line[i] && (*tkl)->token_type == HEREDOC)
	{
		new_token(tkl, tkl_s);
		(*tkl)->is_delim = 1;
	}
	else if (line[i])
		new_token(tkl, tkl_s);
}
