/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:12:59 by louis             #+#    #+#             */
/*   Updated: 2025/04/28 12:34:12 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_heredoc_content(int temp_fd, char *delimiter)
{
	char	*line;

	printf("heredoc> ");
	fflush(stdout);
	line = readline("");
	while (line && ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) != 0)
	{
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = readline("");
	}
	if (line)
		free(line);
}

void	ft_restore_output(int saved_stdout)
{
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

void	ft_restore_input(int saved_stdin)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

int	ft_redirect_output(char *filename)
{
	int	file;
	int	saved_stdout;

	file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file == -1)
		return (-1);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		close(file);
		return (-1);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		close(file);
		close(saved_stdout);
		return (-1);
	}
	close(file);
	return (saved_stdout);
}

int	ft_redirect_output_append(char *filename)
{
	int	file;
	int	saved_stdout;

	file = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file == -1)
		return (-1);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		close(file);
		return (-1);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		close(file);
		close(saved_stdout);
		return (-1);
	}
	close(file);
	return (saved_stdout);
}
