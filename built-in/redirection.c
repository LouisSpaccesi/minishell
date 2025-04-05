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
        
        // Boucle tant que la ligne n'est pas NULL et différente du délimiteur
        while (line)
        {
            // Si la ligne correspond au délimiteur, sortir de la boucle
            if (ft_strcmp(line, delimiter) == 0)
            {
                free(line);
                break;
            }
            
            write(pipefd[1], line, ft_strlen(line));
            write(pipefd[1], "\n", 1);
            free(line);
            
            // Afficher le prompt avant chaque nouvelle lecture
            printf("heredoc> ");
            fflush(stdout);
            line = readline("");
        }
        
        close(pipefd[1]);
        exit(0); // Terminer le processus enfant
    }
    else
    {
        // Processus parent
        close(pipefd[1]); // Fermer le côté écriture du pipe
        
        // Attendre que l'enfant termine (lecture du heredoc complète)
        int status;
        waitpid(pid, &status, 0);
        
        // Connecter l'entrée standard au côté lecture du pipe
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
        else if (ft_strcmp(args[i], "<<") == 0)
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

int execute_heredoc_pipe(char **args, char **env_copy)
{
    int heredoc_idx = -1;
    int pipe_idx = -1;
    int i = 0;
    int status = 0;
    char temp_file[] = "/tmp/minishell_heredoc_XXXXXX";
    int temp_fd;
    
    (void)env_copy;  // Éviter l'avertissement de paramètre non utilisé
    
    // Créer un fichier temporaire
    temp_fd = mkstemp(temp_file);
    if (temp_fd == -1)
        return (1);
    
    // Trouver les positions du heredoc et du pipe
    while (args[i])
    {
        if (ft_strcmp(args[i], "<<") == 0)
            heredoc_idx = i;
        else if (ft_strcmp(args[i], "|") == 0)
            pipe_idx = i;
        i++;
    }
    
    // Si l'un des deux n'est pas trouvé, ou si leur ordre est incorrect
    if (heredoc_idx == -1 || pipe_idx == -1 || heredoc_idx > pipe_idx)
    {
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    // Nous avons trouvé un heredoc, lire son contenu depuis le terminal
    char *delimiter = args[heredoc_idx + 1];
    char *line;
    
    // Lire le heredoc et l'écrire dans le fichier temporaire
    printf("heredoc> ");
    fflush(stdout);
    line = readline("");
    
    while (line && ft_strcmp(line, delimiter) != 0)
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
    
    // Rembobiner le fichier pour la lecture
    lseek(temp_fd, 0, SEEK_SET);
    
    // Créer la première partie de la commande (jusqu'au heredoc)
    char **cmd1 = malloc(sizeof(char *) * (heredoc_idx + 1));
    if (!cmd1)
    {
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    // Copier la première partie de la commande
    for (i = 0; i < heredoc_idx; i++)
        cmd1[i] = ft_strdup(args[i]);
    cmd1[i] = NULL;
    
    // Créer la deuxième partie de la commande (après le pipe)
    int arg_count = 0;
    i = pipe_idx + 1;
    while (args[i])
    {
        i++;
        arg_count++;
    }
    
    char **cmd2 = malloc(sizeof(char *) * (arg_count + 1));
    if (!cmd2)
    {
        i = 0;
        while (cmd1[i])
            free(cmd1[i++]);
        free(cmd1);
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    i = pipe_idx + 1;
    int j = 0;
    while (args[i])
        cmd2[j++] = ft_strdup(args[i++]);
    cmd2[j] = NULL;
    
    // Exécuter la commande avec un pipe standard
    int pipefd[2];
    pid_t pid1, pid2;
    
    if (pipe(pipefd) == -1)
    {
        i = 0;
        while (cmd1[i])
            free(cmd1[i++]);
        free(cmd1);
        i = 0;
        while (cmd2[i])
            free(cmd2[i++]);
        free(cmd2);
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    pid1 = fork();
    if (pid1 == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        // Nettoyer la mémoire
        i = 0;
        while (cmd1[i])
            free(cmd1[i++]);
        free(cmd1);
        i = 0;
        while (cmd2[i])
            free(cmd2[i++]);
        free(cmd2);
        close(temp_fd);
        unlink(temp_file);
        return (1);
    }
    
    if (pid1 == 0)
    {
        // Premier enfant - exécute la première commande (cat)
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        dup2(temp_fd, STDIN_FILENO);  // Lire depuis le fichier temporaire
        
        execvp(cmd1[0], cmd1);
        perror("execvp"); 
        exit(1);
    }
    
    pid2 = fork();
    if (pid2 == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        // Nettoyer la mémoire
        i = 0;
        while (cmd1[i])
            free(cmd1[i++]);
        free(cmd1);
        i = 0;
        while (cmd2[i])
            free(cmd2[i++]);
        free(cmd2);
        close(temp_fd);
        waitpid(pid1, NULL, 0);
        unlink(temp_file);
        return (1);
    }
    
    if (pid2 == 0)
    {
        // Deuxième enfant - exécute la seconde commande
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(temp_fd);
        
        execvp(cmd2[0], cmd2);
        perror("execvp");
        exit(1);
    }
    
    // Parent
    close(pipefd[0]);
    close(pipefd[1]);
    close(temp_fd);
    
    // Attendre les enfants
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    // Nettoyer la mémoire
    i = 0;
    while (cmd1[i])
        free(cmd1[i++]);
    free(cmd1);
    i = 0;
    while (cmd2[i])
        free(cmd2[i++]);
    free(cmd2);
    
    // Supprimer le fichier temporaire
    unlink(temp_file);
    
    return (0);
}

