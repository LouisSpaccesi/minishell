/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:45:24 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/30 18:10:56 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_command_segment(char **args, int *start_idx, int seg_size)
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
		i++;
	}
	segment[i] = NULL;
	return (segment);
}

static char ***allocate_cmd_segments(int pipe_count)
{
    char ***cmd_segments = (char ***)malloc(sizeof(char **) * (pipe_count + 2));
    return cmd_segments;
}

char ***split_command_by_pipes(char **args)
{
    int i = 0, j = 0, pipe_count, seg_size;
    char ***cmd_segments;
    
    pipe_count = count_pipes(args);
    if (!(cmd_segments = allocate_cmd_segments(pipe_count)))
        return (NULL);
    while (j <= pipe_count)
    {
        seg_size = count_segment_size(args, i);
        if (!(cmd_segments[j] = create_segment(args, &i, seg_size)))
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

int	check_for_heredoc_pipe(char **args, int *has_heredoc, int *has_pipe)
{
	int	i;

	*has_heredoc = 0;
	*has_pipe = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<", 3) == 0)
			*has_heredoc = 1;
		else if (ft_strncmp(args[i], "|", 2) == 0)
			*has_pipe = 1;
		i++;
	}
	return (0);
}

int	execute_pipe_without_heredoc(char **args, char **env_copy)
{
	char	***cmd_segments;
	int		status;
	int		pipe_count;

	cmd_segments = split_command_by_pipes(args);
	if (!cmd_segments)
		return (1);
	status = execute_piped_commands_part1(cmd_segments, env_copy);
	pipe_count = 0;
	while (cmd_segments[pipe_count])
		pipe_count++;
	free_command_segments(cmd_segments, pipe_count);
	return (status);
}
