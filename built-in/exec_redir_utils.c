/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:37:21 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:32:24 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	handle_heredoc_redir(char **args, t_redir_info *redir,
		t_shell *shell)
{
	int	fd;

	fd = handle_heredoc_child(args, shell);
	if (fd == -1)
		return (-1);
	redir->original_fd = STDIN_FILENO;
	redir->redir_type = 3;
	return (fd);
}

int	apply_redir(char **args, t_redir_info *redir, t_shell *shell)
{
	int	ret;

	ret = parse_redirection(args, redir);
	if (ret == -1)
		return (-1);
	if (ret == 0 && redir->redir_type == 3)
		return (handle_heredoc_redir(args, redir, shell));
	return (ret);
}
