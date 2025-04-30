/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:19:08 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/29 17:22:22 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(char *rl)
{
	char	*path;
	char	*args;
	int		i;

	path = NULL;
	args = rl + 2;
	i = 0;
	while (args[i] && args[i] == ' ')
		i++;
	if (args[i] == '\0')
	{
		path = getenv("HOME");
		if (!path)
			path = "/home";
	}
	else
		path = args + 1;
	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	return ;
}
