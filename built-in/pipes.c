/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:12:46 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:37:26 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_pipe_segment(char **args, int *start_idx, int seg_size)
{
	char	**segment;
	int		i;

	segment = (char **)malloc(sizeof(char *) * (seg_size + 1));
	if (!segment)
		return (NULL);
	i = 0;
	while (i < seg_size)
	{
		segment[i] = ft_strdup(args[*start_idx]);
		(*start_idx)++;
		i++;
	}
	segment[i] = NULL;
	return (segment);
}

static char	***allocate_cmd_segments(int pipe_count)
{
	char	***cmd_segments;

	cmd_segments = (char ***)malloc(sizeof(char **) * (pipe_count + 2));
	return (cmd_segments);
}

char	***split_command_by_pipes(char **args)
{
	int		i;
	int		j;
	int		pipe_count;
	char	***cmd_segments;

	i = 0;
	j = 0;
	pipe_count = count_pipes(args);
	cmd_segments = allocate_cmd_segments(pipe_count);
	if (!cmd_segments)
		return (NULL);
	while (j <= pipe_count)
	{
		if (!(cmd_segments[j] = create_segment(args, &i,
					count_segment_size(args, i))))
		{
			free_command_segments(cmd_segments, j);
			return (NULL);
		}
		if (args[i] && !ft_strncmp(args[i], "|", 2))
			i++;
		j++;
	}
	cmd_segments[j] = NULL;
	return (cmd_segments);
}

void	setup_child_pipes(int i, int pipe_count, int pipe_fds[2][2],
		int current_pipe)
{
	if (i > 0)
		dup2(pipe_fds[1 - current_pipe][0], STDIN_FILENO);
	if (i < pipe_count)
		dup2(pipe_fds[current_pipe][1], STDOUT_FILENO);
	if (i > 0)
		close(pipe_fds[1 - current_pipe][0]);
	if (i < pipe_count)
	{
		close(pipe_fds[current_pipe][0]);
		close(pipe_fds[current_pipe][1]);
	}
}

static int	handle_optional_heredoc(char **args, t_shell *shell)
{
	int	has_heredoc;
	int	has_pipe;
	int	saved_heredoc_fd;

	check_for_heredoc_pipe(args, &has_heredoc, &has_pipe);
	if (has_heredoc)
	{
		saved_heredoc_fd = handle_heredoc_processing_for_pipe_segment(args,
				shell);
		return (saved_heredoc_fd);
	}
	return (-1);
}

static int	split_exec_cleanup_pipes(char **args, t_shell *shell)
{
	char	***cmd_segments;
	int		status;
	int		pipe_count;

	cmd_segments = split_command_by_pipes(args);
	if (!cmd_segments)
		return (1);
	status = execute_piped_commands_part1(cmd_segments, shell);
	pipe_count = 0;
	while (cmd_segments[pipe_count])
		pipe_count++;
	free_command_segments(cmd_segments, pipe_count);
	return (status);
}

int	execute_pipe_logic(char **args, t_shell *shell)
{
	int	status;
	int	saved_heredoc_fd;

	saved_heredoc_fd = handle_optional_heredoc(args, shell);
	if (saved_heredoc_fd == -1 && shell->exit_status != 0)
		return (shell->exit_status);
	status = split_exec_cleanup_pipes(args, shell);
	if (saved_heredoc_fd != -1)
		ft_restore_fd(STDIN_FILENO, saved_heredoc_fd);
	return (status);
}
