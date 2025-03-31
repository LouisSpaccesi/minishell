/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:40:00 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/31 11:57:25 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_grep_arg(char *arg, int *needs_free)
{
	int		len;
	char	*new_arg;

	*needs_free = 0;
	len = ft_strlen(arg);
	if (len >= 2 && arg[0] == '"' && arg[len - 1] == '"')
	{
		new_arg = malloc(len - 1);
		if (!new_arg)
			return (arg);
		ft_strlcpy(new_arg, arg + 1, len - 1);
		*needs_free = 1;
		return (new_arg);
	}
	return (arg);
}

static char	**prepare_grep_args(char *cmd_path, char **argv, int arg_count)
{
	char	**cmd_args;
	int		j;
	int		*needs_free;

	cmd_args = malloc((arg_count + 2) * sizeof(char *));
	if (!cmd_args)
		return (NULL);
	needs_free = malloc(arg_count * sizeof(int));
	if (!needs_free)
	{
		free(cmd_args);
		return (NULL);
	}
	cmd_args[0] = cmd_path;
	j = 0;
	while (j < arg_count)
	{
		cmd_args[j + 1] = process_grep_arg(argv[j], &needs_free[j]);
		j++;
	}
	cmd_args[j + 1] = NULL;
	cmd_args[0] = (char *)needs_free;
	return (cmd_args);
}
static void	free_grep_args(char **cmd_args, int arg_count)
{
	int		j;
	int		*needs_free;

	if (!cmd_args)
		return;
	needs_free = (int *)cmd_args[0];
	j = 0;
	while (j < arg_count)
	{
		if (needs_free[j])
			free(cmd_args[j + 1]);
		j++;
	}
	free(needs_free);
	free(cmd_args);
}

static void	exec_grep_child(char *cmd_path, char **argv, char **envp, int arg_count)
{
	char	**cmd_args;
	
	cmd_args = prepare_grep_args(cmd_path, argv, arg_count);
	if (!cmd_args)
	{
		perror("malloc");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	
	cmd_args[0] = cmd_path;
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	free_grep_args(cmd_args, arg_count);
	free(cmd_path);
	exit(EXIT_FAILURE);
}

static int	init_grep(char ***argv, char **cmd_path, char **envp)
{
	if (!*argv || !(*argv)[0])
	{
		ft_putstr_fd("Usage: grep [PATTERN] [FILE...]\n", 2);
		return (0);
	}
	*cmd_path = find_command_path("grep", envp);	
	return (1);
}

static int	execute_grep(char *cmd_path, char **argv, char **envp)
{
	pid_t	pid;
	int		status;
	int		i;
	
	i = 0;
	while (argv[i])
		i++;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		exec_grep_child(cmd_path, argv, envp, i);
	else
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		return (WEXITSTATUS(status));
	}
	return (EXIT_FAILURE);
}

int	ft_grep(int argc, char **argv, char **envp)
{
	char	*cmd_path;
	int		ret;
	(void)argc;
	
	ret = init_grep(&argv, &cmd_path, envp);
	if (ret <= 0)
		return (ret);
	return (execute_grep(cmd_path, argv, envp));
}

int ft_cat(int argc, char **argv, char **envp)
{
    (void)argc;
    return (exec_command("cat", argv, envp));
}

int ft_touch(int argc, char **argv, char **envp)
{
    (void)argc;
    if (!argv || !argv[0])
    {
        ft_putstr_fd("Usage: touch [FILE...]\n", 2);
        return (0);
    }
    return (exec_command("touch", argv, envp));
}

int ft_rm(int argc, char **argv, char **envp)
{
    (void)argc;
    if (!argv || !argv[0])
    {
        ft_putstr_fd("Usage: rm [FILE...]\n", 2);
        return (0);
    }
    return (exec_command("rm", argv, envp));
} 

int ft_mkdir(int argc, char **argv, char **envp)
{
    (void)argc;
    if (!argv || !argv[0])
    {
        ft_putstr_fd("Usage: mkdir [DIRECTORY...]\n", 2);
        return (0);
    }
    return (exec_command("mkdir", argv, envp));
}

int ft_rmdir(int argc, char **argv, char **envp)
{
    (void)argc;
    if (!argv || !argv[0])
    {
        ft_putstr_fd("Usage: rmdir [DIRECTORY...]\n", 2);
        return (0);
    }
    return (exec_command("rmdir", argv, envp));
}

int ft_wc(int argc, char **argv, char **envp)
{
	(void)argc;
	return (exec_command("wc", argv, envp));
}