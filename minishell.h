# ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_shell
{
	char	**env;
	int		exit_status;
}	t_shell;

typedef struct s_redir_info
{
	int		redir_type;
	int		original_fd;
	char	*filename;
}	t_redir_info;

/* Builtin commands */
int		ft_echo(char **args, t_shell *shell);
int		ft_cd(char **args, t_shell *shell);
int		ft_pwd(void);
int		ft_export(char **args, t_shell *shell);
int		ft_unset_command(char **args, t_shell *shell);
int		ft_env_builtin(t_shell *shell);
void	ft_exit(char **args, t_shell *shell);

/* Redirection handling */
int		setup_redirection(char **args, t_redir_info *redir, t_shell *shell);
void	update_args(char **args, char **cmd);
int		apply_redir(char **args, t_redir_info *redir, t_shell *shell);
int		handle_heredoc_child(char **args, t_shell *shell);
int		parse_redirection(char **args, t_redir_info *redir);

/* Command execution */
int		execute_external_command(char **args, t_shell *shell, t_redir_info *redir);
int		is_builtin(char *cmd);
int		dispatch_builtin(char **args, t_shell *shell);
int		execute_command_with_redirection(char **args, t_shell *shell);

# endif 