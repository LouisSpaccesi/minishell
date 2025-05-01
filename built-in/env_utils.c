/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:27:01 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:30:24 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env_entry(const char *var, const char *value)
{
	char	*new_entry;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!value)
		value = "";
	new_entry = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	if (!new_entry)
		return (NULL);
	while (var[i])
	{
		new_entry[i] = var[i];
		i++;
	}
	new_entry[i++] = '=';
	while (value[j])
	{
		new_entry[i + j] = value[j];
		j++;
	}
	new_entry[i + j] = '\0';
	return (new_entry);
}

int	find_env_var_index(const char *var_name, char **envp)
{
	int		i;
	size_t	len;

	if (!envp || !var_name)
		return (-1);
	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*get_env_value(char **envp, const char *var_name)
{
	int		idx;
	char	*entry;
	char	*equal_sign;

	idx = find_env_var_index(var_name, envp);
	if (idx == -1)
		return (NULL);
	entry = envp[idx];
	equal_sign = ft_strchr(entry, '=');
	if (!equal_sign)
		return (NULL);
	return (equal_sign + 1);
}

// Helper function to add a new variable when it doesn't exist
static int	add_new_env_var(t_shell *shell, char *new_entry)
{
	size_t	current_size;
	char	**new_env;
	size_t	i;

	current_size = 0;
	if (shell->env)
		while (shell->env[current_size])
			current_size++;
	new_env = malloc((current_size + 2) * sizeof(char *));
	if (!new_env)
	{
		free(new_entry);
		ft_putstr_fd("minishell: add_new_env_var: allocation failed\n", 2);
		return (1);
	}
	i = 0;
	while (i < current_size)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[current_size] = new_entry;
	new_env[current_size + 1] = NULL;
	free(shell->env);     // Free the old env array
	shell->env = new_env; // Point shell->env to the new array
	return (0);           // Success
}

int	set_env_var(t_shell *shell, const char *var, const char *value)
{
	int		idx;
	char	*new_entry;

	if (!var || !shell)
		return (1);
	idx = find_env_var_index(var, shell->env);
	new_entry = create_env_entry(var, value);
	if (!new_entry)
		return (1);
	if (idx != -1)
	{
		// Variable exists, update it
		free(shell->env[idx]);
		shell->env[idx] = new_entry;
	}
	else
	{
		// Variable is new, call helper to add it
		if (add_new_env_var(shell, new_entry) != 0)
		{
			// Helper function already freed new_entry and printed error on failure
			return (1);
		}
	}
	return (0);
}

char	**duplicate_env(char **envp)
{
	size_t	count;
	char	**new_env;
	size_t	i;

	count = 0;
	while (envp && envp[count])
		count++;
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			while (i > 0)
				free(new_env[--i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}
