/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:32:32 by fben-ham          #+#    #+#             */
/*   Updated: 2025/05/01 20:37:14 by fben-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This helper extracts name and sets value pointer.
// Keep it static as it's only used by validate_export_arg here.
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
		*value_ptr = ""; // Point to empty string if no '='
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

static char	*validate_export_arg(char *arg, char **value_ptr)
{
	char	*var;

	*value_ptr = NULL;
	var = extract_variable_name(arg, value_ptr);
	if (!var)
		return (NULL);
	if (!is_valid_identifier(var))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(var);
		return (NULL);
	}
	return (var);
}

static int	export_process_arg(char *arg, t_shell *shell)
{
	char	*var;
	char	*value;
	int		ret;

	var = validate_export_arg(arg, &value);
	if (!var)
		return (1);
	if (ft_strchr(arg, '=') == NULL)
	{
		free(var);
		return (0);
	}
	ret = set_env_var(shell, var, value);
	free(var);
	return (ret);
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
