/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:21:42 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/26 16:04:34 by lospacce         ###   ########.fr       */
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

#endif