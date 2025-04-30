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
