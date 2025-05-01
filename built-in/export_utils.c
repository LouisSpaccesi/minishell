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
