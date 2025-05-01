/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:31:59 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/30 18:34:04 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_simple_builtins(char **args, t_shell *shell, int saved_stdout)
{
    if (ft_strncmp(args[0], "env", 4) == 0)
    {
        shell->saved_stdout = saved_stdout; 
        ft_env(shell);
    }
    else if (ft_strncmp(args[0], "pwd", 4) == 0)
        ft_pwd(); 
    else if (ft_strncmp(args[0], "exit", 5) == 0)
    {
        if (saved_stdout != -1)
            ft_restore_output(saved_stdout);
        ft_exit(args, shell); 
    }
}

void	handle_echo_cd(char **args, char **envp)
{
	int		argc;
	char	cmd[1024]; 

	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		argc = 0;
		while (args[argc])
			argc++;
		ft_echo(argc, args, envp);
	}
	else if (ft_strncmp(args[0], "cd", 3) == 0)
	{
		ft_strcpy(cmd, "cd ");
		if (args[1])
			ft_strcat(cmd, args[1]);
		ft_cd(cmd);
	}
}

void	handle_env_builtins(char **args, t_shell *shell)
{
	char	cmd[1024]; 
	char	*arg_ptr = NULL;

	if (args[1])
		arg_ptr = args[1];

	if (ft_strncmp(args[0], "export", 7) == 0)
	{
		ft_strcpy(cmd, "export ");
		if (arg_ptr)
			ft_strcat(cmd, arg_ptr);
		else
			cmd[ft_strlen("export ")] = '\0';
		
		ft_export(cmd, shell); 
	}
	else if (ft_strncmp(args[0], "unset", 6) == 0)
	{
		ft_strcpy(cmd, "unset ");
		if (arg_ptr)
			ft_strcat(cmd, arg_ptr);
		else
			cmd[ft_strlen("unset ")] = '\0';
		
		ft_unset_command(cmd, shell); 
	}
}

void	handle_complex_builtins(char **args, t_shell *shell)
{
	if (ft_strncmp(args[0], "echo", 5) == 0 || ft_strncmp(args[0], "cd",
			3) == 0)
		handle_echo_cd(args, shell->env); 
	else
		handle_env_builtins(args, shell); 
}

static int handle_command_execution(t_command *cmd, char **args, t_shell *shell, int saved_stdout)
{
    if (cmd->is_builtin)
    {
        if (ft_strncmp(args[0], "env", 4) == 0 || ft_strncmp(args[0], "pwd", 
                4) == 0 || ft_strncmp(args[0], "exit", 5) == 0)
            handle_simple_builtins(args, shell, saved_stdout); 
        else
            handle_complex_builtins(args, shell); 
    }
    else
        parse_args(args, shell->env); 
    
    if (saved_stdout != -1)
        ft_restore_output(saved_stdout);
    return (0);
}

int execute_command_with_redirection(char **args, t_shell *shell)
{
    t_command cmd;
    int saved_stdout;
    int i = 0;
    
    cmd.args = args;
    cmd.is_builtin = is_builtin(args[0]);
    cmd.redirection = 0;
    cmd.redir_file = NULL;
    saved_stdout = -1;
    
    while (args[i])
        i++;
    
    if (handle_redirection(args, &saved_stdout) != 0)
        return (1);
    
    return (handle_command_execution(&cmd, args, shell, saved_stdout));
}
