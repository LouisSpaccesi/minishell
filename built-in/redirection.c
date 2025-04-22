/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:41:24 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/22 13:34:58 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int get_redirection_type(char *arg)
{
    if (ft_strncmp(arg, ">", 2) == 0)
        return (1);
    else if (ft_strncmp(arg, ">>", 3) == 0)
        return (2);
    else if (ft_strncmp(arg, "<<", 3) == 0)
        return (3);
    return (0);
}

int find_last_redirection(char **args, int *redirection_type)
{
    int i;
    int last_redir;
    int current_type;

    i = 0;
    last_redir = -1;
    *redirection_type = 0;
    while (args[i])
    {
        current_type = get_redirection_type(args[i]);
        if (current_type != 0)
        {
            last_redir = i;
            *redirection_type = current_type;
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


