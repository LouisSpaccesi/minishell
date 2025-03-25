/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:19:08 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/24 17:11:58 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int main(int argc, char **argv)
{
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
		return (0);
	if (getcwd(buffer, sizeof(buffer)) == NULL)
		return (0);
	printf("pwd = %s\n", buffer);
	return 0;
}
