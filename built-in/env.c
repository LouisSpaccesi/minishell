/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:46 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/26 15:30:41 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *create_env(const char *var, const char *value)
{
    char *new_entry;
    int i; 
    int j;
    
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
    return new_entry;
}

void copy_env(char **new_env, char **env, int count)
{
    int j;
    
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

    i = 0;
    while(envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return ;
}