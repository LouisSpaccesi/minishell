/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:49 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/24 13:00:30 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

void ft_pwd()
{
    char pwd[1024];
    getcwd(pwd, sizeof(pwd));
    printf("%s\n", pwd);
}

// Version sans retour à la ligne pour l'utilisation avec pipes
void ft_pwd_no_nl()
{
    char pwd[1024];
    getcwd(pwd, sizeof(pwd));
    printf("%s", pwd);
}

// int main() 
// {
//     ft_pwd();    
// }