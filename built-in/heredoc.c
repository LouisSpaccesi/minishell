#include "minishell.h"

void	handle_heredoc_child(int pipefd[2], char *delimiter)
{
	char	*line;

	close(pipefd[0]);
	write(STDOUT_FILENO, "heredoc> ", 9);
	line = readline("");
	while (line)
	{
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0) {
			free(line);
			break;
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
	if (pipe(pipefd) == -1) return (-1);
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1) {
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	*pid = fork();
	if (*pid == -1) {
		close(pipefd[0]);
		close(pipefd[1]);
		close(*saved_stdin);
		return (-1);
	}
	return (0);
}
