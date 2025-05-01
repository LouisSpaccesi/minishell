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
#include <ctype.h>

static int is_valid_identifier(const char *var)
{
	if (!var || (!ft_isalpha(*var) && *var != '_'))
		return (0);
	var++;
	while (*var)
	{
		if (!ft_isalnum(*var) && *var != '_')
			return (0);
		var++;
	}
	return (1);
}

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

int	ft_unset_command(char **args, t_shell *shell)
{
	int	i = 1;
	int	status = 0;

	// Iterate through arguments (variables to unset)
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1; // Mark status as failed
		}
		else
		{
			// Only unset if the identifier is valid
			ft_unset_internal(args[i], shell);
		}
		i++;
	}
	return (status); // Return 0 if all processed identifiers were valid, 1 otherwise
}
