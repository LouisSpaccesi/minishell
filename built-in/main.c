/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/04 15:33:01 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtin(char *cmd)
{
    return (ft_strncmp(cmd, "cd", 3) == 0 ||
            ft_strncmp(cmd, "echo", 5) == 0 ||
            ft_strncmp(cmd, "pwd", 4) == 0 ||
            ft_strncmp(cmd, "export", 7) == 0 ||
            ft_strncmp(cmd, "unset", 6) == 0 ||
            ft_strncmp(cmd, "env", 4) == 0 ||
            ft_strncmp(cmd, "exit", 5) == 0);
}
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
        args = ft_split(rl, ' ');
        if (args && args[0])
            execute_command_with_redirection(args, env_copy);
        free(rl);
        if (args)
        {
            int i = 0;
            while (args[i])
            {
                free(args[i]);
                i++;
            }
            free(args);
        }
    }
	free_env(env_copy);
	return (EXIT_SUCCESS);
}