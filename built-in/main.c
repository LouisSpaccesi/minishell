/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/24 18:56:00 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <unistd.h>

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

void ft_exit()
{
	exit(EXIT_SUCCESS);
}

void ft_env(int argc, char **argv, char **envp)
{
    int i;

    i = 0;
    while(envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return ;
}

void ft_pwd()
{
    char pwd[1024];
    getcwd(pwd, sizeof(pwd));
    printf("%s\n", pwd);
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

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	char *rl;
	char *path;
	int i;

	i = 0;
	while (1)
	{
		rl = readline("minishell > ");
		add_history(rl);
		if (ft_strncmp(rl, "exit", 4) == 0)
			ft_exit();
		if (ft_strncmp(rl, "env", 3) == 0)
			ft_env(argc, argv, envp);
		if (ft_strncmp(rl, "pwd", 4) == 0)
			ft_pwd();
		if (ft_strncmp(rl, "echo", 4) == 0)
		{
			if (strstr(rl, "$PWD") != 0)
				ft_pwd();
			else if (strlen(rl) > 5)
				printf("%s\n", rl + 5);
			else
				printf("\n");
		}
		free(rl);
	}
}