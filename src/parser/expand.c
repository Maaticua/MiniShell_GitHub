/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:23:52 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/03 13:32:08 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	try_expand_at(t_expand *exp)
{
	int	consumed;
	int	step;

	if (exp->token->token[exp->i] != '$')
		return (0);
	if (is_sq(exp->token->qmask[exp->i]) || exp->token->token[exp->i
			+ 1] == '\0')
		return (0);
	step = check_empty_quotes(exp);
	if (step > 0)
		return (step);
	append_slice(exp);
	step = try_expand_special_vars(exp);
	if (step > 0)
		return (step);
	consumed = consume_var_len(exp);
	if (consumed <= 0)
		return (0);
	expand_regular_var(exp, consumed);
	exp->start = exp->i + 1 + consumed;
	return (1 + consumed);
}

static void	finalize_remainder(t_expand *exp)
{
	int	len;

	len = (int)ft_strlen(exp->token->token);
	append_slice(exp);
}

void	exp_init(t_expand *exp, t_tokenlist *token)
{
	exp->token = token;
	exp->new_token = gc_strdup("");
	exp->new_mask = gc_strdup("");
	exp->i = 0;
	exp->start = 0;
}

void	expand_one(t_tokenlist *token)
{
	t_expand	exp;
	int			step;

	exp_init(&exp, token);
	while (token->token[exp.i])
	{
		step = try_expand_at(&exp);
		if (step > 0)
			exp.i += step;
		else
			exp.i++;
	}
	finalize_remainder(&exp);
	token->token = exp.new_token;
	token->qmask = exp.new_mask;
}

void	expand_tokens(t_tokenlist *tokenlist)
{
	while (tokenlist)
	{
		if (!tokenlist->is_delim)
			expand_one(tokenlist);
		tokenlist = tokenlist->next;
	}
}
