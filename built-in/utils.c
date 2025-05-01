#include "minishell.h"

void	close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd)
{
	close(pipefd[0]);
	close(pipefd[1]);
	free_array(cmd1);
	free_array(cmd2);
	close(temp_fd);
}

int	count_args(char **args, int start)
{
	int	count;

	count = start;
	while (args[count])
		count++;
	return (count);
}

void	free_commands(char **cmd1, char **cmd2, char *temp_file)
{
	int	i;

	i = 0;
	while (cmd1[i])
	{
		free(cmd1[i]);
		i++;
	}
	free(cmd1);
	i = 0;
	while (cmd2[i])
	{
		free(cmd2[i]);
		i++;
	}
	free(cmd2);
	unlink(temp_file);
}

int	wait_for_children(int pipe_count)
{
	int	i;
	int	status;

	i = 0;
	while (i <= pipe_count)
	{
		waitpid(-1, &status, 0);
		i++;
	}
	return (0);
}
