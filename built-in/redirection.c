/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:41:24 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/04 16:17:44 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_restore_output(int saved_stdout)
{
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}

int ft_redirect_output(char *filename)
{
    int file;
    int saved_stdout;

    file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file == -1)
        return (-1);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(file);
        return (-1);
    }
    if (dup2(file, STDOUT_FILENO) == -1)
    {
        close(file);
        close(saved_stdout);
        return (-1);
    }
    close(file); 
    return (saved_stdout); 
}

int ft_redirect_output_append(char *filename)
{
    int file;
    int saved_stdout;

    file = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (file == -1)
        return (-1);  
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(file);
        return (-1);
    }
    if (dup2(file, STDOUT_FILENO) == -1)
    {
        close(file);
        close(saved_stdout);
        return (-1);
    }
    close(file); 
    return (saved_stdout); 
}

int find_last_redirection(char **args, int *redirection_type)
{
    int i;
    int last_redir;
    
    i = 0;
    last_redir = -1;
    *redirection_type = 0; 
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0)
        {
            last_redir = i;
            *redirection_type = 1;
        }
        else if (ft_strcmp(args[i], ">>") == 0)
        {
            last_redir = i;
            *redirection_type = 2;
        }
        i++;
    }
    return (last_redir);
}

int handle_redirection(char **args, int *saved_stdout)
{
    int redir_idx;
    int redir_type;
    
    redir_idx = find_last_redirection(args, &redir_type);
    if (redir_idx >= 0)
    {
        if (!args[redir_idx + 1])
            return (1);
            
        if (redir_type == 1) 
            *saved_stdout = ft_redirect_output(args[redir_idx + 1]);
        else if (redir_type == 2) 
            *saved_stdout = ft_redirect_output_append(args[redir_idx + 1]);
        if (*saved_stdout == -1)
            return (1);
        args[redir_idx] = NULL;
    }
    return (0);
}

