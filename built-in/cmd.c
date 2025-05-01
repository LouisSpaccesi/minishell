#include "minishell.h"

char	**create_command_array(char **args, int start, int end)
{
	char	**cmd;
	int		i;
	int		j;

	cmd = malloc(sizeof(char *) * (end - start + 1));
	if (!cmd)
		return (NULL);
	i = start;
	j = 0;
	while (i < end)
	{
		cmd[j] = ft_strdup(args[i]);
		i++;
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

void	execute_first_command(char **cmd1, int temp_fd, int *pipefd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(temp_fd, STDIN_FILENO);
	execvp(cmd1[0], cmd1);
	perror("execvp");
	exit(1);
}

void	execute_second_command(char **cmd2, int *pipefd, int temp_fd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(temp_fd);
	execvp(cmd2[0], cmd2);
	perror("execvp");
	exit(1);
}
