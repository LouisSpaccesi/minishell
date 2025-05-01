#include "../libft/libft.h"
#include "minishell.h"
#include "tokenizer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

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

static char	*handle_exit_status_var(t_tokenizer_state *state, t_shell *shell)
{
	state->pos++;
	return (ft_itoa(shell->exit_status));
}

static char	*handle_regular_var(t_tokenizer_state *state, t_shell *shell)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*var_value_ptr;
	char	*result;

	var_start = state->pos;
	result = NULL;
	while (state->line[state->pos] == '_'
		|| ft_isalnum((unsigned char)state->line[state->pos]))
		state->pos++;
	var_len = state->pos - var_start;
	var_name = ft_substr(state->line, var_start, var_len);
	if (var_name)
	{
		var_value_ptr = get_env_value(shell->env, var_name);
		if (var_value_ptr)
			result = ft_strdup(var_value_ptr);
		else
			result = ft_strdup("");
		free(var_name);
	}
	return (result);
}

static char	*get_variable_value(t_tokenizer_state *state, t_shell *shell)
{
	state->pos++;
	if (state->line[state->pos] == '?')
		return (handle_exit_status_var(state, shell));
	else if (state->line[state->pos] == '_'
		|| ft_isalpha((unsigned char)state->line[state->pos]))
		return (handle_regular_var(state, shell));
	else
		return (ft_strdup("$"));
}

static int	add_token(t_tokenizer_state *state, char *token)
{
	char	**new_tokens;

	if (state->token_count >= state->capacity)
	{
		state->capacity = (state->capacity == 0) ? 10 : state->capacity * 2;
		new_tokens = realloc(state->tokens, (state->capacity + 1)
				* sizeof(char *));
		if (!new_tokens)
		{
			free(token);
			while (state->token_count > 0)
				free(state->tokens[--state->token_count]);
			free(state->tokens);
			state->tokens = NULL;
			return (0);
		}
		state->tokens = new_tokens;
	}
	state->tokens[state->token_count++] = token;
	state->tokens[state->token_count] = NULL;
	return (1);
}

static void	skip_spaces(t_tokenizer_state *state)
{
	while (state->line[state->pos]
		&& isspace((unsigned char)state->line[state->pos]))
	{
		state->pos++;
	}
}

static char	*handle_special_operators(t_tokenizer_state *state, char *result)
{
	char	temp_char_str[2];
	char	op;

	op = state->line[state->pos];
	temp_char_str[1] = '\\0';
	if (op == '<' || op == '>')
	{
		temp_char_str[0] = op;
		result = append_str(result, ft_strdup(temp_char_str));
		state->pos++;
		if (state->line[state->pos] == op)
		{
			result = append_str(result, ft_strdup(temp_char_str));
			state->pos++;
		}
		return (result);
	}
	if (op == '|')
	{
		temp_char_str[0] = op;
		result = append_str(result, ft_strdup(temp_char_str));
		state->pos++;
		return (result);
	}
	return (NULL);
}

// Helper to handle quote toggling
static bool	process_quoted_char(t_tokenizer_state *state,
		bool *in_single_quotes, bool *in_double_quotes)
{
	if (state->line[state->pos] == '\\'' && !*in_double_quotes)
	{
		*in_single_quotes = !*in_single_quotes;
		state->pos++;
		return (true);
	}
	if (state->line[state->pos] == '"' && !*in_single_quotes)
	{
		*in_double_quotes = !*in_double_quotes;
		state->pos++;
		return (true);
	}
	return (false);
}

static bool	process_variable_expansion(t_tokenizer_state *state, t_shell *shell,
		char **result, bool in_single_quotes)
{
	char	*var_value;

	if (state->line[state->pos] == '$' && !in_single_quotes)
	{
		var_value = get_variable_value(state, shell);
		if (var_value)
		{
			*result = append_str(*result, var_value);
			return (true);
		}
	}
	return (false);
}

// Helper containing the main token processing loop
static char	*process_token_loop(t_tokenizer_state *state, t_shell *shell,
		char *result, bool *in_single_quotes, bool *in_double_quotes)
{
	char	temp_char_str[2];

	temp_char_str[1] = '\\0';
	while (state->line[state->pos])
	{
		if (process_quoted_char(state, in_single_quotes, in_double_quotes))
			continue ;
		if (!*in_single_quotes && !*in_double_quotes
			&& (isspace((unsigned char)state->line[state->pos])
				|| state->line[state->pos] == '<'
				|| state->line[state->pos] == '>'
				|| state->line[state->pos] == '|'))
			break ;
		if (process_variable_expansion(state, shell, &result,
				*in_single_quotes))
			continue ;
		temp_char_str[0] = state->line[state->pos];
		result = append_str(result, ft_strdup(temp_char_str));
		state->pos++;
	}
	return (result);
}

static char	*extract_token(t_tokenizer_state *state, t_shell *shell)
{
	char	*result;
	char	*operator_token;
	bool	in_single_quotes;
	bool	in_double_quotes;

	result = ft_strdup("");
	in_single_quotes = false;
	in_double_quotes = false;
	operator_token = handle_special_operators(state, result);
	if (operator_token)
		return (operator_token); // Return early if it was an operator
	result = process_token_loop(state, shell, result, &in_single_quotes,
			&in_double_quotes);
	if (in_single_quotes || in_double_quotes)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\\n", 2);
		shell->exit_status = 2;
		free(result);
		return (NULL);
	}
	return (result);
}

char	**tokenize_command_line(const char *line, t_shell *shell)
{
	t_tokenizer_state	state;
	char				*current_token;

	if (!line)
		return (NULL);
	state.line = line;
	state.pos = 0;
	state.tokens = NULL;
	state.token_count = 0;
	state.capacity = 0;
	state.tokens = malloc(sizeof(char *));
	if (!state.tokens)
		return (NULL);
	state.tokens[0] = NULL;
	state.capacity = 0;
	while (line[state.pos])
	{
		skip_spaces(&state);
		if (!line[state.pos])
			break ;
		current_token = extract_token(&state, shell);
		if (!current_token)
		{
			while (state.token_count > 0)
				free(state.tokens[--state.token_count]);
			free(state.tokens);
			return (NULL);
		}
		if (!add_token(&state, current_token))
		{
			while (state.token_count > 0)
				free(state.tokens[--state.token_count]);
			free(state.tokens);
			free(current_token);
			return (NULL);
		}
	}
	return (state.tokens);
}
