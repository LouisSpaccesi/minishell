/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:04:38 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/29 17:04:10 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value(char *var_name, char **envp)
{
	int		i;
	size_t	len;

	len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	print_env_var(char *var_name, char **envp)
{
	char	*value;

	if (ft_strncmp(var_name, "PWD", 3) == 0)
		ft_pwd_no_nl();
	else
	{
		value = get_env_value(var_name, envp);
		if (value)
			printf("%s", value);
	}
}

static int	is_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(int argc, char **argv, char **envp)
{
	int i;
	int no_newline;

	i = 1;
	no_newline = 0;
	while (i < argc && is_n_option(argv[i]))
	{
		no_newline = 1;
		i++;
	}
	while (i < argc)
	{
		print_echo_arg(argv[i], envp);
		if (i + 1 < argc)
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}