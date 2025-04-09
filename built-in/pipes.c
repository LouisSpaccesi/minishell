/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:45:24 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 14:13:22 by lospacce         ###   ########.fr       */
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

static char	**create_segment(char **args, int *index, int seg_size)
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

static int	count_segment_size(char **args, int i)
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

char	***split_command_by_pipes(char **args)
{
	int		i;
	int		j;
	int		pipe_count;
	int		seg_size;
	char	***cmd_segments;

	pipe_count = count_pipes(args);
	cmd_segments = (char ***)malloc(sizeof(char **) * (pipe_count + 2));
	if (!cmd_segments)
		return (NULL);
	i = 0;
	j = 0;
	while (j <= pipe_count)
	{
		seg_size = count_segment_size(args, i);
		cmd_segments[j] = create_segment(args, &i, seg_size);
		if (!cmd_segments[j])
		{
			while (--j >= 0)
				free(cmd_segments[j]);
			free(cmd_segments);
			return (NULL);
		}
		j++;
	}
	cmd_segments[j] = NULL;
	return (cmd_segments);
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

int	execute_simple_command(char **args, char **env_copy)
{
	return (execute_command_with_redirection(args, env_copy));
}

static void	setup_child_pipes(int i, int pipe_count, int pipe_fds[2][2],
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

static void	handle_parent_pipes(int i, int pipe_count, int pipe_fds[2][2],
		int *current_pipe)
{
	if (i > 0)
		close(pipe_fds[1 - *current_pipe][0]);
	if (i < pipe_count)
		close(pipe_fds[*current_pipe][1]);
	*current_pipe = 1 - *current_pipe;
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
		return (execute_simple_command(cmd_segments[0], env_copy));
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
			execute_simple_command(cmd_segments[i], env_copy);
			exit(0);
		}
		else
			handle_parent_pipes(i, pipe_count, pipe_fds, &current_pipe);
		i++;
	}
	return (0);
}

int	execute_piped_commands(char ***cmd_segments, char **env_copy)
{
	return (execute_piped_commands_part1(cmd_segments, env_copy));
}

static int	check_for_heredoc_pipe(char **args, int *has_heredoc, int *has_pipe)
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

static int	execute_pipe_without_heredoc(char **args, char **env_copy)
{
	char	***cmd_segments;
	int		status;

	cmd_segments = split_command_by_pipes(args);
	if (!cmd_segments)
		return (1);
	status = execute_piped_commands(cmd_segments, env_copy);
	free_command_segments(cmd_segments);
	return (status);
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
		return (execute_simple_command(args, env_copy));
	return (execute_pipe_without_heredoc(args, env_copy));
}

int	execute_command(char **args, char **env_copy)
{
	return (execute_command_part1(args, env_copy));
}