CC = gcc

CFLAGS = -Wall -Wextra -Werror

READLINE_FLAGS = -lreadline

# Adjusted libft path based on your actual structure
LIBFT_PATH = ../libft
LIBFT = $(LIBFT_PATH)/libft.a

BUILTIN_SRCS = cd.c echo.c env.c exit.c pwd.c export.c unset.c \
               external_cmds.c path_utils.c redirection.c \
               exec_builtin.c pipes.c heredoc_pipes.c heredoc.c utils.c \
               cmd.c pipes_utils_cmd.c heredoc_pipes_utils.c \
               heredoc_utils.c mini_utils.c pipes_utils.c echo_utils.c path.c \
               exec_path.c

BUILTIN_OBJS = $(BUILTIN_SRCS:.c=.o)

INCLUDES = -I. -I$(LIBFT_PATH)

NAME = minishell

all: $(NAME)

$(LIBFT):
	@if [ -d $(LIBFT_PATH) ]; then \
		make -C $(LIBFT_PATH); \
	else \
		echo "Error: Directory $(LIBFT_PATH) does not exist."; \
		exit 1; \
	fi

# Fixed the dependency on libft and removed duplicate compilation of main.c
$(NAME): $(BUILTIN_OBJS) main.o $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) main.o $(BUILTIN_OBJS) $(LIBFT) $(READLINE_FLAGS)
	@echo "Compiled $(NAME) successfully"

# Separate rule for main.o to avoid duplicate compilation
main.o: main.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(BUILTIN_OBJS) main.o libbuiltins.a
	@if [ -d $(LIBFT_PATH) ]; then \
		make -C $(LIBFT_PATH) clean; \
	fi

fclean: clean
	rm -f $(NAME)
	@if [ -d $(LIBFT_PATH) ]; then \
		make -C $(LIBFT_PATH) fclean; \
	fi

re: fclean all

.PHONY: all clean fclean re