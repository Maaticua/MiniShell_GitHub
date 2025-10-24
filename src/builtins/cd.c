/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awaegaer <awaegaer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:12:07 by macaruan          #+#    #+#             */
/*   Updated: 2025/10/10 18:20:47 by awaegaer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_pwd_env(t_env **env)
{
	char	cwd[4096];
	char	*old_pwd;

	old_pwd = get_env_value(*env, "PWD");
	if (old_pwd)
		set_env_value(env, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(env, "PWD", cwd);
}

int	builtin_cd(char **args)
{
	const char	*path = args[1];
	char		*home_path;

	if (!path || !ft_strcmp(path, "~"))
	{
		home_path = get_env_value(get_shell()->env, "HOME");
		if (!home_path)
		{
			ft_putendl_fd("cd: HOME not set", 2);
			return (1);
		}
		path = home_path;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd_env(&get_shell()->env);
	return (0);
}
