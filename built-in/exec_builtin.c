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

int execute_command_with_redirection(char **args, t_shell *shell)
{
    int status = 0;
    t_redir_info redir_info;
    int parse_result;
    int saved_fd = -1; // Only needed for builtins now

    // Handle redirections (find last, apply it, remove args)
    parse_result = parse_redirection(args, &redir_info);

    if (parse_result == -2) // Syntax error during parsing
    {
        shell->exit_status = 2; // Standard error code for syntax error
        return (shell->exit_status);
    }
    // No redirection if parse_result == -1, proceed normally
    // If parse_result == 0, redir_info is populated

    // Check if it is a built-in command
    if (is_builtin(args[0]))
    {
        // Apply redirection if needed, BEFORE executing builtin
        if (parse_result == 0)
        {
            saved_fd = apply_redirection(&redir_info);
            if (saved_fd == -1)
            {
                shell->exit_status = 1; // Redirection failed
                return (shell->exit_status); // Don't execute builtin
            }
        }

        // Execute builtin
        if (ft_strncmp(args[0], "pwd", 4) == 0)
            status = ft_pwd();
        else if (ft_strncmp(args[0], "env", 4) == 0)
            status = ft_env_builtin(shell);
        else if (ft_strncmp(args[0], "echo", 5) == 0)
        {
            status = ft_echo(args, shell);
        }
        else if (ft_strncmp(args[0], "cd", 3) == 0)
            status = ft_cd(args, shell);
        else if (ft_strncmp(args[0], "export", 7) == 0)
            status = ft_export(args, shell);
        else if (ft_strncmp(args[0], "unset", 6) == 0)
            status = ft_unset_command(args, shell);
        else if (ft_strncmp(args[0], "exit", 5) == 0)
        {
             // ft_exit handles its own exit, but might return if too many args
             // Need to restore fd *before* calling ft_exit if it might terminate
             if (saved_fd >= 0)
                 ft_restore_fd(redir_info.original_fd, saved_fd);
             ft_exit(args, shell); // This might terminate the shell
             // If ft_exit returned (due to too many args), it set shell->exit_status.
             // We need to use that status.
             status = shell->exit_status;
        }
        shell->exit_status = status; 

        // Restore original fd if redirection was applied
        if (saved_fd != -1)
        {
            if (ft_restore_fd(redir_info.original_fd, saved_fd) == -1)
            {
                // Error during restore?
                shell->exit_status = 1;
            }
        }
    }
    else // External command
    {
        // Pass redir_info to external command execution
        // apply_redirection will be called inside the child process within execute_external_command
        status = execute_external_command(args, shell, &redir_info); // Need to update signature
        shell->exit_status = status;
    }

    return (shell->exit_status);
}
