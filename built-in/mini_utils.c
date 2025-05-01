#include "minishell.h"

void	free_array(char **array)
{
	if (!array)
		return ;
	free(array);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_env(shell->env);
	if (shell->original_stdin != -1)
		close(shell->original_stdin);
	free(shell);
}

int	is_builtin(char *cmd)
{
	return (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "pwd", 4) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}
