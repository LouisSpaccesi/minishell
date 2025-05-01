/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:41:24 by lospacce          #+#    #+#             */
/*   Updated: 2025/05/01 12:45:00 by Cascade          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Type 1: >
// Type 2: >>
// Type 3: <<
// Type 4: <
static int  get_redirection_type(char *arg)
{
    if (!arg)
        return (0);
    if (ft_strncmp(arg, ">>", 3) == 0)
        return (2);
    else if (ft_strncmp(arg, ">", 2) == 0)
        return (1);
    else if (ft_strncmp(arg, "<<", 3) == 0)
        return (3);
    else if (ft_strncmp(arg, "<", 2) == 0)
        return (4);
    return (0);
}

// Helper function to remove redirection operator and filename from args
// Shifts args[index+2] and onwards two positions to the left.
static void remove_redirection_args(char **args, int index)
{
    int i;

    // No need to free args[index] or args[index+1] here,
    // they are part of the original args array managed elsewhere.
    if (!args || !args[index] || !args[index + 1])
        return;
    i = index;
    while (args[i + 2])
    {
        args[i] = args[i + 2];
        i++;
    }
    args[i] = NULL;
    args[i + 1] = NULL; // Ensure the next slot is also NULL
}

int	find_last_redirection(char **args, int *redirection_type)
{
	int	i;
	int	last_redir;
	int	current_type;

	i = 0;
	last_redir = -1;
	*redirection_type = 0;
	if (!args)
		return (-1);
	while (args[i])
	{
		current_type = get_redirection_type(args[i]);
		if (current_type != 0)
		{
			if (!args[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				return (-2);
			}
			last_redir = i;
			*redirection_type = current_type;
		}
		i++;
	}
	return (last_redir);
}

int	ft_restore_fd(int original_fd, int saved_fd)
{
	if (saved_fd != -1)
	{
		if (dup2(saved_fd, original_fd) == -1)
		{
			perror("minishell: restore fd failed");
			close(saved_fd);
			return (-1);
		}
		close(saved_fd);
	}
	return (0);
}

int	ft_redirect_input_heredoc(char *delimiter)
{
	(void)delimiter;
	ft_putstr_fd("minishell: heredoc not implemented yet\n", 2);
	return (-1);
}

int	parse_redirection(char **args, t_redir_info *redir_info)
{
	int	last_redir_index;
	int	redir_type;

	last_redir_index = find_last_redirection(args, &redir_type);
	if (last_redir_index == -1)
		return (-1);
	if (last_redir_index == -2)
		return (-2);
	redir_info->type = redir_type;
	redir_info->filename = args[last_redir_index + 1];
	if (redir_info->type == 1 || redir_info->type == 2)
		redir_info->original_fd = STDOUT_FILENO;
	else if (redir_info->type == 3 || redir_info->type == 4)
		redir_info->original_fd = STDIN_FILENO;
	else
		redir_info->original_fd = -1;
	remove_redirection_args(args, last_redir_index);
	return (0);
}

int	apply_redirection(t_redir_info *redir_info)
{
	int	saved_fd;

	saved_fd = -1;
	if (redir_info->type == 1)
		saved_fd = ft_redirect_output(redir_info->filename);
	else if (redir_info->type == 2)
		saved_fd = ft_redirect_output_append(redir_info->filename);
	else if (redir_info->type == 3)
		saved_fd = ft_redirect_input_heredoc(redir_info->filename);
	else if (redir_info->type == 4)
		saved_fd = ft_redirect_input(redir_info->filename);
	return (saved_fd);
}
