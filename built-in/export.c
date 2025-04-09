/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:10:52 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 16:09:56 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *variable_start(char *rl)
{
    int i = 0;
    char *args; 
    args = rl + 6;

    while (args[i] == ' ')
        i++;
    if (args[i] == '\0')
        return NULL;
    return (&args[i]);
}

static char *extract_variable_value(char *rl, char **value_ptr)
{
    char *var_start;
    char *equals_pos;
    char *var;
    int var_len;

    var_start = variable_start(rl);
    if (!var_start)
        return NULL;
    equals_pos = ft_strchr(var_start, '=');
    if (!equals_pos)
        return NULL;
    var_len = equals_pos - var_start;
    var = malloc(var_len + 1);
    if (!var)
        return NULL;
    ft_strlcpy(var, var_start, var_len + 1);
    var[var_len] = '\0';
    *value_ptr = equals_pos + 1;
    return var;
}

static int replace_variable(char **env, char *var, char *new_entry)
{
    int i;
    int var_len;
    
    i = 0;
    var_len = ft_strlen(var);
    
    while (env[i])
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

static int add_new_variable(char ***env_ptr, char *var, char *new_entry)
{
    (void)var;
    int count;
    
    count = 0;
    while ((*env_ptr)[count])
        count++;
    *env_ptr = realloc(*env_ptr, (count + 2) * sizeof(char *));
    if (!*env_ptr)
    {
        free(new_entry);
        return (0);
    }
    (*env_ptr)[count] = new_entry;
    (*env_ptr)[count + 1] = NULL;
    
    return (1);
}

void ft_export(char *rl, char ***env_ptr)
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
    if (!replace_variable(*env_ptr, var, new_entry))
    {
        if (!add_new_variable(env_ptr, var, new_entry))
        {
            free(var);
            return;
        }
    }
    free(var);
}