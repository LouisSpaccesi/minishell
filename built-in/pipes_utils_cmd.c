/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:32:52 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/22 15:25:50 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_segment(char **args, int *index, int seg_size)
{
	char	**segment;
	int		k;

	segment = (char **)malloc(sizeof(char *) * (seg_size + 1));
	if (!segment)
		return (NULL);
	k = 0;
	while (args[*index] && ft_strncmp(args[*index], "|", 2) != 0)
	{
		segment[k] = ft_strdup(args[*index]);
		(*index)++;
		k++;
	}
	segment[k] = NULL;
	if (args[*index])
		(*index)++;
	return (segment);
}

int	count_segment_size(char **args, int i)
{
	int	k;

	k = 0;
	while (args[i + k] && ft_strncmp(args[i + k], "|", 2) != 0)
	{
		if (args[i + k] && ft_strncmp(args[i + k], "<<", 3) == 0 && args[i + k
				+ 1])
			k += 2;
		else
			k++;
	}
	return (k);
}

int	execute_piped_commands_part1(char ***cmd_segments, char **env_copy)
{
	int		i;
	int		pipe_count;
	pid_t	*pids;
	int		result;

	i = 0;
	while (cmd_segments[i])
		i++;
	pipe_count = i - 1;
	if (pipe_count == 0)
		return (execute_command_with_redirection(cmd_segments[0], env_copy));
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	if (!pids)
		return (1);
	result = execute_piped_commands_setup(cmd_segments, env_copy, pipe_count,
			pids);
	if (result != 0)
	{
		free(pids);
		return (result);
	}
	result = wait_for_children(pipe_count);
	free(pids);
	return (result);
}

int	init_command_pipes(int i, int pipe_count, int pipe_fds[2][2],
		int current_pipe)
{
	if (i < pipe_count)
	{
		if (pipe(pipe_fds[current_pipe]) == -1)
			return (1);
	}
	return (0);
}

int	execute_piped_commands_setup(char ***cmd_segments, char **env_copy,
		int pipe_count, pid_t *pids)
{
	int	i;
	int	pipe_fds[2][2];
	int	current_pipe;

	i = 0;
	current_pipe = 0;
	while (i <= pipe_count)
	{
		if (init_command_pipes(i, pipe_count, pipe_fds, current_pipe) != 0)
			return (1);
		pids[i] = fork();
		if (pids[i] == -1)
			return (1);
		if (pids[i] == 0)
		{
			setup_child_pipes(i, pipe_count, pipe_fds, current_pipe);
			execute_command_with_redirection(cmd_segments[i], env_copy);
			exit(0);
		}
		else
			handle_parent_pipes(i, pipe_count, pipe_fds, &current_pipe);
		i++;
	}
	return (0);
}
