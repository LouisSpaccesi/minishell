/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:32:20 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:32:22 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**extract_cmd_without_redir(char **args, int *i)
{
	char	**cmd_args;

	*i = 0;
	while (args[*i] && args[*i][0] != '<' && args[*i][0] != '>')
		(*i)++;
	cmd_args = malloc(sizeof(char *) * (*i + 1));
	if (!cmd_args)
		return (NULL);
	return (cmd_args);
}

int	copy_and_parse(char **args, char **cmd_args, t_redir_info *redir,
		t_shell *shell)
{
	int	i;
	int	res;

	i = 0;
	while (args[i] && args[i][0] != '<' && args[i][0] != '>')
	{
		cmd_args[i] = ft_strdup(args[i]);
		i++;
	}
	cmd_args[i] = NULL;
	res = parse_redirection(args, redir);
	if (res == -2)
	{
		free_and_set_status(cmd_args, shell, 2);
		return (-2);
	}
	return (res);
}

void	replace_args(char **args, char **cmd_args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	i = 0;
	while (cmd_args[i])
	{
		args[i] = cmd_args[i];
		i++;
	}
	args[i] = NULL;
}
