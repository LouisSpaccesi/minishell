/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:29:18 by louis             #+#    #+#             */
/*   Updated: 2025/04/28 18:30:53 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exec_failure(char *cmd_path, char **cmd_args)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd_path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	if (cmd_args)
		free(cmd_args);
	if (cmd_path)
		free(cmd_path);
	exit(EXIT_FAILURE);
}

void	exec_cmd_child(char *cmd_path, char **args, char **envp, int arg_count)
{
	char	**cmd_args;
	int		i;

	i = 0;
	cmd_args = malloc((arg_count + 1) * sizeof(char *));
	if (!cmd_args)
	{
		perror("malloc");
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
	ft_putstr_fd("Command not found: ", 2);
	ft_putendl_fd(cmd, 2);
	return (0);
}

int	count_cmd_args(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}
