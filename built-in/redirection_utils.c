/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:37:21 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:29:14 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_redirection_args(char **args, int index)
{
	int	i;

	if (!args || !args[index] || !args[index + 1])
		return ;
	i = index;
	while (args[i + 2])
	{
		args[i] = args[i + 2];
		i++;
	}
	args[i] = NULL;
	args[i + 1] = NULL;
}

static int	count_heredocs(char **args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<<", 2) == 0)
			count++;
		i++;
	}
	return (count);
}

static char	*find_heredoc_delimiter(char **args)
{
	int	i;

	i = 0;
	while (args[i] && ft_strncmp(args[i], "<<", 2))
		i++;
	if (!args[i] || !args[i + 1])
		return (NULL);
	return (args[i + 1]);
}

static int	fork_heredoc_child(int pipefd[2], char *delimiter)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		handle_heredoc_child(pipefd, delimiter);
		return (-1);
	}
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		return (pipefd[0]);
	}
}

static int	setup_heredoc(char **args)
{
	int		pipefd[2];
	int		i;
	int		result;
	char	*delimiter;

	i = 0;
	while (args[i] && ft_strncmp(args[i], "<<", 2))
		i++;
	if (!args[i] || !args[i + 1])
		return (-1);
	delimiter = args[i + 1];
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	result = fork_heredoc_child(pipefd, delimiter);
	if (result == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	return (result);
}

// Helper to find the heredoc redirection index
static int	find_heredoc_index(char **args)
{
	int	i;

	i = 0;
	while (args[i] && ft_strncmp(args[i], "<<", 2))
		i++;
	if (!args[i] || !args[i + 1])
		return (-1);
	return (i);
}

static int	process_heredoc_path(char **args, t_redir_info *redir_info,
		t_shell *shell)
{
	int	pipe_fd;
	int	i;

	pipe_fd = setup_heredoc(args);
	if (pipe_fd == -1)
	{
		shell->exit_status = 1;
		return (-1);
	}
	i = find_heredoc_index(args);
	if (i != -1)
	{
		remove_redirection_args(args, i);
	}
	else
	{
		close(pipe_fd);
		ft_putstr_fd("minishell: internal error finding heredoc delimiter\n",
			2);
		shell->exit_status = 1;
		return (-1);
	}
	redir_info->original_fd = STDIN_FILENO;
	redir_info->redir_type = 3;
	return (pipe_fd);
}

int	handle_redirections(char **args, t_redir_info *redir_info, t_shell *shell)
{
	if (count_heredocs(args) > 0)
		return (process_heredoc_path(args, redir_info, shell));
	else
		return (parse_redirection(args, redir_info));
}

void	update_args(char **args, char **cmd)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	i = 0;
	while (cmd[i])
	{
		args[i] = cmd[i];
		i++;
	}
	args[i] = NULL;
}

void	restore_redirection(t_redir_info redir_info, int saved_fd,
		t_shell *shell)
{
	if (saved_fd != -1)
	{
		if (ft_restore_fd(redir_info.original_fd, saved_fd) == -1)
			shell->exit_status = 1;
	}
}

static int	apply_heredoc_redirection(int pipe_fd, t_redir_info *redir_info,
		t_shell *shell)
{
	int	saved_fd;

	saved_fd = dup(STDIN_FILENO);
	if (saved_fd == -1 || dup2(pipe_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 failed for heredoc");
		close(pipe_fd);
		if (saved_fd != -1)
			close(saved_fd);
		shell->exit_status = 1;
		return (-1);
	}
	close(pipe_fd);
	redir_info->original_fd = STDIN_FILENO;
	return (saved_fd);
}

static int	apply_other_redirection(t_redir_info *redir_info, t_shell *shell)
{
	int	saved_fd;

	saved_fd = apply_redirection(redir_info);
	if (saved_fd == -1)
	{
		ft_putstr_fd("minishell: redirection failed\n", 2);
		shell->exit_status = 1;
		return (-1);
	}
	return (saved_fd);
}

int	setup_redirection(char **args, t_redir_info *redir_info, t_shell *shell)
{
	int	result_handle;

	result_handle = handle_redirections(args, redir_info, shell);
	if (result_handle == -2)
	{
		shell->exit_status = 2;
		return (-1);
	}
	if (result_handle == -1)
		return (-1);
	if (redir_info->redir_type == 3)
		return (apply_heredoc_redirection(result_handle, redir_info, shell));
	else if (redir_info->redir_type != 0)
		return (apply_other_redirection(redir_info, shell));
	return (-1);
}
