/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:49 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/22 13:40:59 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

int	ft_pwd(void)
{
	char	pwd[1024];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
	{
		printf("%s\n", pwd);
		return (0); // Success
	}
	else
	{
		perror("minishell: pwd"); // Error message
		return (1); // Failure
	}
}

void	ft_pwd_no_nl(void)
{
	char	pwd[1024];

	getcwd(pwd, sizeof(pwd));
	printf("%s", pwd);
}
