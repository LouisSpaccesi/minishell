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

void ft_restore_input(int saved_stdin)
{
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
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

int ft_redirect_input_heredoc(char *delimiter)
{
    int pipefd[2];
    int saved_stdin;
    char *line;
    
    if (pipe(pipefd) == -1)
        return (-1);
    
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdin);
        return (-1);
    }
    
    if (pid == 0)
    {
        // Processus enfant qui gère la lecture du heredoc
        close(pipefd[0]); // Fermer le côté lecture du pipe
        
        // Afficher le prompt avant la première lecture
        printf("heredoc> ");
        fflush(stdout);
        line = readline("");
        while (line)
        {
            if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
            {
                free(line);
                break;
            }
            write(pipefd[1], line, ft_strlen(line));
            write(pipefd[1], "\n", 1);
            free(line);
            printf("heredoc> ");
            fflush(stdout);
            line = readline("");
        }
        
        close(pipefd[1]);
        exit(0); 
    }
    else
    {
        close(pipefd[1]); 
        int status;
        waitpid(pid, &status, 0);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            close(pipefd[0]);
            close(saved_stdin);
            return (-1);
        }
        
        close(pipefd[0]);        
        return (saved_stdin);
    }
    
    // Ne devrait jamais atteindre ce point, mais pour éviter les avertissements du compilateur
    return (-1);
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
        if (ft_strncmp(args[i], ">", 2) == 0)
        {
            last_redir = i;
            *redirection_type = 1;
        }
        else if (ft_strncmp(args[i], ">>", 3) == 0)
        {
            last_redir = i;
            *redirection_type = 2;
        }
        else if (ft_strncmp(args[i], "<<", 3) == 0)
        {
            last_redir = i;
            *redirection_type = 3;
        }
        i++;
    }
    return (last_redir);
}

int handle_redirection(char **args, int *saved_stdout)
{
    int redir_idx;
    int redir_type;
    static int saved_stdin = -1;
    
    // Restaurer l'entrée standard si nécessaire (avant de traiter une nouvelle commande)
    if (saved_stdin != -1)
    {
        ft_restore_input(saved_stdin);
        saved_stdin = -1;
    }
    
    redir_idx = find_last_redirection(args, &redir_type);
    if (redir_idx >= 0)
    {
        if (!args[redir_idx + 1])
            return (1);
            
        if (redir_type == 1) 
            *saved_stdout = ft_redirect_output(args[redir_idx + 1]);
        else if (redir_type == 2) 
            *saved_stdout = ft_redirect_output_append(args[redir_idx + 1]);
        else if (redir_type == 3)
        {
            saved_stdin = ft_redirect_input_heredoc(args[redir_idx + 1]);
            if (saved_stdin == -1)
                return (1);
        }
            
        if ((redir_type == 1 || redir_type == 2) && *saved_stdout == -1)
            return (1);
            
        args[redir_idx] = NULL;
    }
    
    return (0);
}

static void read_heredoc_content(int temp_fd, char *delimiter)
{
    char *line;

    printf("heredoc> ");
    fflush(stdout);
    line = readline("");
    
    while (line && ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) != 0)
    {
        write(temp_fd, line, ft_strlen(line));
        write(temp_fd, "\n", 1);
        free(line);
        printf("heredoc> ");
        fflush(stdout);
        line = readline("");
    }
    
    if (line)
        free(line);
}

static char **create_command_array(char **args, int start, int end)
{
    char **cmd;
    int i;
    int j;
    
    cmd = malloc(sizeof(char *) * (end - start + 1));
    if (!cmd)
        return (NULL);
    
    i = start;
    j = 0;
    while (i < end)
    {
        cmd[j] = ft_strdup(args[i]);
        i++;
        j++;
    }
    cmd[j] = NULL;
    
    return (cmd);
}

static void execute_first_command(char **cmd1, int temp_fd, int *pipefd)
{
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    dup2(temp_fd, STDIN_FILENO);
    
    execvp(cmd1[0], cmd1);
    perror("execvp");
    exit(1);
}

static void execute_second_command(char **cmd2, int *pipefd, int temp_fd)
{
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(temp_fd);
    
    execvp(cmd2[0], cmd2);
    perror("execvp");
    exit(1);
}

static void free_commands(char **cmd1, char **cmd2, char *temp_file)
{
    int i;
    
    i = 0;
    while (cmd1[i])
    {
        free(cmd1[i]);
        i++;
    }
    free(cmd1);
    
    i = 0;
    while (cmd2[i])
    {
        free(cmd2[i]);
        i++;
    }
    free(cmd2);
    
    unlink(temp_file);
}

int execute_heredoc_pipe(char **args, char **env_copy)
{
    int heredoc_idx;
    int pipe_idx;
    char temp_file[32];
    int i;
    
    (void)env_copy;
    
    heredoc_idx = -1;
    pipe_idx = -1;
    i = 0;
    ft_strcpy(temp_file, "/tmp/minishell_heredoc_XXXXXX");
    
    // Trouver les positions du heredoc et du pipe
    while (args[i])
    {
        if (ft_strncmp(args[i], "<<", 3) == 0)
            heredoc_idx = i;
        else if (ft_strncmp(args[i], "|", 2) == 0)
            pipe_idx = i;
        i++;
    }
    
    return (execute_heredoc_pipe_part2(args, heredoc_idx, pipe_idx, temp_file));
}

int execute_heredoc_pipe_part2(char **args, int heredoc_idx, int pipe_idx, char *temp_file)
{
    int temp_fd;
    char **cmd1;
    char *delimiter;
    
    // Vérifier les positions
    if (heredoc_idx == -1 || pipe_idx == -1 || heredoc_idx > pipe_idx)
        return (1);
    
    // Créer le fichier temporaire
    temp_fd = mkstemp(temp_file);
    if (temp_fd == -1)
        return (1);
    
    // Lire le contenu du heredoc
    delimiter = args[heredoc_idx + 1];
    read_heredoc_content(temp_fd, delimiter);
    
    // Créer les tableaux de commandes
    lseek(temp_fd, 0, SEEK_SET);
    cmd1 = create_command_array(args, 0, heredoc_idx);
    if (!cmd1)
    {
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    return (execute_heredoc_pipe_part3(args, pipe_idx, temp_fd, cmd1, temp_file));
}

int execute_heredoc_pipe_part3(char **args, int pipe_idx, int temp_fd, char **cmd1, char *temp_file)
{
    char **cmd2;
    int pipefd[2];
    
    // Créer la deuxième commande
    cmd2 = create_command_array(args, pipe_idx + 1, count_args(args, pipe_idx + 1));
    if (!cmd2)
    {
        free_array(cmd1);
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    // Créer le pipe
    if (pipe(pipefd) == -1)
    {
        free_array(cmd1);
        free_array(cmd2);
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    return (execute_heredoc_pipe_part4(cmd1, cmd2, temp_fd, pipefd, temp_file));
}

int execute_heredoc_pipe_part4(char **cmd1, char **cmd2, int temp_fd, int *pipefd, char *temp_file)
{
    pid_t pid1;
    pid_t pid2;
    
    // Premier fork
    pid1 = fork();
    if (pid1 == -1)
    {
        close_and_free(pipefd, cmd1, cmd2, temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    if (pid1 == 0)
        execute_first_command(cmd1, temp_fd, pipefd);
    
    // Deuxième fork
    pid2 = fork();
    if (pid2 == -1)
    {
        close_and_free(pipefd, cmd1, cmd2, temp_fd);
        waitpid(pid1, NULL, 0);
        unlink(temp_file);
        return (1);
    }
    
    if (pid2 == 0)
        execute_second_command(cmd2, pipefd, temp_fd);
    
    // Parent
    return (execute_heredoc_pipe_part5(pid1, pid2, pipefd, cmd1, cmd2, temp_fd, temp_file));
}

int execute_heredoc_pipe_part5(pid_t pid1, pid_t pid2, int *pipefd, char **cmd1, char **cmd2, int temp_fd, char *temp_file)
{
    int status;
    
    // Fermer les descripteurs de fichiers dans le parent
    close(pipefd[0]);
    close(pipefd[1]);
    close(temp_fd);
    
    // Attendre les enfants
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    // Libérer la mémoire
    free_commands(cmd1, cmd2, temp_file);
    
    return (0);
}

void close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd)
{
    close(pipefd[0]);
    close(pipefd[1]);
    free_array(cmd1);
    free_array(cmd2);
    close(temp_fd);
}

int count_args(char **args, int start)
{
    int count;
    
    count = start;
    while (args[count])
        count++;
    
    return (count);
}

