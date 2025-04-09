/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:46 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 16:05:28 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env(const char *var, const char *value)
{
	char	*new_entry;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_entry = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	if (!new_entry)
		return (NULL);
	while (var[i])
	{
		new_entry[i] = var[i];
		i++;
	}
	new_entry[i++] = '=';
	j = 0;
	while (value[j])
	{
		new_entry[i + j] = value[j];
		j++;
	}
	new_entry[i + j] = '\0';
	return (new_entry);
}

void	copy_env(char **new_env, char **env, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		new_env[j] = env[j];
		j++;
	}
}

void ft_env(char **envp)
{
    int i;

    if (!envp)
        return;
    i = 0;
    while (envp[i])
    {
        if (envp[i] && ft_strchr(envp[i], '=') && ft_strlen(ft_strchr(envp[i], '=') + 1) > 0)
        {
            write(STDOUT_FILENO, envp[i], ft_strlen(envp[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
        i++;
    }
}