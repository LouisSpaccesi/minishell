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

void ft_unset(char *args, char ***env)
{
    int i = 0;
    int var_len = ft_strlen(args);

    while ((*env)[i])
    {
        if (ft_strncmp((*env)[i], args, var_len) == 0 && (*env)[i][var_len] == '=')
        {
            free((*env)[i]); 
            while ((*env)[i + 1]) 
            {
                (*env)[i] = (*env)[i + 1];
                i++;
            }
            (*env)[i] = NULL;
            return; 
        }
        i++;
    }
}


void ft_unset_command(char *rl, char ***env)
{
    char *args;
	int i;

	i = 0;
	args = rl + 6;
    while (args[i] == ' ') 
        i++;
    if (args[i])
        ft_unset(args, env);
}
