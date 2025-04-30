/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis <louis@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:50:46 by lospacce          #+#    #+#             */
/*   Updated: 2025/04/30 18:29:19 by louis            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env(const char *var, const char *value)
{
	char	*new_entry;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_entry = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	if (!new_entry)
		return (NULL);
	while (var[i])
	{
		new_entry[i] = var[i];
		i++;
	}
	new_entry[i++] = '=';
	j = 0;
	while (value[j])
	{
		new_entry[i + j] = value[j];
		j++;
	}
	new_entry[i + j] = '\0';
	return (new_entry);
}

char **copy_env_safe(char **env)
{
    int count = 0;
    char **new_env;
    int i;
    
    // Compter les entrées
    while (env[count])
        count++;
    
    // Allouer le nouveau tableau
    new_env = malloc((count + 1) * sizeof(char *));
    if (!new_env)
        return NULL;
    
    // Copier chaque entrée
    i = 0;
    while (i < count)
    {
        new_env[i] = ft_strdup(env[i]);
        if (!new_env[i])
        {
            // En cas d'erreur, libérer ce qui a été alloué
            while (--i >= 0)
                free(new_env[i]);
            free(new_env);
            return NULL;
        }
        i++;
    }
    new_env[count] = NULL;
    
    return new_env;
}

void	copy_env(char **new_env, char **env, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		new_env[j] = env[j];
		j++;
	}
}

void ft_env(t_shell *shell)
{
    int i = 0;
    t_env_var *current;

    // Afficher l'environnement d'origine
    while (shell->env[i])
    {
        write(STDOUT_FILENO, shell->env[i], ft_strlen(shell->env[i]));
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }

    // Afficher les variables personnalisées
    current = shell->custom_env;
    while (current)
    {
        write(STDOUT_FILENO, current->name, ft_strlen(current->name));
        write(STDOUT_FILENO, "=", 1);
        write(STDOUT_FILENO, current->value, ft_strlen(current->value));
        write(STDOUT_FILENO, "\n", 1);
        current = current->next;
    }
}
