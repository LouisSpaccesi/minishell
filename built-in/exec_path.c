/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:28:45 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:28:46 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_exec_error(char *cmd_path)
{
	int			exit_code;
	struct stat	st;

	exit_code = 126;
	if (errno == ENOENT)
	{
		ft_putendl_fd("command not found", 2);
		exit_code = 127;
	}
	else if (errno == EACCES)
	{
		if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
			ft_putendl_fd("Is a directory", 2);
		else
			ft_putendl_fd("Permission denied", 2);
	}
	else
		ft_putendl_fd(strerror(errno), 2);
	return (exit_code);
}

void	handle_exec_failure(char *cmd_path, char **cmd_args)
{
	int	exit_code;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_path, 2);
	ft_putstr_fd(": ", 2);
	exit_code = print_exec_error(cmd_path);
	if (cmd_args)
		free(cmd_args);
	if (cmd_path)
		free(cmd_path);
	exit(exit_code);
}

void	exec_cmd_child(char *cmd_path, char **args, char **envp, int arg_count)
{
	char	**cmd_args;
	int		i;

	i = 0;
	cmd_args = malloc((arg_count + 1) * sizeof(char *));
	if (!cmd_args)
	{
		ft_putstr_fd("minishell: malloc failed\n", 2);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	while (i < arg_count)
	{
		cmd_args[i] = args[i];
		i++;
	}
	cmd_args[i] = NULL;
	execve(cmd_path, cmd_args, envp);
	handle_exec_failure(cmd_path, cmd_args);
}

int	handle_command_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	return (127);
}

int	count_cmd_args(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}
