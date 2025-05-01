/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_basic.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:30:45 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:23:03 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_redirection_type(char *arg)
{
	if (!arg)
		return (0);
	if (arg[0] == '>' && arg[1] == '>' && !arg[2])
		return (2);
	if (arg[0] == '>' && !arg[1])
		return (1);
	if (arg[0] == '<' && arg[1] == '<' && !arg[2])
		return (3);
	if (arg[0] == '<' && !arg[1])
		return (4);
	return (0);
}

void	clean_redir_args(char **args, int index)
{
	int	i;

	if (!args || !args[index] || !args[index + 1])
		return ;
	i = index;
	while (args[i + 2])
	{
		args[i] = args[i + 2];
		i++;
	}
	args[i] = NULL;
	args[i + 1] = NULL;
}

int	ft_redirect_output(const char *filename)
{
	int	fd;
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		ft_putstr_fd("minishell: dup failed\n", 2);
		return (-1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(saved_stdout);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(saved_stdout);
		return (-1);
	}
	close(fd);
	return (saved_stdout);
}

int	ft_redirect_output_append(const char *filename)
{
	int	fd;
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		ft_putstr_fd("minishell: dup failed\n", 2);
		return (-1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		close(saved_stdout);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(saved_stdout);
		return (-1);
	}
	close(fd);
	return (saved_stdout);
}

int	ft_redirect_input(const char *filename)
{
	int	fd;
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		ft_putstr_fd("minishell: dup failed\n", 2);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		close(saved_stdin);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		close(saved_stdin);
		return (-1);
	}
	close(fd);
	return (saved_stdin);
}
