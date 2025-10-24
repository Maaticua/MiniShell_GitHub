/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macaruan <macaruan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:12:33 by awaegaer          #+#    #+#             */
/*   Updated: 2025/10/03 14:47:34 by macaruan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// void	print_banner(void)
// {
// 	ft_printf("\033[1;36m");
// 	ft_printf("  __    __   __   __   __   __   ______
//__  __   ______   __       __        \n");
// 	ft_printf(" /\\ \"-./  \\ /\\ \\ /\\ \"-.\\ \\ /\\ \\ /
//\\  ___\\ /\\ \\_\\ \\ /\\  ___\\ /\\ \\     /\\ \\       \n");
// 	ft_printf(" \\ \\ \\-./\\ \\\\ \\ \\\\ \\ \\-.  \\\\ \\ \\\\ \\__
//_  \\\\ \\  __ \\\\ \\  __\\ \\ \\ \\____\\ \\ \\____  \n");
// 	ft_printf("  \\ \\_\\ \\ \\_\\\\ \\_\\\\ \\_\\\"\\_\\\\ \\_\\\\/
//\\_____\\\\ \\_\\ \\_\\\\ \\_____\\\\ \\_____\\\\ \\_____\\ \n");
// 	ft_printf("   \\/_/  \\/_/ \\/_/ \\/_/ \\/_/ \\/_/
// \\/_____/ \\/_/\\/_/ \\/_____/ \\/_____/ \\/_____/ \n");
// 	ft_printf("\033[1;33m                              by
// Maati & Arthur\n\n\033[0m");
// }

// int	main_loop_test(char *argv, char **envp)
// {
// 	char 		*line;
// 	int			tkl_size;
// 	int			abort_line;
// 	t_tokenlist	*tokens;
// 	t_cmd		*cmds;
// 	t_cmd		*first_cmd;
// 	size_t		len;

// 	len = -1;
// 	line = NULL;
// 	init_sig();
// 	get_shell()->env = init_env(envp);
// 	get_shell()->last_status = 0;
// 	tokens = NULL;
// 	cmds = NULL;
// 	first_cmd = NULL;
// 	abort_line = 0;
// 	line = ft_strdup(argv);
// 	if (line)
// 	{
// 		tokens = tokenize_line(line, &tkl_size);
// 		free(line);
// 		if (!tokens)
// 		{
// 			gc_free_all();
// 			get_shell()->last_status = 1;
// 			ft_putstr_fd("bash: syntax error\n", 1);
// 		}
// 		expand_tokens(tokens);
// 		cmds = parse_tokenlist(tokens);
// 		if (!cmds)
// 		{
// 			gc_free_all();
// 		}
// 		first_cmd = cmds;
// 		while (first_cmd)
// 		{
// 			if (first_cmd->heredoc == 1 && first_cmd->heredoc_delim)
// 			{
// 				if (prepare_hd_tmp(cmds) == EXIT_FAILURE)
// 				{
// 					abort_line = 1;
// 					break;
// 				}
// 			}
// 			first_cmd = first_cmd->next;
// 		}
// 		if (abort_line)
// 		{
// 			gc_free_all();
// 			get_shell()->last_status = 129;
// 			ft_putstr_fd("invalid target\n", 1);
// 		}
// 		get_shell()->last_status = run_cmdlist(cmds, envp);
// 	}
// 	else
// 		free(line);
// 	// gc_free_all();
// 	// if (line)
// 	// 	free(line);
// 	gc_free_all();
// 	free_env(&get_shell()->env);
// 	return (get_shell()->last_status);
// }

int	main(int argc, char **argv, char **envp)
{
	int	exit_status;

	(void)argc;
	(void)argv;
	exit_status = main_loop(envp);
	return (exit_status);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	int exit_status;
// 	if (argc == 2)
// 		exit_status = main_loop_test(argv[1], envp);
// 	else
// 		exit_status = main_loop(envp);
// 	return exit_status;
// }
