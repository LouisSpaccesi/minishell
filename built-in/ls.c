/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:00:46 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/25 16:30:06 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int ft_ls(int argc, char **argv, char **envp)
{
    char **args; 

    args = malloc(2 * sizeof(char *));
    if (!args)
        return (1);
    args[0] = "/bin/ls";
    args[1] = NULL;
    execve(args[0], args, envp); 
    perror("execve"); 
    return (EXIT_FAILURE);
}