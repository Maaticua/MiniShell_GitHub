/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:44:53 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/03 11:58:01 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_key(const char *key)
{
	char	*val;

	if (ft_strncmp(key, "?", 2) == 0)
		return (gc_itoa(get_shell()->last_status));
	if (ft_strncmp(key, "$", 2) == 0)
		return (gc_itoa(getpid()));
	if (ft_isdigit(key[0]) && key[1] == '\0')
		return (gc_strdup(""));
	val = get_env_value(get_shell()->env, key);
	if (!val)
		return (gc_strdup(""));
	return (gc_strdup(val));
}

int	handle_special_vars(t_expand *exp)
{
	char	*val;

	if (exp->token->token[exp->i + 1] == '?')
		val = gc_itoa(get_shell()->last_status);
	else if (exp->token->token[exp->i + 1] == '$')
		val = gc_itoa(getpid());
	else
		return (0);
	exp->new_token = gc_strjoin(exp->new_token, val);
	append_mask_repeated(&exp->new_mask, exp->token->qmask[exp->i], val);
	return (2);
}

int	consume_var_len(t_expand *exp)
{
	int	j;

	j = exp->i + 1;
	if (ft_isdigit(exp->token->token[j]))
		return (1);
	if (!is_var_start(exp->token->token[j]))
		return (0);
	while (exp->token->token[j] && is_var_char(exp->token->token[j]))
		j++;
	return (j - (exp->i + 1));
}

void	expand_regular_var(t_expand *exp, int len)
{
	char	*key;
	char	*val;

	key = gc_substr(exp->token->token, (unsigned int)(exp->i + 1), (size_t)len);
	val = expand_key(key);
	exp->new_token = gc_strjoin(exp->new_token, val);
	append_mask_repeated(&exp->new_mask, exp->token->qmask[exp->i], val);
}
