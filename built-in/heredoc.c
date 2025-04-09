/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:01:42 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 14:33:17 by lospacce         ###   ########.fr       */
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

void	handle_heredoc_child(int pipefd[2], char *delimiter)
{
	char	*line;

	close(pipefd[0]);
	write(STDOUT_FILENO, "heredoc> ", 9);
	line = readline("");
	while (line)
	{
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = readline("");
	}
	close(pipefd[1]);
	exit(0);
}

int	setup_heredoc_pipe(int pipefd[2], int *saved_stdin, pid_t *pid)
{
	if (pipe(pipefd) == -1)
		return (-1);
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		close(*saved_stdin);
		return (-1);
	}
	return (0);
}

int	ft_redirect_input_heredoc(char *delimiter)
{
	int		pipefd[2];
	int		saved_stdin;
	pid_t	pid;
	int		status;

	if (setup_heredoc_pipe(pipefd, &saved_stdin, &pid) == -1)
		return (-1);
	if (pid == 0)
		handle_heredoc_child(pipefd, delimiter);
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			close(pipefd[0]);
			close(saved_stdin);
			return (-1);
		}
		close(pipefd[0]);
		return (saved_stdin);
	}
	return (-1);
}
