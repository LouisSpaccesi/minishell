/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:24:38 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 18:24:51 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_error(const char *msg, const char *path, int print_errno)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (msg && msg[0] != '\0')
	{
		ft_putstr_fd((char *)msg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (path)
	{
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd(": ", 2);
	}
	if (print_errno)
		ft_putendl_fd(strerror(errno), 2);
	else
		ft_putstr_fd("\n", 2);
}

static char	*cd_get_target_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0'))
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
			cd_error("HOME not set", NULL, 0);
		return (path);
	}
	else if (args[1][0] == '-' && args[1][1] == '\0')
	{
		path = get_env_value(shell->env, "OLDPWD");
		if (!path)
			cd_error("OLDPWD not set", NULL, 0);
		else
			ft_putendl_fd(path, 1);
		return (path);
	}
	else
		return (args[1]);
}

static void	cd_update_env(t_shell *shell, const char *old_pwd_val)
{
	char	new_pwd[PATH_MAX];

	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
		cd_error("error retrieving current directory", NULL, 1);
	else
	{
		if (set_env_var(shell, "PWD", new_pwd) != 0)
			cd_error("failed to update PWD environment variable", NULL, 0);
	}
	if (old_pwd_val && old_pwd_val[0] != '\0')
	{
		if (set_env_var(shell, "OLDPWD", old_pwd_val) != 0)
			cd_error("failed to update OLDPWD environment variable", NULL, 0);
	}
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*target_path;
	char	old_pwd[PATH_MAX];

	if (args[1] && args[2])
	{
		cd_error("too many arguments", NULL, 0);
		return (1);
	}
	old_pwd[0] = '\0';
	target_path = cd_get_target_path(args, shell);
	if (!target_path)
		return (1);
	if (chdir(target_path) != 0)
	{
		cd_error(NULL, target_path, 1);
		return (1);
	}
	cd_update_env(shell, old_pwd);
	return (0);
}
