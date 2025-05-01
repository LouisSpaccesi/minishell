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


char	**tokenize_command_line(const char *line, t_shell *shell);

#endif
