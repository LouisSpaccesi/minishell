/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:31:59 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/29 17:04:16 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_simple_builtins(char **args, char **env_copy, int saved_stdout)
{
	t_shell	shell;

	if (ft_strncmp(args[0], "env", 4) == 0)
		ft_env(env_copy);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		ft_pwd();
	else if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		if (saved_stdout != -1)
			ft_restore_output(saved_stdout);
		shell.env = env_copy;
		shell.exit_status = 0;
		ft_exit(args, &shell);
	}
}

void	handle_echo_cd(char **args, char **envp)
{
	int		argc;
	char	cmd[1024];

	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		argc = 0;
		while (args[argc])
			argc++;
		ft_echo(argc, args, envp);
	}
	else if (ft_strncmp(args[0], "cd", 3) == 0)
	{
		ft_strcpy(cmd, "cd ");
		if (args[1])
			ft_strcat(cmd, args[1]);
		ft_cd(cmd);
	}
}

void	handle_env_builtins(char **args, char ***env_copy)
{
	char	cmd[1024];

	if (ft_strncmp(args[0], "export", 7) == 0)
	{
		ft_strcpy(cmd, "export ");
		if (args[1])
			ft_strcat(cmd, args[1]);
		ft_export(cmd, env_copy);
	}
	else if (ft_strncmp(args[0], "unset", 6) == 0)
	{
		ft_strcpy(cmd, "unset ");
		if (args[1])
			ft_strcat(cmd, args[1]);
		ft_unset_command(cmd, env_copy);
	}
}

void	handle_complex_builtins(char **args, char **env_copy)
{
	if (ft_strncmp(args[0], "echo", 5) == 0 || ft_strncmp(args[0], "cd",
			3) == 0)
		handle_echo_cd(args, env_copy);
	else
		handle_env_builtins(args, &env_copy);
}

int	execute_command_with_redirection(char **args, char **env_copy)
{
	t_command	cmd;
	int			saved_stdout;

	cmd.args = args;
	cmd.is_builtin = is_builtin(args[0]);
	cmd.redirection = 0;
	cmd.redir_file = NULL;
	saved_stdout = -1;
	if (handle_redirection(args, &saved_stdout) != 0)
		return (1);
	if (cmd.is_builtin)
	{
		if (ft_strncmp(args[0], "env", 4) == 0 || ft_strncmp(args[0], "pwd",
				4) == 0 || ft_strncmp(args[0], "exit", 5) == 0)
			handle_simple_builtins(args, env_copy, saved_stdout);
		else
			handle_complex_builtins(args, env_copy);
	}
	else
		parse_args(args, env_copy);
	if (saved_stdout != -1)
		ft_restore_output(saved_stdout);
	return (0);
}
