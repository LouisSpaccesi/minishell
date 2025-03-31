/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/31 12:03:06 by lospacce         ###   ########.fr       */
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
    (void)argc;
    (void)argv;
	char *rl;
    char **env_copy;
	char **args;
    
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
		if (ft_strncmp(rl, "ls", 2) == 0 && (rl[2] == ' ' || rl[2] == '\0'))
		{
			args = ft_split(rl, ' ');
			ft_ls(0, args + 1, env_copy);
			free_array(args);
		}
		else if (ft_strncmp(rl, "grep", 4) == 0 && (rl[4] == ' ' || rl[4] == '\0'))
		{
			args = ft_split(rl, ' ');
			ft_grep(0, args + 1, env_copy);
			free_array(args);
		}
		else if (ft_strncmp(rl, "cat", 3) == 0 && (rl[3] == ' ' || rl[3] == '\0'))
		{
			args = ft_split(rl, ' ');
			ft_cat(0, args + 1, env_copy);
			free_array(args);
		}
		else if (ft_strncmp(rl, "touch", 5) == 0 && (rl[5] == ' ' || rl[5] == '\0'))
		{
			args = ft_split(rl, ' ');
			ft_touch(0, args + 1, env_copy);
			free_array(args);
		}
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
		else if (ft_strncmp(rl, "rm", 2) == 0 && (rl[2] == ' ' || rl[2] == '\0'))
		{
			args = ft_split(rl, ' ');
			ft_rm(0, args + 1, env_copy);
			free_array(args);
		}
		else if (ft_strncmp(rl, "mkdir", 5) == 0 && (rl[5] == ' ' || rl[5] == '\0'))
		{
		    args = ft_split(rl, ' ');
		    ft_mkdir(0, args + 1, env_copy);
		    free_array(args);
		}
		else if (ft_strncmp(rl, "rmdir", 5) == 0 && (rl[5] == ' ' || rl[5] == '\0'))
		{
		    args = ft_split(rl, ' ');
		    ft_rmdir(0, args + 1, env_copy);
		    free_array(args);
		}
		else if (ft_strncmp(rl, "wc", 2) == 0 && (rl[2] == ' ' || rl[2] == '\0'))
		{
			args = ft_split(rl, ' ');
			ft_wc(0, args + 1, env_copy);
			free_array(args);
		}
		free(rl);
	}
	free_env(env_copy);
	return (EXIT_SUCCESS);
}