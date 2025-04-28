/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:00:27 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/28 12:34:56 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_pipe(t_hd_pipe *hp)
{
	close(hp->pipefd[0]);
	close(hp->pipefd[1]);
	free_array(hp->cmd1);
	free_array(hp->cmd2);
	close(hp->temp_fd);
	unlink(hp->temp_file);
}

int	execute_commands(t_hd_pipe *hp)
{
	hp->pid1 = fork();
	if (hp->pid1 == -1)
	{
		close_pipe(hp);
		return (1);
	}
	if (hp->pid1 == 0)
		execute_first_command(hp->cmd1, hp->temp_fd, hp->pipefd);
	hp->pid2 = fork();
	if (hp->pid2 == -1)
	{
		close(hp->pipefd[0]);
		close(hp->pipefd[1]);
		free_array(hp->cmd1);
		free_array(hp->cmd2);
		close(hp->temp_fd);
		waitpid(hp->pid1, NULL, 0);
		unlink(hp->temp_file);
		return (1);
	}
	if (hp->pid2 == 0)
		execute_second_command(hp->cmd2, hp->pipefd, hp->temp_fd);
	return (0);
}

int	cleanup_and_wait(t_hd_pipe *hp)
{
	int	status;

	close(hp->pipefd[0]);
	close(hp->pipefd[1]);
	close(hp->temp_fd);
	waitpid(hp->pid1, &status, 0);
	waitpid(hp->pid2, &status, 0);
	free_array(hp->cmd1);
	free_array(hp->cmd2);
	unlink(hp->temp_file);
	return (0);
}

int	execute_heredoc_pipe(char **args, char **env_copy)
{
	int			heredoc_idx;
	int			pipe_idx;
	t_hd_pipe	hp;

	(void)env_copy;
	find_special_tokens(args, &heredoc_idx, &pipe_idx);
	if (init_heredoc_pipe(&hp, args, heredoc_idx, pipe_idx) != 0)
		return (1);
	if (setup_second_command(&hp, args, pipe_idx) != 0)
		return (1);
	if (execute_commands(&hp) != 0)
		return (1);
	return (cleanup_and_wait(&hp));
}
