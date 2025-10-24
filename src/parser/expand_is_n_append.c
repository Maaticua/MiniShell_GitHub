/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_is_n_append.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:36:19 by awaegaer          #+#    #+#             */
/*   Updated: 2025/09/27 16:36:42 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_sq(char m)
{
	return (m == '1');
}

int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	append_slice(t_expand *exp)
{
	int	len;

	len = exp->i - exp->start;
	if (len > 0)
	{
		exp->new_token = gc_strjoin_len(
				exp->new_token, exp->token->token + exp->start, (size_t)len);
		exp->new_mask = gc_strjoin_len(
				exp->new_mask, exp->token->qmask + exp->start, (size_t)len);
	}
	exp->start = exp->i;
}

void	append_mask_repeated(char **dst_m, char mask, const char *val)
{
	int	i;

	i = 0;
	while (val[i])
	{
		*dst_m = gc_strjoin_char(*dst_m, mask);
		i++;
	}
}
