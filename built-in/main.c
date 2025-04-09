/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 13:50:08 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

char	**copy_all_env(char **envp)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = copy_all_env(envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
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

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_env(shell->env);
	if (shell->original_stdin != -1)
		close(shell->original_stdin);
	free(shell);
}

int	main(int argc, char **argv, char **envp)
{
	char	*rl;
	char	**args;
	t_shell	*shell;
	int		i;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
	{
		return (EXIT_FAILURE);
	}
	while (1)
	{
		dup2(shell->original_stdin, STDIN_FILENO);
		rl = readline("minishell > ");
		if (!rl)
		{
			printf("exit\n");
			break ;
		}
		add_history(rl);
		if (ft_strncmp(rl, "exit", 4) == 0)
		{
			free(rl);
			break ;
		}
		args = ft_split(rl, ' ');
		if (args && args[0])
			execute_command(args, shell->env);
		free(rl);
		if (args)
		{
			i = 0;
			while (args[i])
			{
				free(args[i]);
				i++;
			}
			free(args);
		}
	}
	free_shell(shell);
	return (EXIT_SUCCESS);
}
