/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:27:56 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:27:57 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array && env_array[i])
	{
		ft_putstr_fd(env_array[i], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

int	ft_env_builtin(t_shell *shell)
{
	t_env_var	*current;

	if (!shell)
		return (1);
	print_env_array(shell->env);
	current = shell->custom_env;
	while (current)
	{
		ft_putstr_fd(current->name, STDOUT_FILENO);
		write(STDOUT_FILENO, "=", 1);
		ft_putstr_fd(current->value, STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		current = current->next;
	}
	return (0);
}
