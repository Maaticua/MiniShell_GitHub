/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_name.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:03:00 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 14:49:13 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*build_tmp_path(char *base, char *pid_str, char *count_str)
{
	char	*tmp1;
	char	*tmp2;
	char	*result;

	tmp1 = gc_strjoin(base, pid_str);
	tmp2 = gc_strjoin(tmp1, "_");
	result = gc_strjoin(tmp2, count_str);
	return (result);
}

char	*generate_tmp_name(void)
{
	static int	count = 0;
	char		*pid_str;
	char		*count_str;

	pid_str = gc_itoa(getpid());
	count_str = gc_itoa(count);
	count++;
	return (build_tmp_path("/tmp/minishell_hd_", pid_str, count_str));
}

int	handle_hd_error(int fdtmp, char *tmp_tpl)
{
	perror("heredoc write");
	close(fdtmp);
	unlink(tmp_tpl);
	return (EXIT_FAILURE);
}

int	open_and_fill_tmp(char *tmp_tpl, t_tokenlist *tk)
{
	int	fdtmp;
	int	ret;

	fdtmp = open(tmp_tpl, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (fdtmp == -1)
	{
		perror("open");
		return (-1);
	}
	ret = hd_fill_tmp(fdtmp, tk->token, tk->token_heredoc_quoted);
	if (fdtmp)
		close(fdtmp);
	if (ret == -1)
	{
		unlink(tmp_tpl);
		return (-1);
	}
	return (0);
}
