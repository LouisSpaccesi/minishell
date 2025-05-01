#include "minishell.h"

static int setup_redirection(char **args, t_redir_info *redir_info, t_shell *shell)
{
    int parse_result;
    int saved_fd;

    parse_result = parse_redirection(args, redir_info);
    saved_fd = -1;
    if (parse_result == -2)
    {
        shell->exit_status = 2;
        return (-2);
    }
    if (parse_result == 0)
    {
        saved_fd = apply_redirection(redir_info);
        if (saved_fd == -1)
            shell->exit_status = 1;
    }
    return (saved_fd);
}

static int dispatch_builtin(char **args, t_shell *shell)
{
    if (ft_strncmp(args[0], "pwd", 4) == 0)
        return (ft_pwd());
    else if (ft_strncmp(args[0], "env", 4) == 0)
        return (ft_env_builtin(shell));
    else if (ft_strncmp(args[0], "echo", 5) == 0)
        return (ft_echo(args, shell));
    else if (ft_strncmp(args[0], "cd", 3) == 0)
        return (ft_cd(args, shell));
    else if (ft_strncmp(args[0], "export", 7) == 0)
        return (ft_export(args, shell));
    else if (ft_strncmp(args[0], "unset", 6) == 0)
        return (ft_unset_command(args, shell));
    else if (ft_strncmp(args[0], "exit", 5) == 0)
    {
        ft_exit(args, shell);
        return (shell->exit_status);
    }
    return (1); 
}

static void restore_redirection(t_redir_info redir_info, int saved_fd, t_shell *shell)
{
    if (saved_fd != -1)
    {
        if (ft_restore_fd(redir_info.original_fd, saved_fd) == -1)
            shell->exit_status = 1;
    }
}

int execute_command_with_redirection(char **args, t_shell *shell)
{
    int status;
    t_redir_info redir_info;
    int saved_fd;

    redir_info.redir_type = 0; 
    saved_fd = setup_redirection(args, &redir_info, shell);
    if (saved_fd == -2)
        return (shell->exit_status);
    if (is_builtin(args[0]))
    {
        if (saved_fd == -1 && redir_info.redir_type != 0)
            status = 1;
        else
        {
            if (ft_strncmp(args[0], "exit", 5) == 0)
                restore_redirection(redir_info, saved_fd, shell);
            status = dispatch_builtin(args, shell);
            if (ft_strncmp(args[0], "exit", 5) != 0)
                restore_redirection(redir_info, saved_fd, shell);
        }
        shell->exit_status = status;
    }
    else
    {
        status = execute_external_command(args, shell, &redir_info);
        shell->exit_status = status;
    }
    return (shell->exit_status);
}
