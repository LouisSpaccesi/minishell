#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <stdlib.h> // Pour size_t
# include "minishell.h" // Ajout pour dépendance t_shell
# include "../libft/libft.h" // Pour ft_isspace, ft_substr, etc.

// Structure pour aider à gérer l'état du tokenizer
typedef struct s_tokenizer_state
{
	const char	*line;
	size_t		pos;
	char		**tokens;
	size_t		token_count;
	size_t		capacity;
}				t_tokenizer_state;

/**
 * @brief Découpe une ligne de commande en tokens (arguments).
 * 
 * Gère les espaces comme séparateurs, mais respecte les guillemets simples (')
 * et doubles (") pour regrouper des arguments. Les guillemets externes
 * qui servent au regroupement sont retirés des tokens résultants.
 * 
 * Exemple :
 *   "echo \"hello world\" 'another arg'" -> ["echo", "hello world", "another arg"]
 * 
 * NOTE : Ne gère PAS encore l'expansion de variables, les caractères échappés
 * à l'intérieur des guillemets, la concaténation (a'b'c -> abc), 
 * ou les opérateurs spéciaux comme |, <, >.
 *
 * @param line La ligne de commande à découper.
 * @param shell Pointeur vers la structure shell pour l'expansion.
 * @return Un tableau de chaînes (char **) terminé par NULL, alloué dynamiquement.
 *         Retourne NULL en cas d'erreur d'allocation ou si la ligne est NULL.
 *         L'appelant doit libérer le tableau et chaque chaîne qu'il contient.
 */
char	**tokenize_command_line(const char *line, t_shell *shell);

#endif
