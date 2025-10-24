/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 13:25:27 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/10 11:20:32 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_tokens(char *line, t_tokenlist **tokens, int *tkl_size)
{
	*tokens = tokenize_line(line, tkl_size);
	free(line);
	if (!*tokens)
	{
		gc_free_all();
		get_shell()->last_status = 2;
		ft_putstr_fd("bash: syntax error\n", 2);
		return (EXIT_FAILURE);
	}
	expand_tokens(*tokens);
	return (EXIT_SUCCESS);
}

int	handle_cmds(t_minishell *ml)
{
	ml->cmds = parse_tokenlist(ml->tokens);
	if (!ml->cmds)
	{
		gc_free_all();
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_heredocs(t_tokenlist **ptk)
{
	t_tokenlist	*tk;

	tk = *ptk;
	if (prepare_hd_tmp(tk) == EXIT_FAILURE)
	{
		gc_free_all();
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_parsing(char *line, t_tokenlist **tokens, int *tkl_size)
{
	if (!*line)
		return (EXIT_FAILURE);
	return (handle_tokens(line, tokens, tkl_size));
}

int	handle_execution(t_minishell *ml, char **envp)
{
	if (handle_cmds(ml) == EXIT_SUCCESS)
	{
		ml->shell->last_status = run_cmdlist(ml->cmds, envp);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
