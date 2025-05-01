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

int	ft_echo(char **args, t_shell *shell)
{
	int i;
	int no_newline;

	(void)shell; // Mark shell as unused for now, might be needed for expansion later

	i = 1;
	no_newline = 0;
	// Check for -n option(s)
	while (args[i] && is_n_option(args[i]))
	{
		no_newline = 1;
		i++;
	}
	// Print arguments
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}