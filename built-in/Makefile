CC = gcc

CFLAGS = -Wall -Wextra -Werror

READLINE_FLAGS = -lreadline

SRCS = main.c cd.c echo.c env.c exit.c pwd.c

OBJS = $(SRCS:.c=.o)

HEADERS = minishell.h

NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE_FLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re