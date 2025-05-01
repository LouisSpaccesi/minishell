#include "minishell.h"
#include <errno.h>  // For errno
#include <string.h> // For strerror

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
		// Add error check for ft_strdup?
		if (!cmd[j])
		{
			while (--j >= 0)
				free(cmd[j]);
			free(cmd);
			return (NULL);
		}
		i++;
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

// Added t_shell *shell parameter
void	execute_first_command(char **cmd1, int temp_fd, int *pipefd, t_shell *shell)
{
	char *cmd_path;

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(temp_fd, STDIN_FILENO);
	close(temp_fd); // Close the original heredoc fd

	cmd_path = find_command_path(cmd1[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd1[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		// Need to free cmd1? Assumed handled by caller
		exit(127);
	}
	execve(cmd_path, cmd1, shell->env); // Use execve and found path
	// If execve returns, an error occurred
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd1[0], 2); // Use cmd1[0] for error msg
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2); // Replace perror
	free(cmd_path);
	// Need to free cmd1? Assumed handled by caller
	exit(126); // Use 126 for exec errors other than not found
}

// Added t_shell *shell parameter
void	execute_second_command(char **cmd2, int *pipefd, int temp_fd, t_shell *shell)
{
	char *cmd_path;

	(void)temp_fd; // temp_fd seems unused here now? Check logic. It was closed in execute_first_command. Okay.
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	// close(temp_fd); // Already closed in parent or first command

	cmd_path = find_command_path(cmd2[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd2[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		// Need to free cmd2? Assumed handled by caller
		exit(127);
	}
	execve(cmd_path, cmd2, shell->env); // Use execve and found path
	// If execve returns, an error occurred
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd2[0], 2); // Use cmd2[0] for error msg
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2); // Replace perror
	free(cmd_path);
	// Need to free cmd2? Assumed handled by caller
	exit(126); // Use 126 for exec errors other than not found
}
