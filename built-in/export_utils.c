#include "minishell.h"

int	is_valid_identifier(const char *var)
{
	if (!var || (!ft_isalpha(*var) && *var != '_'))
		return (0);
	var++;
	while (*var)
	{
		if (!ft_isalnum(*var) && *var != '_')
			return (0);
		var++;
	}
	return (1);
}

char	*create_env_entry(const char *var, const char *value)
{
	char	*new_entry;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!value)
		value = "";
	new_entry = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	if (!new_entry)
		return (NULL);
	while (var[i])
	{
		new_entry[i] = var[i];
		i++;
	}
	new_entry[i++] = '=';
	while (value[j])
	{
		new_entry[i + j] = value[j];
		j++;
	}
	new_entry[i + j] = '\0';
	return (new_entry);
}
