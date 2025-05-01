/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:36:41 by lospacce          #+#    #+#             */
/*   Updated: 2025/05/01 20:00:12 by fben-ham         ###   ########.fr       */
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

int	execute_command_part1(char **args, t_shell *shell)
{
	int	has_heredoc;
	int	has_pipe;

	check_for_heredoc_pipe(args, &has_heredoc, &has_pipe);
	if (has_heredoc && has_pipe)
		return (execute_heredoc_pipe(args, shell));
	if (has_heredoc)
		return (execute_command_with_redirection(args, shell));
	return (execute_command_part2(args, shell));
}

int	execute_command_part2(char **args, t_shell *shell)
{
	int	pipe_count;

	pipe_count = count_pipes(args);
	if (pipe_count == 0)
		return (execute_command_with_redirection(args, shell));
	return (execute_pipe_logic(args, shell));
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

void	free_command_segments(char ***cmd_segments, int count)
{
	int	j;

	if (!cmd_segments)
		return ;
	j = 0;
	while (j < count)
	{
		if (cmd_segments[j])
			free_array(cmd_segments[j]);
		j++;
	}
	free(cmd_segments);
}
