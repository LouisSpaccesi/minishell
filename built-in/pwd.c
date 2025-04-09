/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:49 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/09 13:59:25 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

void	ft_pwd(void)
{
	char	pwd[1024];

	getcwd(pwd, sizeof(pwd));
	printf("%s\n", pwd);
}

void	ft_pwd_no_nl(void)
{
	char	pwd[1024];

	getcwd(pwd, sizeof(pwd));
	printf("%s", pwd);
}

// int main()
// {
//     ft_pwd();
// }