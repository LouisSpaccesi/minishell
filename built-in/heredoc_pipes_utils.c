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
		return (1); // Invalid sequence or missing tokens

	// Use a fixed temporary file name
	ft_strcpy(hp->temp_file, "/tmp/minishell_heredoc"); // Fixed name

	// 1. Open for writing (create/truncate) - Replaces mkstemp part 1
	hp->temp_fd = open(hp->temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (hp->temp_fd == -1)
	{
		// Add error message?
		ft_putstr_fd("minishell: failed to create heredoc file\n", 2);
		return (1); // Return error if file creation fails
	}

	// 2. Read heredoc content and write to the file
	delimiter = args[heredoc_idx + 1];
	read_heredoc_content(hp->temp_fd, delimiter); // Assume this func handles write errors

	// 3. Close the write file descriptor - Replaces lseek part 1
	if (close(hp->temp_fd) == -1)
	{
		// Add error message?
		ft_putstr_fd("minishell: failed to close heredoc file after write\n", 2);
		unlink(hp->temp_file); // Clean up the file
		return (1);
	}

	// 4. Reopen the file for reading - Replaces lseek part 2
	hp->temp_fd = open(hp->temp_file, O_RDONLY);
	if (hp->temp_fd == -1)
	{
		// Add error message?
		ft_putstr_fd("minishell: failed to reopen heredoc file for read\n", 2);
		unlink(hp->temp_file); // Clean up the file
		return (1); // Return error if reopening fails
	}

	// 5. Prepare the first command
	// Note: count_args needs adjusting or validating if heredoc_idx is last arg
	hp->cmd1 = create_command_array(args, 0, heredoc_idx);
	if (!hp->cmd1)
	{
		close(hp->temp_fd); // Close the read fd
		unlink(hp->temp_file);
		return (1);
	}

	// Success
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
