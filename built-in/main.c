/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/25 16:53:59 by lospacce         ###   ########.fr       */
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

int ft_ls(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    int status;
    char **args = malloc(3 * sizeof(char *));
    
    args[0] = "/usr/bin/ls";
    args[1] = "-la";
    args[2] = NULL;
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(args);
        return EXIT_FAILURE;
    }
    if (pid == 0)
    {
        if (execve(args[0], args, envp) == -1)
        {
            perror("execve");
            free(args);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        free(args);
    }
    return EXIT_SUCCESS;
}


int	main(int argc, char **argv, char **envp)
{
	char *rl;

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
		if (ft_strncmp(rl, "ls", 2) == 0)
			ft_ls(argc, argv, envp);
		if (ft_strncmp(rl, "echo", 4) == 0)
		{
			if (strstr(rl, "$PWD") != 0)
				ft_pwd();
			else if (strlen(rl) > 5)
				printf("%s\n", rl + 5);
			else
				printf("\n");
		}
		if (ft_strncmp(rl, "cd", 2) == 0)
			ft_cd(argc, argv, envp);
		free(rl);
	}
}