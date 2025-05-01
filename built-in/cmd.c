/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:27:19 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:27:20 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_command_array(char **args, int start, int end)
{
	char	**cmd;
	int		i;
	int		j;

	cmd = malloc(sizeof(char *) * (end - start + 1));
	if (!cmd)
		return (NULL);
	i = start;
	j = 0;
	while (i < end)
	{
		cmd[j] = ft_strdup(args[i]);
		if (!cmd[j])
		{
			while (--j >= 0)
				free(cmd[j]);
			free(cmd);
			return (NULL);
		}
		i++;
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

void	execute_first_command(char **cmd1, int temp_fd, int *pipefd,
		t_shell *shell)
{
	char	*cmd_path;

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(temp_fd, STDIN_FILENO);
	close(temp_fd);
	cmd_path = find_command_path(cmd1[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd1[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(cmd_path, cmd1, shell->env);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd1[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	free(cmd_path);
	exit(126);
}

void	execute_second_command(char **cmd2, int *pipefd, int temp_fd,
		t_shell *shell)
{
	char	*cmd_path;

	(void)temp_fd;
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	cmd_path = find_command_path(cmd2[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd2[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(cmd_path, cmd2, shell->env);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd2[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	free(cmd_path);
	exit(126);
}
