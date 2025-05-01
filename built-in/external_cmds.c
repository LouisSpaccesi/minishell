/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:40:00 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/30 18:29:43 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    shell->env = copy_env_safe(envp);
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

int	execute_external_command(char **args, t_shell *shell, t_redir_info *redir_info)
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
		return (1);
	}
	else if (pid == 0)
	{
		if (redir_info && redir_info->redir_type != 0)
		{
			if (apply_redirection(redir_info) == -1)
			{
				free(cmd_path);
				exit(EXIT_FAILURE);
			}
		}

		if (execve(cmd_path, args, shell->env) == -1)
		{
			perror("minishell");
			free(cmd_path);
			exit(126);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (status);
}
