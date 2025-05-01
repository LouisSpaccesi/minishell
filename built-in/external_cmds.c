/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:06:04 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:06:42 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	duplicate_original_fds(t_shell *shell)
{
	shell->original_stdin = dup(STDIN_FILENO);
	if (shell->original_stdin == -1)
	{
		perror("minishell: dup failed for stdin");
		return (-1);
	}
	shell->original_stdout = dup(STDOUT_FILENO);
	if (shell->original_stdout == -1)
	{
		perror("minishell: dup failed for stdout");
		close(shell->original_stdin);
		return (-1);
	}
	return (0);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("minishell: malloc failed for shell");
		return (NULL);
	}
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
	if (duplicate_original_fds(shell) == -1)
	{
		free_env(shell->env);
		free(shell);
		return (NULL);
	}
	return (shell);
}

static void	child_process_exec(char *cmd_path, char **args, t_shell *shell)
{
	if (execve(cmd_path, args, shell->env) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		free(cmd_path);
		exit(126);
	}
	exit(EXIT_FAILURE);
}

static int	handle_parent_execution(pid_t pid, char *cmd_path)
{
	int	status;

	free(cmd_path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status = 128 + WTERMSIG(status);
	else
		status = EXIT_FAILURE;
	return (status);
}

int	execute_external_command(char **args, t_shell *shell)
{
	pid_t	pid;
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
	{
		child_process_exec(cmd_path, args, shell);
		return (EXIT_FAILURE);
	}
	else
		return (handle_parent_execution(pid, cmd_path));
}
