/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:46 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/21 15:04:17 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
    int i;

    i = 0;
    while(envp[i])
    {
        printf("\n%s", envp[i]);
        i++;
    }
    getchar();
    return 0;
}