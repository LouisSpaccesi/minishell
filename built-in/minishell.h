/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:21:42 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/30 18:30:05 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <errno.h>

# include "../libft/libft.h"

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

typedef struct s_env_var {
    char *name;
    char *value;
    struct s_env_var *next;
} 				t_env_var;

typedef struct s_shell
{
	char	**env;
	int		exit_status;
	int		saved_stdout;
	int		saved_stdin;
	int		original_stdin;
	t_env_var *custom_env;
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

typedef struct s_redir_info
{
	int		redir_type;
	char	*filename;
	int		original_fd;
} t_redir_info;

void		close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd);
int			count_args(char **args, int start);
void		free_commands(char **cmd1, char **cmd2, char *temp_file);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_env_builtin(t_shell *shell);
void		ft_pwd_no_nl(void);
int			ft_pwd(void);
int			ft_echo(char **args, t_shell *shell);
int			ft_cd(char **args, t_shell *shell);
int			ft_export(char **args, t_shell *shell);
int			ft_unset_command(char **args, t_shell *shell);
void		ft_exit(char **args, t_shell *shell);
void		copy_env(char **new_env, char **env, int count);
void		free_array(char **array);
char		*get_path_env(char **envp);
char		*find_command_path(char *cmd, char **envp);
int			exec_command(char **argv, char **envp);
int			parse_args(char **args, char **envp);
int			ft_redirect_input_heredoc(char *delimiter);
int			execute_command_with_redirection(char **args, t_shell *shell);
int			ft_strcmp(const char *s1, const char *s2);
int			is_builtin(char *cmd);
void		handle_complex_builtins(char **args, t_shell *shell);
void		handle_env_builtins(char **args, t_shell *shell);
void		handle_echo_cd(char **args, char **envp);
int			find_last_redirection(char **args, int *redirection_type);
int			execute_heredoc_pipe(char **args, t_shell *shell);
t_shell		*init_shell(char **envp);
void		free_env(char **env);
void		find_special_tokens(char **args, int *heredoc_idx, int *pipe_idx);
int			init_heredoc_pipe(t_hd_pipe *hp, char **args, int heredoc_idx,
				int pipe_idx);
int			setup_second_command(t_hd_pipe *hp, char **args, int pipe_idx);
int			count_pipes(char **args);
char		***split_command_by_pipes(char **args);
int			execute_simple_command(char **args, char **env_copy);
int			execute_piped_commands(char ***cmd_segments, char **env_copy);
int			execute_piped_commands_part1(char ***cmd_segments, t_shell *shell);
int			execute_piped_commands_part2(char ***cmd_segments, char **env_copy,
				int pipe_count);
int			wait_for_children(int pipe_count);
int			execute_command(char **args, char **env_copy);
int			execute_command_part1(char **args, t_shell *shell);
int			execute_command_part2(char **args, t_shell *shell);
void		free_command_segments(char ***cmd_segments, int count);
int			count_args(char **args, int start);
void		close_and_free(int *pipefd, char **cmd1, char **cmd2, int temp_fd);
void		read_heredoc_content(int temp_fd, char *delimiter);
void		free_commands(char **cmd1, char **cmd2, char *temp_file);
void		free_command_segments(char ***cmd_segments, int count);
char		**create_command_array(char **args, int start, int end);
void		execute_first_command(char **cmd1, int temp_fd, int *pipefd);
void		execute_second_command(char **cmd2, int *pipefd, int temp_fd);
int			count_pipes(char **args);
int			wait_for_children(int pipe_count);
int			execute_pipe_without_heredoc(char **args, t_shell *shell);
int			check_for_heredoc_pipe(char **args, int *has_heredoc,
				int *has_pipe);
int			init_command_pipes(int i, int pipe_count, int pipe_fds[2][2],
				int current_pipe);
int			execute_piped_commands_setup(char ***cmd_segments, t_shell *shell,
				int pipe_count, pid_t *pids);
int			count_segment_size(char **args, int i);
char		**create_segment(char **args, int *index, int seg_size);
int			wait_for_children(int pipe_count);
void		handle_parent_pipes(int i, int pipe_count, int pipe_fds[2][2],
				int *current_pipe);
void		setup_child_pipes(int i, int pipe_count, int pipe_fds[2][2],
				int current_pipe);
void		print_echo_arg(char *arg, char **envp);
void		print_env_var(char *var_name, char **envp);
void		read_heredoc_content(int temp_fd, char *delimiter);
void		ft_restore_output(int saved_stdout);
void		ft_restore_input(int saved_stdin);
int			ft_restore_fd(int original_fd, int saved_fd);
int			parse_redirection(char **args, t_redir_info *redir_info);
int			apply_redirection(t_redir_info *redir_info);
int 		ft_redirect_output(const char *filename);
int 		ft_redirect_output_append(const char *filename);
int 		ft_redirect_input(const char *filename);
int			ft_redirect_input_heredoc(char *delimiter);
char		*find_command_path(char *command, char **envp);
char		*build_full_path(char *path, char *cmd);
char		*search_in_paths(char **paths, char *cmd);
void		free_paths_array(char **paths);
void		handle_exec_failure(char *cmd_path, char **cmd_args);
void		exec_cmd_child(char *cmd_path, char **args, char **envp,
				int arg_count);
int			handle_command_not_found(char *cmd);
int			count_cmd_args(char **args);
void	handle_sigint(int sig);
void	setup_signals(void);
void	setup_exec_signals(void);
void	restore_terminal(void);
int	is_eof(char *line);
char	**tokenize_command_line(const char *line, t_shell *shell);
char	*get_env_value(char **envp, const char *var_name);
int		set_env_var(t_shell *shell, const char *varname, const char *value);
int		find_env_var_index(const char *var_name, char **envp);
char	*create_env_entry(const char *var, const char *value);
int		is_valid_identifier(const char *var);
char		**duplicate_env(char **envp);
int			execute_heredoc_pipe(char **args, t_shell *shell);
int execute_builtin(char **args, t_shell *shell);
int				execute_external_command(char **args, t_shell *shell, t_redir_info *redir_info);
char			*find_command_path(char *command, char **envp);
void			free_shell(t_shell *shell);

#endif