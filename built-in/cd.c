/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:19:08 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/25 16:54:11 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && i < n)
	{
		i++;
	}
	if (n == i)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void ft_cd(int argc, char **argv, char **envp)
{
	(void)envp;
	char buffer[256];
	char *path;

	if (argc == 2 && ft_strncmp(argv[1], "cd", 2) == 0)
	{
		path = getenv("HOME");
		if (!path)
			path = "/home";
	}
	else if (argc == 3 && ft_strncmp(argv[1], "cd", 2) == 0)
		path = argv[2];
	if (chdir(path) != 0)
		return ;
	if (getcwd(buffer, sizeof(buffer)) == NULL)
		return ;
	printf("pwd = %s\n", buffer);
	return ;
}

int main(int argc, char **argv, char **envp)
{
	ft_cd(argc, argv, envp);
}