/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:30:49 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 19:51:55 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dispatch_builtin(char **args, t_shell *shell)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (ft_echo(args, shell));
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (ft_cd(args, shell));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (ft_export(args, shell));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (ft_unset_command(args, shell));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (ft_env_builtin(shell));
	if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		ft_exit(args, shell);
		return (shell->exit_status);
	}
	return (1);
}
