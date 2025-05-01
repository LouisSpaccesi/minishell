/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:00:48 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/26 16:10:49 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_unset_internal(char *args, t_shell *shell)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(args);
	if (!shell || !shell->env)
		return;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], args, var_len) == 0
			&& shell->env[i][var_len] == '=')
		{
			free(shell->env[i]);
			while (shell->env[i + 1])
			{
				shell->env[i] = shell->env[i + 1];
				i++;
			}
			shell->env[i] = NULL;
			return ;
		}
		i++;
	}
}

void	ft_unset_command(char *rl, t_shell *shell)
{
	char	*args;
	int		i;

	i = 0;
	args = rl + 6;
	while (args && args[i] == ' ')
		i++;
	args = &args[i];

	if (args && *args)
	{
		char *space = ft_strchr(args, ' ');
		if (space)
			*space = '\0';

		ft_unset_internal(args, shell);
	}
}
