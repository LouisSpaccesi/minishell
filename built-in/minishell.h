/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce <lospacce@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:21:42 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/22 15:26:31 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_command
{
	char	**args;
	int		redirection;
	char	*redir_file;
	int		is_builtin;
	int		pipe_in;
	int		pipe_out;
	int		is_piped;
}			t_command;

typedef struct s_shell
{
	char	**env;
	int		exit_status;
	int		saved_stdout;
	int		saved_stdin;
	int		original_stdin;
}			t_shell;

typedef struct s_hd_pipe
{
	char	**cmd1;
	char	**cmd2;
	int		temp_fd;
	int		pipefd[2];
	char	temp_file[32];
	pid_t	pid1;
	pid_t	pid2;
}			t_hd_pipe;

void	close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd);
int	count_args(char **args, int start);
void	free_commands(char **cmd1, char **cmd2, char *temp_file);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		ft_exit(void);
void		ft_env(char **envp);
int			ft_ls(int argc, char **argv, char **envp);
void		ft_pwd(void);
void		ft_pwd_no_nl(void);
void		ft_cd(char *rl);
int			ft_echo(int argc, char **argv);
void		ft_exit(void);
void		ft_export(char *rl, char ***env_ptr);
char		*create_env(const char *var, const char *value);
void		copy_env(char **new_env, char **env, int count);
void		ft_unset_command(char *rl, char ***env);
void		free_array(char **array);
char		*get_path_env(char **envp);
char		*find_command_path(char *cmd, char **envp);
int			exec_command(char **argv, char **envp);
int			parse_args(char **args, char **envp);
int			ft_redirect_output(char *filename);
int			ft_redirect_output_append(char *filename);
int			ft_redirect_input_heredoc(char *delimiter);
void		ft_restore_output(int saved_stdout);
void		ft_restore_input(int saved_stdin);
int			execute_command_with_redirection(char **args, char **env_copy);
int			ft_strcmp(const char *s1, const char *s2);
int			is_builtin(char *cmd);
void		handle_complex_builtins(char **args, char **env_copy);
void		handle_env_builtins(char **args, char ***env_copy);
void		handle_echo_cd(char **args);
int			handle_redirection(char **args, int *saved_stdout);

/* Fonctions spécifiques pour les cas complexes */
int			execute_heredoc_pipe(char **args, char **env_copy);

/* Fonctions de gestion des pipes */
int			count_pipes(char **args);
char		***split_command_by_pipes(char **args);
int			execute_simple_command(char **args, char **env_copy);
int			execute_piped_commands(char ***cmd_segments, char **env_copy);
int			execute_piped_commands_part1(char ***cmd_segments, char **env_copy);
int			execute_piped_commands_part2(char ***cmd_segments, char **env_copy,
				int pipe_count);
int			wait_for_children(int pipe_count);
int			execute_command(char **args, char **env_copy);
int			execute_command_part1(char **args, char **env_copy);
int			execute_command_part2(char **args, char **env_copy);

/* Fonctions utilitaires */
int			count_args(char **args, int start);
void		close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd);
void		read_heredoc_content(int temp_fd, char *delimiter);
void		free_commands(char **cmd1, char **cmd2, char *temp_file);
void		execute_second_command(char **cmd2, int *pipefd, int temp_fd);
void		execute_first_command(char **cmd1, int temp_fd, int *pipefd);
char		**create_command_array(char **args, int start, int end);
int			execute_piped_commands_setup(char ***cmd_segments, char **env_copy,
				int pipe_count, pid_t *pids);
void free_command_segments(char ***cmd_segments, int count);

char	**create_command_array(char **args, int start, int end);
void	execute_first_command(char **cmd1, int temp_fd, int *pipefd);
void	execute_second_command(char **cmd2, int *pipefd, int temp_fd);
int	count_pipes(char **args);
int	wait_for_children(int pipe_count);
int	execute_command_part1(char **args, char **env_copy);
int	execute_command_part2(char **args, char **env_copy);
int	execute_pipe_without_heredoc(char **args, char **env_copy);
int	check_for_heredoc_pipe(char **args, int *has_heredoc, int *has_pipe);
int	execute_piped_commands_part1(char ***cmd_segments, char **env_copy);
int	init_command_pipes(int i, int pipe_count, int pipe_fds[2][2],
	int current_pipe);
int	execute_piped_commands_setup(char ***cmd_segments, char **env_copy,
		int pipe_count, pid_t *pids);
int	count_segment_size(char **args, int i);
char	**create_segment(char **args, int *index, int seg_size);
int	wait_for_children(int pipe_count);
void	handle_parent_pipes(int i, int pipe_count, int pipe_fds[2][2],
	int *current_pipe);
void	setup_child_pipes(int i, int pipe_count, int pipe_fds[2][2],
		int current_pipe);


#endif