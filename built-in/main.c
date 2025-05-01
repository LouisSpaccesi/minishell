/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:09:50 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:11:34 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

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

	rl = readline("minishell > ");
	if (is_eof(rl))
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	add_history(rl);
	*args = tokenize_command_line(rl, shell);
	if (!*args)
	{
		free(rl);
		shell->exit_status = 2;
		return (NULL);
	}
	return (rl);
}

static int	find_first_valid_arg(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (-1);
	while (args[i] && args[i][0] == '\0')
		i++;
	if (!args[i])
		return (-1);
	return (i);
}

static char	**read_and_validate_input(t_shell *shell, char ***args_base_ptr,
		char **rl_ptr)
{
	int	first_arg_idx;

	*args_base_ptr = NULL;
	*rl_ptr = read_command_line(shell, args_base_ptr);
	if (!(*rl_ptr))
		return (NULL);
	first_arg_idx = find_first_valid_arg(*args_base_ptr);
	if (first_arg_idx == -1)
	{
		shell->exit_status = 0;
		free(*rl_ptr);
		*rl_ptr = NULL;
		if (*args_base_ptr)
			free_array(*args_base_ptr);
		*args_base_ptr = NULL;
		return (NULL);
	}
	return ((*args_base_ptr) + first_arg_idx);
}

static void	execute_validated_command(t_shell *shell, char **valid_args)
{
	setup_exec_signals();
	shell->exit_status = execute_command_part1(valid_args, shell);
	setup_signals();
}

int	process_command(t_shell *shell)
{
	char	*rl;
	char	**args_base;
	char	**valid_args;

	valid_args = read_and_validate_input(shell, &args_base, &rl);
	if (!rl && !args_base)
		return (0);
	if (!valid_args)
		return (1);
	execute_validated_command(shell, valid_args);
	free(rl);
	if (args_base)
		free_array(args_base);
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
