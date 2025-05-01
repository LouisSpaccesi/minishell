/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:34:41 by louis             #+#    #+#             */
/*   Updated: 2025/05/01 21:09:02 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_special_tokens(char **args, int *heredoc_idx, int *pipe_idx)
{
	int	i;

	*heredoc_idx = -1;
	*pipe_idx = -1;
	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<", 2) == 0 && args[i][2] == '\0')
			*heredoc_idx = i;
		else if (ft_strncmp(args[i], "|", 1) == 0 && args[i][1] == '\0')
			*pipe_idx = i;
		i++;
	}
}

static int	create_heredoc_temp_file(char *temp_filename)
{
	int	fd;

	ft_strcpy(temp_filename, "/tmp/minishell_heredoc_XXXXXX");
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("minishell: failed to create heredoc file");
		return (-1);
	}
	return (fd);
}

static int	read_and_reopen_heredoc(int write_fd, char *delimiter,
		char *temp_filename)
{
	int	read_fd;

	read_heredoc_content(write_fd, delimiter);
	if (close(write_fd) == -1)
	{
		perror("minishell: failed to close heredoc file after write");
		unlink(temp_filename);
		return (-1);
	}
	read_fd = open(temp_filename, O_RDONLY);
	if (read_fd == -1)
	{
		perror("minishell: failed to reopen heredoc file for read");
		unlink(temp_filename);
		return (-1);
	}
	return (read_fd);
}

int	init_heredoc_pipe(t_hd_pipe *hp, char **args, int heredoc_idx, int pipe_idx)
{
	int		write_fd;
	char	*delimiter;

	if (heredoc_idx == -1 || pipe_idx == -1 || heredoc_idx > pipe_idx)
		return (1);
	write_fd = create_heredoc_temp_file(hp->temp_file);
	if (write_fd == -1)
		return (1);
	delimiter = args[heredoc_idx + 1];
	hp->temp_fd = read_and_reopen_heredoc(write_fd, delimiter, hp->temp_file);
	if (hp->temp_fd == -1)
		return (1);
	hp->cmd1 = create_command_array(args, 0, heredoc_idx);
	if (!hp->cmd1)
	{
		close(hp->temp_fd);
		unlink(hp->temp_file);
		return (1);
	}
	return (0);
}

int	setup_second_command(t_hd_pipe *hp, char **args, int pipe_idx)
{
	hp->cmd2 = create_command_array(args, pipe_idx + 1, count_args(args,
				pipe_idx + 1));
	if (!hp->cmd2)
	{
		free_array(hp->cmd1);
		close(hp->temp_fd);
		unlink(hp->temp_file);
		return (1);
	}
	if (pipe(hp->pipefd) == -1)
	{
		free_array(hp->cmd1);
		free_array(hp->cmd2);
		close(hp->temp_fd);
		unlink(hp->temp_file);
		return (1);
	}
	return (0);
}
