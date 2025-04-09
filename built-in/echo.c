/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:04:38 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 13:50:08 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_echo_arg(char *arg)
{
	int		len;
	char	temp[1024];

	len = ft_strlen(arg);
	if (ft_strncmp(arg, "$PWD", 4) == 0)
		ft_pwd_no_nl();
	else if (len >= 2 && arg[0] == '"' && arg[len - 1] == '"')
	{
		ft_strcpy(temp, arg + 1);
		temp[len - 2] = '\0';
		if (ft_strncmp(temp, "$PWD", 4) == 0)
			ft_pwd_no_nl();
		else
			printf("%s", temp);
	}
	else
		printf("%s", arg);
}

static int	check_no_newline(int argc, char **argv, int *i)
{
	int	no_newline;

	no_newline = 0;
	if (*i < argc && ft_strncmp(argv[*i], "-n", 3) == 0)
	{
		no_newline = 1;
		(*i)++;
	}
	return (no_newline);
}

static void	print_echo_args(int argc, char **argv, int i)
{
	while (i < argc)
	{
		print_echo_arg(argv[i]);
		if (i + 1 < argc)
			printf(" ");
		i++;
	}
}

int	ft_echo(int argc, char **argv)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = check_no_newline(argc, argv, &i);
	print_echo_args(argc, argv, i);
	if (!no_newline)
		printf("\n");
	return (0);
}
