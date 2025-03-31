/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:21:42 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/31 15:05:32 by lospacce         ###   ########.fr       */
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
int 	replace_var(char **env, char *var, char *new_entry);
void ft_unset_command(char *rl, char ***env);
int     exec_external_cmd(char *cmd_path, char **args, char **envp);
void    free_array(char **array);
int     ft_grep(int argc, char **argv, char **envp);
int     ft_cat(int argc, char **argv, char **envp);
int     ft_touch(int argc, char **argv, char **envp);
char    *get_path_env(char **envp);
char    *find_command_path(char *cmd, char **envp);
int	exec_command(char **argv, char **envp);
int     ft_rm(int argc, char **argv, char **envp);
int 	ft_mkdir(int argc, char **argv, char **envp);
int 	ft_rmdir(int argc, char **argv, char **envp);
int 	ft_wc(int argc, char **argv, char **envp);
int parse_args(char **args, char **envp);

#endif