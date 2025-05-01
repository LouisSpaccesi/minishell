#include "minishell.h"

int	get_redirection_type(char *arg)
{
	if (!arg) 
		return (0);
	if (ft_strncmp(arg, ">>", 3) == 0) 
		return (2);
	else if (ft_strncmp(arg, ">", 2) == 0) 
		return (1);
	else if (ft_strncmp(arg, "<<", 3) == 0) 
		return (3);
	else if (ft_strncmp(arg, "<", 2) == 0) 
		return (4);
	return (0);
}

void	remove_redirection_args(char **args, int index)
{
	int	i = index;
	if (!args || !args[index] || !args[index + 1]) 
		return ;
	while (args[i + 2]) { 
		args[i] = args[i + 2];
		i++; 
	}
	args[i] = NULL; args[i + 1] = NULL;
}

int	ft_redirect_output(const char *filename)
{
	int	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	int	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1) { perror("minishell: dup failed"); return (-1); }
	if (fd == -1) { perror("minishell"); close(saved_stdout); return (-1); }
	if (dup2(fd, STDOUT_FILENO) == -1) { perror("minishell: dup2 failed"); close(fd); close(saved_stdout); return (-1); }
	close(fd); return (saved_stdout);
}

int	ft_redirect_output_append(const char *filename)
{
	int	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	int	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1) { perror("minishell: dup failed"); return (-1); }
	if (fd == -1) { perror("minishell"); close(saved_stdout); return (-1); }
	if (dup2(fd, STDOUT_FILENO) == -1) { perror("minishell: dup2 failed"); close(fd); close(saved_stdout); return (-1); }
	close(fd); return (saved_stdout);
}

int	ft_redirect_input(const char *filename)
{
	int	fd = open(filename, O_RDONLY);
	int	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1) { perror("minishell: dup failed"); return (-1); }
	if (fd == -1) { perror("minishell"); close(saved_stdin); return (-1); }
	if (dup2(fd, STDIN_FILENO) == -1) { perror("minishell: dup2 failed"); close(fd); close(saved_stdin); return (-1); }
	close(fd); return (saved_stdin);
}
