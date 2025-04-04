/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:04:38 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/04 16:10:50 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_echo_arg(char *arg)
{
    int len;
    char temp[1024];
    
    len = ft_strlen(arg);
    if (ft_strncmp(arg, "$PWD", 4) == 0)
        ft_pwd();
    else if (len >= 2 && arg[0] == '"' && arg[len - 1] == '"')
    {
        ft_strlcpy(temp, arg + 1, len);
        temp[len - 2] = '\0';
        printf("%s", temp);
    }
    else
        printf("%s", arg);
}

int ft_echo(int argc, char **argv)
{
    int i;
    
    i = 1;
    while(i < argc)
    {
        print_echo_arg(argv[i]);
        if(i + 1 < argc)
            printf(" ");
        i++;
    }
    printf("\n");
    return (0);
}
