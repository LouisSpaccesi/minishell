#include "tokenizer.h"
#include <stdio.h> // Pour debug (fprintf stderr)
#include <stdlib.h> // Pour realloc, free, malloc
#include <ctype.h> // Pour isspace, isalnum, isalpha
#include <string.h> // Pour strdup (alternative à ft_strdup si besoin)
#include <stdbool.h> // Pour bool type
#include "../libft/libft.h" // Assurez-vous que ft_substr, ft_strdup, ft_strjoin, ft_itoa sont ici
#include "minishell.h" // <-- Ajout de l'include

// Helper pour joindre deux chaînes, libérant les anciennes.
// Prend possession de s1 et s2 (les free).
static char	*append_str(char *s1, char *s2)
{
	char	*new_str;

	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (new_str);
}

// Helper pour obtenir la valeur d'une variable d'environnement ou $?
// Retourne une NOUVELLE chaîne allouée (doit être free par l'appelant) ou NULL.
static char	*get_variable_value(t_tokenizer_state *state, t_shell *shell)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*var_value_ptr;
	char	*result;

	state->pos++; // On a déjà vu le '$', on avance
	var_start = state->pos;
	result = NULL;
	if (state->line[state->pos] == '?')
	{
		state->pos++;
		result = ft_itoa(shell->exit_status);
	}
	else if (state->line[state->pos] == '_' || ft_isalpha((unsigned char)state->line[state->pos]))
	{
		while (state->line[state->pos] == '_' || ft_isalnum((unsigned char)state->line[state->pos]))
			state->pos++;
		var_len = state->pos - var_start;
		var_name = ft_substr(state->line, var_start, var_len);
		if (var_name)
		{
			var_value_ptr = get_env_value(shell->env, var_name);
			if (var_value_ptr)
				result = ft_strdup(var_value_ptr);
			else
				result = ft_strdup(""); // Variable non définie -> chaîne vide
			free(var_name);
		}
	}
	else
	{
		// Ce n'est pas '?' ni une variable valide (e.g., $$, $1, $ ), on retourne juste "$"
		result = ft_strdup("$");
		// Pas besoin de bouger state->pos, on a déjà avancé après le '$' initial
		// et on veut que le caractère suivant soit traité normalement.
	}
	return (result);
}

// Fonction pour ajouter un token au tableau dynamique
static int	add_token(t_tokenizer_state *state, char *token)
{
	char	**new_tokens;

	if (state->token_count >= state->capacity)
	{
		state->capacity = (state->capacity == 0) ? 10 : state->capacity * 2;
		new_tokens = realloc(state->tokens, (state->capacity + 1) * sizeof(char *));
		if (!new_tokens)
		{
			free(token); // Libérer le token courant si realloc échoue
			// Libérer les tokens précédents déjà ajoutés
			while (state->token_count > 0)
				free(state->tokens[--state->token_count]);
			free(state->tokens);
			state->tokens = NULL;
			return (0); // Échec
		}
		state->tokens = new_tokens;
	}
	state->tokens[state->token_count++] = token;
	state->tokens[state->token_count] = NULL; // Toujours terminer par NULL
	return (1); // Succès
}

// Fonction pour ignorer les espaces
static void	skip_spaces(t_tokenizer_state *state)
{
	while (state->line[state->pos] && isspace((unsigned char)state->line[state->pos]))
	{
		state->pos++;
	}
}

// Fonction pour extraire un token (REFACTORISEE pour l'expansion)
static char	*extract_token(t_tokenizer_state *state, t_shell *shell)
{
	char	*result;
	char	quote_char;
	bool	in_single_quotes;
	bool	in_double_quotes;
	char	temp_char_str[2];

	result = ft_strdup("");
	in_single_quotes = false;
	in_double_quotes = false;
	temp_char_str[1] = '\0';

	while (state->line[state->pos])
	{
		quote_char = state->line[state->pos];

		// 1. Gestion des quotes (ouverture/fermeture)
		if (quote_char == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			state->pos++;
			continue ;
		}
		if (quote_char == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			state->pos++;
			continue ;
		}

		// 2. Fin du token si non quoté et espace rencontré
		if (!in_single_quotes && !in_double_quotes && isspace((unsigned char)quote_char))
			break ;

		// 3. Expansion de variable ($)
		if (quote_char == '$' && !in_single_quotes)
		{
			result = append_str(result, get_variable_value(state, shell));
			continue ; // get_variable_value a déjà avancé state->pos
		}

		// 4. Ajouter le caractère courant
		temp_char_str[0] = state->line[state->pos];
		result = append_str(result, ft_strdup(temp_char_str));
		state->pos++;
	}

	// Gestion d'erreur: quote non fermée
	if (in_single_quotes || in_double_quotes)
	{
		fprintf(stderr, "minishell: syntax error: unclosed quote\n");
		shell->exit_status = 2; // Ou un autre code d'erreur de syntaxe
		free(result);
		return (NULL);
	}

	return (result);
}

// --- Fonction principale ---

char	**tokenize_command_line(const char *line, t_shell *shell)
{
	t_tokenizer_state	state;
	char				*current_token;

	if (!line)
		return (NULL);
	// Initialisation de l'état
	state.line = line;
	state.pos = 0;
	state.tokens = NULL;
	state.token_count = 0;
	state.capacity = 0;
	state.tokens = malloc(sizeof(char *)); // Allocation initiale pour le NULL final
	if (!state.tokens)
		return (NULL);
	state.tokens[0] = NULL;
	state.capacity = 0; // Sera augmentée par add_token

	while (line[state.pos])
	{
		skip_spaces(&state);
		if (!line[state.pos]) // Fin de ligne après des espaces
			break;
		current_token = extract_token(&state, shell);
		if (!current_token)
		{
			 // Erreur de parsing (quote non fermée) ou d'allocation dans substr
			 while (state.token_count > 0) free(state.tokens[--state.token_count]);
			 free(state.tokens);
			 return NULL; 
		}
		if (!add_token(&state, current_token))
		{
			// add_token a déjà libéré la mémoire en cas d'erreur realloc
			return (NULL); // Erreur d'allocation
		}
	}

	return (state.tokens);
}
