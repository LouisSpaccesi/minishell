/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:21:42 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/25 16:31:27 by lospacce         ###   ########.fr       */
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

int     ft_strncmp(const char *s1, const char *s2, size_t n);
void    ft_exit(void);
void    ft_env(int argc, char **argv, char **envp);
int     ft_ls(int argc, char **argv, char **envp);
void    ft_pwd(void);
void    ft_cd(int argc, char **argv, char **envp);
int     ft_echo(int argc, char **argv);
void    ft_exit();

#endif