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

// typedef struct t_data
// {
//     int 
// }       s_data;


int     ft_strncmp(const char *s1, const char *s2, size_t n);
void    ft_exit(void);
void 	ft_env(char **envp);
int 	ft_ls(int argc, char **argv, char **envp);
void    ft_pwd(void);
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
void    ft_restore_output(int saved_stdout);
int     execute_command_with_redirection(char **args, char **env_copy);
int     ft_strcmp(const char *s1, const char *s2); 
int     is_builtin(char *cmd);
int execute_command_with_redirection(char **args, char **env_copy);
void handle_complex_builtins(char **args, char **env_copy);
void handle_env_builtins(char **args, char ***env_copy);
void handle_echo_cd(char **args);
int handle_redirection(char **args, int *saved_stdout);





#endif