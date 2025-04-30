/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:03:04 by louis             #+#    #+#             */
/*   Updated: 2025/04/29 17:04:01 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_echo_arg(char *arg, char **envp)
{
	int			len;
	char		temp[1024];

	len = ft_strlen(arg);
	if (len >= 2 && arg[0] == '\'' && arg[len - 1] == '\'')
	{
		ft_strcpy(temp, arg + 1);
		temp[len - 2] = '\0';
		printf("%s", temp);
	}
	else if (len >= 2 && arg[0] == '"' && arg[len - 1] == '"')
	{
		ft_strcpy(temp, arg + 1);
		temp[len - 2] = '\0';
		if (temp[0] == '$')
			print_env_var(temp + 1, envp);
		else
			printf("%s", temp);
	}
	else if (arg[0] == '$')
		print_env_var(arg + 1, envp);
	else
		printf("%s", arg);
}
