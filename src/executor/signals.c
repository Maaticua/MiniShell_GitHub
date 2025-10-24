/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:33:19 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 15:44:10 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	sig_handle(int sig)
{
	(void)sig;
	get_shell()->last_status = 130;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_handle_heredoc(int sig)
{
	(void)sig;
	close(HD_FD);
	write(STDOUT_FILENO, "\n", 1);
	cleanup_and_exit(130, 1);
}

void	init_sig(void)
{
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	init_sig_heredoc(void)
{
	signal(SIGINT, sig_handle_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_handle_quit(int sig)
{
	(void)sig;
	ft_printf("Quit (core dumped)\n");
}
