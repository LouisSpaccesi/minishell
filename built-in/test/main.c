/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:27:26 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/31 15:16:28 by lospacce         ###   ########.fr       */
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
    {
        if (ft_strncmp(args[0], "env", 4) == 0)
            ft_env(env_copy);
        else if (ft_strncmp(args[0], "pwd", 4) == 0)
            ft_pwd();
        else if (ft_strncmp(args[0], "echo", 5) == 0)
        {
            if (args[1] && strstr(args[1], "$PWD") != 0)
                ft_pwd();
            else
            {
                char *echo_text = rl + 5;
                while (*echo_text && (*echo_text == ' ' || *echo_text == '\t'))
                    echo_text++;
                
                if (*echo_text)
                    printf("%s\n", echo_text);
                else
                    printf("\n");
            }
        }
        else if (ft_strncmp(args[0], "cd", 3) == 0)
            ft_cd(rl); 
        else if (ft_strncmp(args[0], "export", 7) == 0)
            ft_export(rl, &env_copy);
        else if (ft_strncmp(args[0], "unset", 6) == 0)
            ft_unset_command(rl, &env_copy); 
        else
            parse_args(args, env_copy);
    }
}
	free_env(env_copy);
	return (EXIT_SUCCESS);
}