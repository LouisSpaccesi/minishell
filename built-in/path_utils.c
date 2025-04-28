/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:50:00 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/28 18:29:47 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_child_process(char *cmd_path, char **args, char **envp,
		int arg_count)
{
	exec_cmd_child(cmd_path, args, envp, arg_count);
	free(cmd_path);
	exit(EXIT_FAILURE);
}

int	handle_parent_process(pid_t pid, char *cmd_path)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	return (status);
}

int	exec_command(char **argv, char **envp)
{
	char	*cmd_path;
	pid_t	pid;
	int		arg_count;

	if (!argv || !argv[0])
		return (1);
	arg_count = count_cmd_args(argv);
	cmd_path = find_command_path(argv[0], envp);
	if (!cmd_path)
		return (handle_command_not_found(argv[0]));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		handle_child_process(cmd_path, argv, envp, arg_count);
	else
		return (handle_parent_process(pid, cmd_path));
	return (EXIT_FAILURE);
}
