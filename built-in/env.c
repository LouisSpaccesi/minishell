/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:46 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/25 16:30:06 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_env(int argc, char **argv, char **envp)
{
    int i;
    (void)argv;
    (void)argc;

    i = 0;
    while(envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return ;
}