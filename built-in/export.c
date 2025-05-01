/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:10:52 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/30 18:31:59 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*variable_start(char *rl)
{
	int		i;
	char	*args;

	i = 0;
	args = rl + 6;
	while (args[i] == ' ')
		i++;
	if (args[i] == '\0')
		return (NULL);
	return (&args[i]);
}

static char	*extract_variable_value(char *rl, char **value_ptr)
{
	char	*var_start;
	char	*equals_pos;
	char	*var;
	int		var_len;

	var_start = variable_start(rl);
	if (!var_start)
		return (NULL);
	equals_pos = ft_strchr(var_start, '=');
	if (!equals_pos)
		return (NULL);
	var_len = equals_pos - var_start;
	var = malloc(var_len + 1);
	if (!var)
		return (NULL);
	ft_strlcpy(var, var_start, var_len + 1);
	var[var_len] = '\0';
	*value_ptr = equals_pos + 1;
	return (var);
}

static int replace_variable(char **env, char *var, char *new_entry)
{
    int i;
    int var_len;
    
    i = 0;
    var_len = ft_strlen(var);
    while (env && env[i]) 
    {
        if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
        {
            free(env[i]);
            env[i] = new_entry;
            return (1);
        }
        i++;
    }
    return (0);
}

static int add_new_variable(t_shell *shell, char *new_entry)
{
    int count = 0;
    char **new_env;
    int i;
    
    if (shell->env)
    {
        while (shell->env[count])
            count++;
    }
    
    new_env = malloc((count + 2) * sizeof(char *));
    if (!new_env)
    {
        free(new_entry);
        return (0);
    }
    
    i = 0;
    while (i < count)
    {
        new_env[i] = shell->env[i];
        i++;
    }
    
    new_env[count] = new_entry;
    new_env[count + 1] = NULL;
    
    if (shell->env)
        free(shell->env);
    
    shell->env = new_env;
    
    return (1);
}

void ft_export(char *rl, t_shell *shell)
{
    char *var;
    char *value;
    char *new_entry;
    
    var = extract_variable_value(rl, &value);
    if (!var)
        return;
    
    new_entry = create_env(var, value);
    if (!new_entry)
    {
        free(var);
        return;
    }
    
    if (!replace_variable(shell->env, var, new_entry))
    {
        if (!add_new_variable(shell, new_entry))
        {
            free(var);
            return;
        }
    }
    
    free(var);
}