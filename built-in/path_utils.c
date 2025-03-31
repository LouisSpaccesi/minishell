/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospacce < lospacce@student.42angouleme    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:50:00 by lospacce          #+#    #+#             */
/*   Updated: 2025/03/30 00:14:51 by lospacce         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_path_env(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

static char *build_full_path(char *path, char *cmd)
{
    char *temp;
    char *full_path;

    temp = ft_strjoin(path, "/");
    if (!temp)
        return (NULL);
    
    full_path = ft_strjoin(temp, cmd);
    free(temp);
    return (full_path);
}

static char *search_in_paths(char **paths, char *cmd)
{
    char *full_path;
    int i;

    i = 0;
    while (paths[i])
    {
        full_path = build_full_path(paths[i], cmd);
        if (!full_path)
            continue;
            
        if (access(full_path, X_OK) == 0)
            return (full_path);
            
        free(full_path);
        i++;
    }
    return (NULL);
}

static void free_paths_array(char **paths)
{
    int i;

    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);
}

char *find_command_path(char *cmd, char **envp)
{
    char *path_env;
    char **paths;
    char *full_path;

    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));
        
    path_env = get_path_env(envp);
    if (!path_env)
        return (NULL);
        
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
        
    full_path = search_in_paths(paths, cmd);
    free_paths_array(paths);
    
    return (full_path);
}

static char	**prepare_cmd_args(char *cmd_path, char **args, int arg_count)
{
	char	**cmd_args;
	int		j;
	
	cmd_args = malloc((arg_count + 2) * sizeof(char *));
	if (!cmd_args)
		return (NULL);
	
	cmd_args[0] = cmd_path;
	
	j = 0;
	while (j < arg_count)
	{
		cmd_args[j + 1] = args[j];
		j++;
	}
	cmd_args[j + 1] = NULL;
	
	return (cmd_args);
}

static void	handle_exec_failure(char *cmd_path, char **cmd_args)
{
	perror("execve");
	if (cmd_args)
		free(cmd_args);
	if (cmd_path)
		free(cmd_path);
	exit(EXIT_FAILURE);
}

static void	exec_cmd_child(char *cmd_path, char **args, char **envp, int arg_count)
{
	char	**cmd_args;
	
	cmd_args = prepare_cmd_args(cmd_path, args, arg_count);
	if (!cmd_args)
	{
		perror("malloc");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, cmd_args, envp);
	handle_exec_failure(cmd_path, cmd_args);
}

static int handle_command_not_found(char *cmd)
{
	ft_putstr_fd("Command not found: ", 2);
	ft_putendl_fd(cmd, 2);
	return (0);
}

static int count_args(char **args)
{
	int i;
	
	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

int	handle_child_process(char *cmd_path, char **args, char **envp, int arg_count)
{
	exec_cmd_child(cmd_path, args, envp, arg_count);
	free(cmd_path);
	exit(EXIT_FAILURE); 
}

int handle_parent_process(pid_t pid, char *cmd_path) 
{
    int status;
    
    waitpid(pid, &status, 0);
    free(cmd_path);
    return status;
}

int	exec_command(char *cmd, char **args, char **envp)
{
	char	*cmd_path;
	pid_t	pid;
	int		arg_count;

	arg_count = count_args(args);
	cmd_path = find_command_path(cmd, envp);
	if (!cmd_path)
		return (handle_command_not_found(cmd));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		handle_child_process(cmd_path, args, envp, arg_count);
	else
		return (handle_parent_process(pid, cmd_path));
	return (EXIT_FAILURE); 
}
