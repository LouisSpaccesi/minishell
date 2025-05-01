#include "minishell.h"

static char	*extract_variable_name(const char *arg, char **value_ptr)
{
	char	*equals_pos;
	char	*var;
	int		var_len;

	*value_ptr = NULL;
	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
	{
		var_len = ft_strlen(arg);
		*value_ptr = "";
	}
	else
	{
		var_len = equals_pos - arg;
		*value_ptr = equals_pos + 1;
	}
	var = malloc(var_len + 1);
	if (!var)
		return (NULL);
	ft_strlcpy(var, arg, var_len + 1);
	var[var_len] = '\0';
	return (var);
}

static int	replace_variable(char **env, char *var, char *new_entry)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (env && env[i])
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
		{
			free(env[i]);
			env[i] = new_entry;
			free(var);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	add_new_variable(t_shell *shell, char *new_entry)
{
	char	**new_env;
	int		count;

	count = 0;
	if (shell->env)
		while (shell->env[count])
			count++;
	new_env = malloc((count + 2) * sizeof(char *));
	if (!new_env)
	{
		free(new_entry);
		return (0);
	}
	ft_memcpy(new_env, shell->env, count * sizeof(char *));
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	if (shell->env)
		free(shell->env);
	shell->env = new_env;
	return (1);
}

static int	export_process_arg(char *arg, t_shell *shell)
{
	char	*var;
	char	*value;
	char	*new_entry;

	var = extract_variable_name(arg, &value);
	if (!var)
		return (1);
	if (!is_valid_identifier(var))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(var);
		return (1);
	}
	if (ft_strchr(arg, '=') == NULL)
	{
		free(var); 
		return (0);
	}
	new_entry = create_env_entry(var, value);
	if (!new_entry)
	{
		free(var);
		return (1); 
	}
	if (replace_variable(shell->env, var, new_entry))
	{
		return (0); 
	}
	if (!add_new_variable(shell, new_entry))
	{
		free(var); 
		return (1); 
	}
	free(var);
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args[1])
		return (ft_env_builtin(shell));
	i = 1;
	status = 0;
	while (args[i])
	{
		if (export_process_arg(args[i], shell) != 0)
			status = 1;
		i++;
	}
	return (status);
}