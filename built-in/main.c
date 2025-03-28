/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/26 16:06:16 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/26 14:46:07 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **copy_all_env(char **envp)
{
    int i;
    char **new_env;

	i = 0;
    while (envp[i])
        i++;
    new_env = malloc((i + 1) * sizeof(char *));
    if (!new_env)
        return (NULL);
    i = 0;
    while (envp[i]) 
    {
        new_env[i] = ft_strdup(envp[i]);
        if (!new_env[i])
        {
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            return (NULL);
        }
        i++;
    }
    new_env[i] = NULL; 
    return (new_env);
}

void free_env(char **env)
{
    int i = 0;
    
    if (!env)
        return; 
    while (env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}

int	main(int argc, char **argv, char **envp)
{
	char *rl;
    char **env_copy;
    
    env_copy = copy_all_env(envp);
	while (1)
	{
		rl = readline("minishell > ");
        if (!rl)
        {
            printf("exit\n");
            break;
        }
		add_history(rl);
		if (ft_strncmp(rl, "exit", 4) == 0)
		{
		    free(rl);
		    break;
		}
		if (ft_strncmp(rl, "env", 3) == 0)
			ft_env(env_copy);
		if (ft_strncmp(rl, "pwd", 4) == 0)
			ft_pwd();
		if (ft_strncmp(rl, "ls", 2) == 0)
			ft_ls(argc, argv, env_copy);
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
			ft_cd(rl);
		if (ft_strncmp(rl, "export", 6) == 0)
			ft_export(rl, &env_copy);
		if (ft_strncmp(rl, "unset", 5) == 0)
			ft_unset_command(rl, &env_copy);
		free(rl);
	}
	free_env(env_copy);
	return (EXIT_SUCCESS);
}