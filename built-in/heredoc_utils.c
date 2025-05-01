#include "minishell.h"

void read_heredoc_content(int temp_fd, char *delimiter)
{
	char *line;

	printf("heredoc> ");
	fflush(stdout);
	line = readline("");
	while (line && ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) != 0) {
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = readline("");
	}
	if (line) free(line);
}

void ft_restore_output(int saved_stdout)
{
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

void ft_restore_input(int saved_stdin)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}
