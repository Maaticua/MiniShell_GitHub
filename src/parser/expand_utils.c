/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:13:52 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/03 13:36:59 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_empty_quotes(t_expand *exp)
{
	if ((exp->token->token[exp->i + 1] == '\"' || exp->token->token[exp->i
				+ 1] == '\'') && exp->token->token[exp->i
			+ 2] == exp->token->token[exp->i + 1])
	{
		append_slice(exp);
		exp->new_token = gc_strjoin(exp->new_token, "");
		exp->new_mask = gc_strjoin(exp->new_mask, "");
		exp->start = exp->i + 3;
		exp->i += 2;
		return (3);
	}
	return (0);
}

int	try_expand_special_vars(t_expand *exp)
{
	int	delta;

	delta = handle_special_vars(exp);
	if (delta == 2)
	{
		exp->start = exp->i + 2;
		return (2);
	}
	return (0);
}
