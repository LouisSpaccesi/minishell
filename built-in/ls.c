/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:00:46 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/26 13:04:09 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_error(char *str, char **args)
{
    perror(str);
    free(args);
    return (EXIT_FAILURE);
}

static void child_process(char **args, char **envp)
{
    execve(args[0], args, envp);
    perror("execve");
    free(args);
    exit(EXIT_FAILURE);
}

int ft_ls(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    int status;
    char **args;
    pid_t pid;

    args = malloc(2 * sizeof(char *));
    if (!args)
        return (1);
    args[0] = "/usr/bin/ls";
    args[1] = NULL;
    
    pid = fork();
    if (pid == -1)
        return (handle_error("fork", args));
    if (pid == 0)
        child_process(args, envp);
    else
    {
        waitpid(pid, &status, 0);
        free(args);
    }
    return (EXIT_SUCCESS);
}