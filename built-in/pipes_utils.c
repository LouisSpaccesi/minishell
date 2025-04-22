/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:36:41 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/22 13:43:29 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "|", 2) == 0)
			count++;
		i++;
	}
	return (count);
}

void	free_command_segments(char ***cmd_segments)
{
	int	i;
	int	j;

	i = 0;
	while (cmd_segments[i])
	{
		j = 0;
		while (cmd_segments[i][j])
		{
			free(cmd_segments[i][j]);
			j++;
		}
		free(cmd_segments[i]);
		i++;
	}
	free(cmd_segments);
}

int	wait_for_children(int pipe_count)
{
	int	i;
	int	status;

	i = 0;
	while (i <= pipe_count)
	{
		waitpid(-1, &status, 0);
		i++;
	}
	return (0);
}

int	execute_command_part1(char **args, char **env_copy)
{
	int	has_heredoc;
	int	has_pipe;

	check_for_heredoc_pipe(args, &has_heredoc, &has_pipe);
	if (has_heredoc && has_pipe)
		return (execute_heredoc_pipe(args, env_copy));
	if (has_heredoc)
		return (execute_command_with_redirection(args, env_copy));
	return (execute_command_part2(args, env_copy));
}

int	execute_command_part2(char **args, char **env_copy)
{
	int	pipe_count;

	pipe_count = count_pipes(args);
	if (pipe_count == 0)
		return (execute_command_with_redirection(args, env_copy));
	return (execute_pipe_without_heredoc(args, env_copy));
}