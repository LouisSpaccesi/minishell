/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:36:41 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/22 15:26:31 by lospacce         ###   ########.fr       */
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

void	handle_parent_pipes(int i, int pipe_count, int pipe_fds[2][2],
	int *current_pipe)
{
if (i > 0)
	close(pipe_fds[1 - *current_pipe][0]);
if (i < pipe_count)
	close(pipe_fds[*current_pipe][1]);
*current_pipe = 1 - *current_pipe;
}