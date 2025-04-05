/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:45:24 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/05 13:45:24 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Compte le nombre de pipes dans une commande */
int count_pipes(char **args)
{
    int i;
    int count;
    
    i = 0;
    count = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], "|") == 0)
            count++;
        i++;
    }
    return (count);
}

/* Sépare la commande en plusieurs segments basés sur les pipes */
char ***split_command_by_pipes(char **args)
{
    int i;
    int j;
    int k;
    int pipe_count;
    char ***cmd_segments;
    
    pipe_count = count_pipes(args);
    cmd_segments = (char ***)malloc(sizeof(char **) * (pipe_count + 2));
    if (!cmd_segments)
        return (NULL);
    
    i = 0;
    j = 0;
    while (j <= pipe_count)
    {
        // Compter combien d'arguments il y a jusqu'au prochain pipe
        k = 0;
        while (args[i + k] && ft_strcmp(args[i + k], "|") != 0)
        {
            // Si on trouve un heredoc, il faut sauter le délimiteur aussi
            if (args[i + k] && ft_strcmp(args[i + k], "<<") == 0 && args[i + k + 1])
                k += 2;
            else
                k++;
        }
        
        // Allouer un tableau pour cette partie de la commande
        cmd_segments[j] = (char **)malloc(sizeof(char *) * (k + 1));
        if (!cmd_segments[j])
        {
            // Gérer l'erreur (nettoyer la mémoire)
            while (--j >= 0)
                free(cmd_segments[j]);
            free(cmd_segments);
            return (NULL);
        }
        
        // Copier les arguments dans le segment
        k = 0;
        while (args[i] && ft_strcmp(args[i], "|") != 0)
        {
            cmd_segments[j][k] = ft_strdup(args[i]);
            i++;
            k++;
        }
        cmd_segments[j][k] = NULL;
        
        // Passer le pipe
        if (args[i])
            i++;
        j++;
    }
    cmd_segments[j] = NULL;
    return (cmd_segments);
}

/* Libère la mémoire allouée pour les segments de commande */
void free_command_segments(char ***cmd_segments)
{
    int i;
    int j;
    
    i = 0;
    while (cmd_segments[i])
    {
        j = 0;
        while (cmd_segments[i][j])
        {
            free(cmd_segments[i][j]);
            j++;
        }
        free(cmd_segments[i]);
        i++;
    }
    free(cmd_segments);
}

/* Exécute une commande simple (sans pipe) */
int execute_simple_command(char **args, char **env_copy)
{
    return (execute_command_with_redirection(args, env_copy));
}

/* Exécute une commande avec pipes */
int execute_piped_commands(char ***cmd_segments, char **env_copy)
{
    int i;
    int pipe_count;
    int pipe_fds[2][2];
    pid_t pid;
    int status;
    int current_pipe;
    
    i = 0;
    while (cmd_segments[i])
        i++;
    pipe_count = i - 1;
    
    if (pipe_count == 0)
        return (execute_simple_command(cmd_segments[0], env_copy));
    
    i = 0;
    current_pipe = 0;
    while (i <= pipe_count)
    {
        if (i < pipe_count)
        {
            if (pipe(pipe_fds[current_pipe]) == -1)
                return (1);
        }
        
        pid = fork();
        if (pid == -1)
            return (1);
            
        if (pid == 0)
        {
            // Processus enfant
            if (i > 0)
            {
                // Connecter l'entrée de cette commande à la sortie de la commande précédente
                dup2(pipe_fds[1 - current_pipe][0], STDIN_FILENO);
            }
            
            if (i < pipe_count)
            {
                // Connecter la sortie de cette commande à l'entrée de la commande suivante
                dup2(pipe_fds[current_pipe][1], STDOUT_FILENO);
            }
            
            // Fermer tous les descripteurs de pipe inutiles
            if (i > 0)
                close(pipe_fds[1 - current_pipe][0]);
            if (i < pipe_count)
            {
                close(pipe_fds[current_pipe][0]);
                close(pipe_fds[current_pipe][1]);
            }
            execute_simple_command(cmd_segments[i], env_copy);
            exit(0);
        }
        else
        {
            if (i > 0)
                close(pipe_fds[1 - current_pipe][0]);
            if (i < pipe_count)
                close(pipe_fds[current_pipe][1]);
            
            current_pipe = 1 - current_pipe;  // Basculer entre 0 et 1
        }
        
        i++;
    }
    i = 0;
    while (i <= pipe_count)
    {
        waitpid(-1, &status, 0);
        i++;
    }
    
    return (0);
}

/* Point d'entrée principal pour l'exécution des commandes avec ou sans pipes */
int execute_command(char **args, char **env_copy)
{
    int has_heredoc;
    int has_pipe;
    int i;
    int pipe_count;
    char ***cmd_segments;
    int status;
    
    // Vérifier si la commande contient un heredoc
    has_heredoc = 0;
    has_pipe = 0;
    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], "<<") == 0)
            has_heredoc = 1;
        else if (ft_strcmp(args[i], "|") == 0)
            has_pipe = 1;
        i++;
    }
    
    // Si la commande contient à la fois un heredoc et un pipe, utiliser la fonction spécialisée
    if (has_heredoc && has_pipe)
    {
        return (execute_heredoc_pipe(args, env_copy));
    }
    
    // Si la commande contient seulement un heredoc, utiliser la redirection standard
    if (has_heredoc)
    {
        return (execute_command_with_redirection(args, env_copy));
    }
    
    pipe_count = count_pipes(args);
    if (pipe_count == 0)
        return (execute_simple_command(args, env_copy));
    
    cmd_segments = split_command_by_pipes(args);
    if (!cmd_segments)
        return (1);
    
    status = execute_piped_commands(cmd_segments, env_copy);
    free_command_segments(cmd_segments);
    
    return (status);
} 