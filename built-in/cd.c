#include "minishell.h"

static void	cd_error(const char *msg, const char *path, int print_perror)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd((char *)msg, 2);
	if (path)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
	}
	if (print_perror)
		perror(NULL);
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
			printf("%s\n", path);
		return (path);
	}
	else
		return (args[1]);
}

static void	cd_update_env(t_shell *shell, const char *old_pwd_val)
{
	char	new_pwd[PATH_MAX];

	if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
		cd_error("error getting new directory path after chdir", NULL, 1);
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
	if (getcwd(old_pwd, sizeof(old_pwd)) == NULL)
	{
		cd_error("error getting current directory", NULL, 1);
		old_pwd[0] = '\0';
	}
	target_path = cd_get_target_path(args, shell);
	if (!target_path)
		return (1);
	if (chdir(target_path) != 0)
	{
		cd_error("", target_path, 1);
		return (1);
	}
	cd_update_env(shell, old_pwd);
	return (0);
}
