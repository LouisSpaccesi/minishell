#include "minishell.h"

static void	ft_unset_internal(char *args, t_shell *shell)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(args);
	if (!shell || !shell->env)
		return ;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], args, var_len) == 0
			&& shell->env[i][var_len] == '=')
		{
			free(shell->env[i]);
			while (shell->env[i + 1])
			{
				shell->env[i] = shell->env[i + 1];
				i++;
			}
			shell->env[i] = NULL;
			return ;
		}
		i++;
	}
}

int	ft_unset_command(char **args, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			ft_unset_internal(args[i], shell);
		i++;
	}
	return (status);
}
