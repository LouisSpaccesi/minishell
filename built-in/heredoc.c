/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:04:29 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:04:49 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc_child(int pipefd[2], char *delimiter)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	while ((line = readline("heredoc> ")))
	{
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			close(pipefd[1]);
			exit(0);
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	exit(0);
}

int	setup_heredoc_pipe(int pipefd[2], int *saved_stdin, pid_t *pid)
{
	if (pipe(pipefd) == -1)
	{
		ft_putstr_fd("minishell: pipe creation failed\n", 2);
		return (-1);
	}
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
	{
		ft_putstr_fd("minishell: failed to save stdin\n", 2);
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		ft_putstr_fd("minishell: fork failed\n", 2);
		close(pipefd[0]);
		close(pipefd[1]);
		close(*saved_stdin);
		return (-1);
	}
	return (0);
}
