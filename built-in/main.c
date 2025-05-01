/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/29 17:16:14 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*read_command_line(t_shell *shell, char ***args)
{
	char	*rl;

	dup2(shell->original_stdin, STDIN_FILENO);
	rl = readline("minishell > ");
	if (is_eof(rl))
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	add_history(rl);
	if (ft_strncmp(rl, "exit", 4) == 0)
	{
		free(rl);
		return (NULL);
	}
	*args = ft_split(rl, ' ');
	return (rl);
}

int	process_command(t_shell *shell)
{
	char	*rl;
	char	**args;
	int		i;

	args = NULL;
	rl = read_command_line(shell, &args);
	if (!rl)
		return (0);
	if (args && args[0])
	{
		setup_exec_signals();
		// Passer la structure shell complète
		execute_command_part1(args, shell);
		setup_signals();
	}
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
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	setup_signals();
	shell = init_shell(envp);
	if (!shell)
	{
		restore_terminal();
		return (EXIT_FAILURE);
	}
	while (1)
	{
		if (!process_command(shell))
			break ;
	}
	restore_terminal();
	free_shell(shell);
	return (EXIT_SUCCESS);
}
