/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:44:47 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/10 13:53:01 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*read_line(size_t *len)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		return (readline("minishell &> "));
	line = get_next_line(STDIN_FILENO);
	if (line)
	{
		*len = ft_strlen(line);
		if (*len > 0 && line[*len - 1] == '\n')
			line[*len -1] = '\0';
	}
	return (line);
}

void	handle_history(char *line)
{
	if (*line && isatty(STDIN_FILENO))
		add_history(line);
}

void	init_loop(t_minishell *ml, char **envp)
{
	ml->shell = get_shell();
	ml->len = 0;
	ml->line = NULL;
	ml->tokens = NULL;
	ml->cmds = NULL;
	ml->shell->env = init_env(envp);
	ml->shell->last_status = 0;
	init_sig();
}

void	reset_loop(t_minishell *ml)
{
	ml->tokens = NULL;
	ml->cmds = NULL;
}

int	main_loop(char **envp)
{
	t_minishell	ml;
	char		**env_cpy;

	init_loop(&ml, envp);
	while (1)
	{
		reset_loop(&ml);
		env_cpy = env_to_array(ml.shell->env);
		ml.line = read_line(&ml.len);
		if (!ml.line)
		{
			if (isatty(STDIN_FILENO))
				ft_printf("exit\n");
			break ;
		}
		handle_history(ml.line);
		if (handle_parsing(ml.line, &ml.tokens, &ml.tkl_size) == EXIT_SUCCESS)
			handle_execution(&ml, env_cpy);
		else if (!*ml.line)
			free(ml.line);
		gc_free_all();
	}
	if (ml.line)
		free(ml.line);
	return (cleanup_and_exit(0, 0), ml.shell->last_status);
}
