#include "minishell.h"

static char	**copy_args_segment(char **args, int end_index)
{
	char	**cmd_args;
	int		j;
	int		k;

	cmd_args = malloc(sizeof(char *) * (end_index + 1));
	if (!cmd_args)
		return (NULL);
	j = 0;
	while (j < end_index)
	{
		cmd_args[j] = ft_strdup(args[j]);
		if (!cmd_args[j])
		{
			k = 0;
			while (k < j)
			{
				free(cmd_args[k]);
				k++;
			}
			free(cmd_args);
			return (NULL);
		}
		j++;
	}
	cmd_args[j] = NULL;
	return (cmd_args);
}

void	free_copied_args_segment(char **cmd_args)
{
	int	j;

	if (!cmd_args)
		return ;
	j = 0;
	while (cmd_args[j])
	{
		free(cmd_args[j]);
		j++;
	}
	free(cmd_args);
}

int	check_for_heredoc_pipe(char **args, int *has_heredoc, int *has_pipe)
{
	int	i;

	*has_heredoc = 0;
	*has_pipe = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<", 3) == 0)
			*has_heredoc = 1;
		else if (ft_strncmp(args[i], "|", 2) == 0)
			*has_pipe = 1;
		i++;
	}
	return (0);
}

// Handles the parent logic after forking for heredoc
static int	parent_process_heredoc(pid_t pid, int pipefd[2], int saved_stdin,
		int *last_saved_fd, t_shell *shell)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
	{
		if (*last_saved_fd != -1)
			close(*last_saved_fd);
		*last_saved_fd = saved_stdin;
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			ft_putstr_fd("minishell: dup2 failed\n", 2);
			close(pipefd[0]);
			close(saved_stdin);
			shell->exit_status = 1;
			close(pipefd[0]); // Close read end on error too
			return (-2);      // Indicate dup2 failure
		}
	}
	else
	{
		close(pipefd[0]); // Close read end
		if (*last_saved_fd != -1)
			close(*last_saved_fd);
		shell->exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 128
			+ WTERMSIG(status);
		return (-3); // Indicate child failure
	}
	close(pipefd[0]); // Close read end on success
	return (0);       // Success
}

// Simplified main heredoc processing function
static int	process_single_heredoc(char *delimiter, int *last_saved_fd,
		t_shell *shell)
{
	int		pipefd[2];
	int		saved_stdin;
	pid_t	pid;

	if (setup_heredoc_pipe(pipefd, &saved_stdin, &pid) == -1)
		return (-1); // Setup failure (pipe, fork, or initial dup)
	if (pid == 0)
	{
		handle_heredoc_child(pipefd, delimiter); // Child exits
		return (0);                              // Should not be reached
	}
	else
	{
		// Parent handles waiting, dup2, and cleanup
		return (parent_process_heredoc(pid, pipefd, saved_stdin, last_saved_fd,
				shell));
	}
}

// Finds the end index of the first command segment in args
static int	find_pipe_segment_end(char **args)
{
	int	i;

	i = 0;
	while (args[i] && args[i][0] != '<' && args[i][0] != '>'
		&& ft_strncmp(args[i], "|", 2) != 0)
	{
		i++;
	}
	return (i);
}

// Processes all heredocs found in the args list
static int	process_all_heredocs(char **args, int *last_saved_fd,
		t_shell *shell)
{
	int	i;
	int	ret_heredoc;

	i = 0;
	*last_saved_fd = -1; // Initialize here
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<", 2) == 0 && args[i + 1])
		{
			ret_heredoc = process_single_heredoc(args[i + 1], last_saved_fd,
					shell);
			if (ret_heredoc != 0)
			{
				if (*last_saved_fd != -1)
					ft_restore_fd(STDIN_FILENO, *last_saved_fd);
				return (-1); // Error during heredoc processing
			}
		}
		i++;
	}
	return (0); // All heredocs processed successfully
}

// Executes a command segment (copied)
static int	execute_pipe_segment(char **args, int end_idx, t_shell *shell)
{
	char	**cmd_segment_args;
	int		status;

	cmd_segment_args = copy_args_segment(args, end_idx);
	if (!cmd_segment_args)
		return (1); // Allocation error
	status = 0;     // Default status
	if (cmd_segment_args[0])
	{
		if (is_builtin(cmd_segment_args[0]))
			status = dispatch_builtin(cmd_segment_args, shell);
		else
		{
			status = execute_external_command(cmd_segment_args, shell);
		}
	}
	free_copied_args_segment(cmd_segment_args);
	return (status);
}

// Main function to handle heredoc processing and first segment execution
int	handle_heredoc_processing_for_pipe_segment(char **args, t_shell *shell)
{
	int last_saved_fd;
	int cmd_segment_end_idx;
	int status;

	cmd_segment_end_idx = find_pipe_segment_end(args);
	if (process_all_heredocs(args, &last_saved_fd, shell) != 0)
	{
		// Error occurred,
		status set by process_all_heredocs /
			process_single_heredoc
			// last_saved_fd should have been restored if necessary
			return (-1); // Indicate failure to caller
	}

	// Heredocs processed, now execute the first segment
	status = execute_pipe_segment(args, cmd_segment_end_idx, shell);
	shell->exit_status = status;

	// Return the saved fd. The caller (execute_pipe_logic) MUST restore it.
	return (last_saved_fd);
}