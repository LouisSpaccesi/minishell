/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:31:18 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:23:10 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_heredoc(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if (args[i][0] == '<' && args[i][1] == '<' && !args[i][2])
			count++;
		i++;
	}
	return (count);
}

static int	setup_heredoc(char **args, t_shell *shell)
{
	int	saved_fd;

	saved_fd = handle_heredoc_in_pipe(args, shell);
	if (saved_fd == -1)
	{
		shell->exit_status = 1;
		return (-1);
	}
	return (saved_fd);
}

int	handle_redirections(char **args, t_redir_info *redir_info, t_shell *shell)
{
	int	saved_fd;

	if (count_heredoc(args) > 0)
	{
		saved_fd = setup_heredoc(args, shell);
		if (saved_fd == -1)
			return (-1);
		redir_info->original_fd = STDIN_FILENO;
		return (saved_fd);
	}
	return (parse_and_apply_redirections(args, redir_info, shell));
}
