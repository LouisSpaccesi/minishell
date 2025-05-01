/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:34:59 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:28:36 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_builtin_cmd(char **args, t_shell *shell)
{
	if (!ft_strncmp(args[0], "echo", 5))
		return (ft_echo(args, shell));
	if (!ft_strncmp(args[0], "cd", 3))
		return (ft_cd(args, shell));
	if (!ft_strncmp(args[0], "pwd", 4))
		return (ft_pwd());
	if (!ft_strncmp(args[0], "env", 4))
		return (ft_env_builtin(shell));
	return (-1);
}

static int	handle_builtin_env(char **args, t_shell *shell)
{
	if (!ft_strncmp(args[0], "export", 7))
		return (ft_export(args, shell));
	if (!ft_strncmp(args[0], "unset", 6))
		return (ft_unset_command(args, shell));
	if (!ft_strncmp(args[0], "exit", 5))
	{
		ft_exit(args, shell);
		return (shell->exit_status);
	}
	return (-1);
}

int	dispatch_builtin(char **args, t_shell *shell)
{
	int	ret;

	ret = handle_builtin_cmd(args, shell);
	if (ret != -1)
		return (ret);
	ret = handle_builtin_env(args, shell);
	if (ret != -1)
		return (ret);
	return (1);
}

int	execute_command_with_redirection(char **args, t_shell *shell)
{
	t_redir_info	redir;
	int				saved_fd;
	int				status;

	redir.redir_type = 0;
	redir.filename = NULL;
	redir.original_fd = -1;
	saved_fd = setup_redirection(args, &redir, shell);
	if (saved_fd < -1 || (saved_fd == -1 && shell->exit_status != 0
			&& redir.redir_type != 0))
		return (shell->exit_status);
	if (args[0])
	{
		if (is_builtin(args[0]))
			status = dispatch_builtin(args, shell);
		else
			status = execute_external_command(args, shell);
	}
	else
		status = 0;
	shell->exit_status = status;
	if (saved_fd != -1)
		ft_restore_fd(redir.original_fd, saved_fd);
	return (shell->exit_status);
}
