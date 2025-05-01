#include "minishell.h"

int	parse_args(char **args, char **envp)
{
	if (!args || !args[0])
		return (1);
	return (exec_command(args, envp));
}

t_shell *init_shell(char **envp)
{
    t_shell *shell;
    
    shell = malloc(sizeof(t_shell));
    if (!shell)
        return (NULL);
    shell->env = duplicate_env(envp);
    if (!shell->env)
    {
        free(shell);
        return (NULL);
    }
	shell->custom_env = NULL;
    shell->exit_status = 0;
    shell->saved_stdout = -1;
    shell->saved_stdin = -1;
    shell->original_stdin = dup(STDIN_FILENO);
    if (shell->original_stdin == -1)
    {
        perror("minishell: dup failed");
        free_env(shell->env);
        free(shell);
        return (NULL);
    }
    return (shell);
}

void	free_command_segments(char ***cmd_segments, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(cmd_segments[j]);
		j++;
	}
	free(cmd_segments);
}

static void	child_process_exec(char *cmd_path, char **args, t_shell *shell,
								t_redir_info *redir_info)
{
	if (redir_info && redir_info->redir_type != 0)
	{
		if (apply_redirection(redir_info) == -1)
		{
			free(cmd_path);
			perror("minishell: redirection failed");
			exit(EXIT_FAILURE);
		}
	}
	if (execve(cmd_path, args, shell->env) == -1)
	{
		perror("minishell");
		free(cmd_path);
		exit(126);
	}
	exit(EXIT_FAILURE);
}

static int	parent_process_wait(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}

int	execute_external_command(char **args, t_shell *shell,
								 t_redir_info *redir_info)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	cmd_path = find_command_path(args[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork failed");
		free(cmd_path);
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
		child_process_exec(cmd_path, args, shell, redir_info);
	free(cmd_path);
	status = parent_process_wait(pid);
	return (status);
}
