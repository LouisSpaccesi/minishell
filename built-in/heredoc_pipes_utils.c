/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:34:41 by louis             #+#    #+#             */
/*   Updated: 2025/04/28 12:35:04 by louis            ###   ########.fr       */
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
		if (ft_strncmp(args[i], "<<", 3) == 0)
			*heredoc_idx = i;
		else if (ft_strncmp(args[i], "|", 2) == 0)
			*pipe_idx = i;
		i++;
	}
}

int	init_heredoc_pipe(t_hd_pipe *hp, char **args, int heredoc_idx, int pipe_idx)
{
	char	*delimiter;

	if (heredoc_idx == -1 || pipe_idx == -1 || heredoc_idx > pipe_idx)
		return (1);
	ft_strcpy(hp->temp_file, "/tmp/minishell_heredoc_XXXXXX");
	hp->temp_fd = mkstemp(hp->temp_file);
	if (hp->temp_fd == -1)
		return (1);
	delimiter = args[heredoc_idx + 1];
	read_heredoc_content(hp->temp_fd, delimiter);
	lseek(hp->temp_fd, 0, SEEK_SET);
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
