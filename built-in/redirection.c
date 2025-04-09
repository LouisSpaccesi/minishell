/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:41:24 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 14:34:44 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_last_redirection(char **args, int *redirection_type)
{
	int	i;
	int	last_redir;

	i = 0;
	last_redir = -1;
	*redirection_type = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], ">", 2) == 0)
		{
			last_redir = i;
			*redirection_type = 1;
		}
		else if (ft_strncmp(args[i], ">>", 3) == 0)
		{
			last_redir = i;
			*redirection_type = 2;
		}
		else if (ft_strncmp(args[i], "<<", 3) == 0)
		{
			last_redir = i;
			*redirection_type = 3;
		}
		i++;
	}
	return (last_redir);
}

int	prepare_redirection(char **args, int *redir_idx, int *redir_type)
{
	static int	saved_stdin = -1;

	if (saved_stdin != -1)
	{
		ft_restore_input(saved_stdin);
		saved_stdin = -1;
	}
	*redir_idx = find_last_redirection(args, redir_type);
	if (*redir_idx >= 0)
	{
		if (!args[*redir_idx + 1])
			return (1);
	}
	return (0);
}

int	handle_redirection(char **args, int *saved_stdout)
{
	int			redir_idx;
	int			redir_type;
	static int	saved_stdin = -1;

	if (prepare_redirection(args, &redir_idx, &redir_type) != 0)
		return (1);
	if (redir_idx >= 0)
	{
		if (redir_type == 1)
			*saved_stdout = ft_redirect_output(args[redir_idx + 1]);
		else if (redir_type == 2)
			*saved_stdout = ft_redirect_output_append(args[redir_idx + 1]);
		else if (redir_type == 3)
		{
			saved_stdin = ft_redirect_input_heredoc(args[redir_idx + 1]);
			if (saved_stdin == -1)
				return (1);
		}
		if ((redir_type == 1 || redir_type == 2) && *saved_stdout == -1)
			return (1);
		args[redir_idx] = NULL;
	}
	return (0);
}

char	**create_command_array(char **args, int start, int end)
{
	char	**cmd;
	int		i;
	int		j;

	cmd = malloc(sizeof(char *) * (end - start + 1));
	if (!cmd)
		return (NULL);
	i = start;
	j = 0;
	while (i < end)
	{
		cmd[j] = ft_strdup(args[i]);
		i++;
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

void	execute_first_command(char **cmd1, int temp_fd, int *pipefd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(temp_fd, STDIN_FILENO);
	execvp(cmd1[0], cmd1);
	perror("execvp");
	exit(1);
}

void	execute_second_command(char **cmd2, int *pipefd, int temp_fd)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(temp_fd);
	execvp(cmd2[0], cmd2);
	perror("execvp");
	exit(1);
}

void	free_commands(char **cmd1, char **cmd2, char *temp_file)
{
	int	i;

	i = 0;
	while (cmd1[i])
	{
		free(cmd1[i]);
		i++;
	}
	free(cmd1);
	i = 0;
	while (cmd2[i])
	{
		free(cmd2[i]);
		i++;
	}
	free(cmd2);
	unlink(temp_file);
}

void	close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd)
{
	close(pipefd[0]);
	close(pipefd[1]);
	free_array(cmd1);
	free_array(cmd2);
	close(temp_fd);
}

int	count_args(char **args, int start)
{
	int	count;

	count = start;
	while (args[count])
		count++;
	return (count);
}
