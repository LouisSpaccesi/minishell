#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "../libft/libft.h"
# include "minishell.h"

typedef struct s_tokenizer_state
{
	const char	*line;
	size_t		pos;
	char		**tokens;
	size_t		token_count;
	size_t		capacity;
}				t_tokenizer_state;

char			**tokenize_command_line(const char *line, t_shell *shell);

#endif
