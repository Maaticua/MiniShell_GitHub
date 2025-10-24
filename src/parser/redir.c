/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:09:00 by awaegaer          #+#    #+#             */
/*   Updated: 2025/09/30 18:11:29 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	add_redir(t_redir **redir_list, char *file, int type)
{
	t_redir	*new;
	t_redir	*tmp;

	new = gc_malloc(sizeof(t_redir));
	if (!new)
		return (EXIT_FAILURE);
	new->file = gc_strdup(file);
	new->type = type;
	new->next = NULL;
	if (!*redir_list)
		*redir_list = new;
	else
	{
		tmp = *redir_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (EXIT_SUCCESS);
}

int	parse_redirections(t_tokenlist **tokens, t_redir **redir_list)
{
	int			type;
	t_tokenlist	*file_token;

	while (*tokens)
	{
		if ((*tokens)->token_type == REDIR_IN
			|| (*tokens)->token_type == REDIR_OUT
			|| (*tokens)->token_type == APPEND
			|| (*tokens)->token_type == HEREDOC)
		{
			type = (*tokens)->token_type;
			file_token = (*tokens)->next;
			if (!file_token || file_token->token_type != WORD)
			{
				ft_putstr_fd("minishell: syntax error", 2);
				return (EXIT_FAILURE);
			}
			add_redir(redir_list, file_token->token, type);
			*tokens = file_token->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	return (EXIT_SUCCESS);
}
