/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:21:42 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/04 15:36:11 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include "../libft/libft.h"
# include <fcntl.h>

typedef struct s_command
{
    char    **args;          /* Tableau des arguments de la commande */
    int     redirection;     /* Type de redirection: 0 = aucune, 1 = >, 2 = >>, 3 = << */
    char    *redir_file;     /* Fichier de redirection */
    int     is_builtin;      /* Indique si la commande est une commande intégrée */
    int     pipe_in;         /* Descripteur de pipe d'entrée */
    int     pipe_out;        /* Descripteur de pipe de sortie */
    int     is_piped;        /* Indique si la commande fait partie d'un pipeline */
}   t_command;

typedef struct s_shell
{
    char    **env;           /* Copie de l'environnement */
    int     exit_status;     /* Code de sortie de la dernière commande */
    int     saved_stdout;    /* Sauvegarde du descripteur stdout pour les redirections */
    int     saved_stdin;     /* Sauvegarde du descripteur stdin pour les redirections */
    int     original_stdin;  /* Descripteur original de stdin pour restauration complète */
}   t_shell;


int     ft_strncmp(const char *s1, const char *s2, size_t n);
void    ft_exit(void);
void 	ft_env(char **envp);
int 	ft_ls(int argc, char **argv, char **envp);
void    ft_pwd(void);
void    ft_pwd_no_nl(void);
void 	ft_cd(char *rl);
int     ft_echo(int argc, char **argv);
void    ft_exit();
void 	ft_export(char *rl, char ***env_ptr);
char 	*create_env(const char *var, const char *value);
void 	copy_env(char **new_env, char **env, int count);
void ft_unset_command(char *rl, char ***env);
void    free_array(char **array);
char    *get_path_env(char **envp);
char    *find_command_path(char *cmd, char **envp);
int	exec_command(char **argv, char **envp);
int     parse_args(char **args, char **envp);
int     ft_redirect_output(char *filename);
int     ft_redirect_output_append(char *filename);
int     ft_redirect_input_heredoc(char *delimiter);
void    ft_restore_output(int saved_stdout);
void    ft_restore_input(int saved_stdin);
int     execute_command_with_redirection(char **args, char **env_copy);
int     ft_strcmp(const char *s1, const char *s2); 
int     is_builtin(char *cmd);
void    handle_complex_builtins(char **args, char **env_copy);
void    handle_env_builtins(char **args, char ***env_copy);
void    handle_echo_cd(char **args);
int     handle_redirection(char **args, int *saved_stdout);

/* Fonctions spécifiques pour les cas complexes */
int     execute_heredoc_pipe(char **args, char **env_copy);
int     execute_heredoc_pipe_part2(char **args, int heredoc_idx, int pipe_idx, char *temp_file);
int     execute_heredoc_pipe_part3(char **args, int pipe_idx, int temp_fd, char **cmd1, char *temp_file);
int     execute_heredoc_pipe_part4(char **cmd1, char **cmd2, int temp_fd, int *pipefd, char *temp_file);
int     execute_heredoc_pipe_part5(pid_t pid1, pid_t pid2, int *pipefd, char **cmd1, char **cmd2, int temp_fd, char *temp_file);

/* Fonctions de gestion des pipes */
int     count_pipes(char **args);
char    ***split_command_by_pipes(char **args);
void    free_command_segments(char ***cmd_segments);
int     execute_simple_command(char **args, char **env_copy);
int     execute_piped_commands(char ***cmd_segments, char **env_copy);
int     execute_piped_commands_part1(char ***cmd_segments, char **env_copy);
int     execute_piped_commands_part2(char ***cmd_segments, char **env_copy, int pipe_count);
int     wait_for_children(int pipe_count);
int     execute_command(char **args, char **env_copy);
int     execute_command_part1(char **args, char **env_copy);
int     execute_command_part2(char **args, char **env_copy);

/* Fonctions utilitaires */
int     count_args(char **args, int start);
void    close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd);





#endif