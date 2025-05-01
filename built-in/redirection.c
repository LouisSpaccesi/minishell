/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:28:37 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 21:28:37 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_redirection_type(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strncmp(arg, ">>", 2) == 0 && arg[2] == '\0')
		return (2);
	else if (ft_strncmp(arg, ">", 1) == 0 && arg[1] == '\0')
		return (1);
	else if (ft_strncmp(arg, "<<", 2) == 0 && arg[2] == '\0')
		return (3);
	else if (ft_strncmp(arg, "<", 1) == 0 && arg[1] == '\0')
		return (4);
	return (0);
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
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
					2);
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

int	parse_redirection(char **args, t_redir_info *redir_info)
{
	int	last_redir_index;
	int	redir_type;

	last_redir_index = find_last_redirection(args, &redir_type);
	if (last_redir_index == -1)
		return (-1);
	if (last_redir_index == -2)
		return (-2);
	redir_info->redir_type = redir_type;
	redir_info->filename = args[last_redir_index + 1];
	if (redir_info->redir_type == 1 || redir_info->redir_type == 2)
		redir_info->original_fd = STDOUT_FILENO;
	else if (redir_info->redir_type == 3 || redir_info->redir_type == 4)
		redir_info->original_fd = STDIN_FILENO;
	else
		redir_info->original_fd = -1;
	remove_redirection_args(args, last_redir_index);
	return (0);
}

int	apply_redirection(t_redir_info *redir_info)
{
	int saved_fd;

	saved_fd = -1;
	if (redir_info->redir_type == 1)
		saved_fd = ft_redirect_output(redir_info->filename);
	else if (redir_info->redir_type == 2)
		saved_fd = ft_redirect_output_append(redir_info->filename);
	else if (redir_info->redir_type == 4)
		saved_fd = ft_redirect_input(redir_info->filename);
	return (saved_fd);
}