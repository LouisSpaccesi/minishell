CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

NAME = minishell

SRCS = minishell.c

OBJS = $(SRCS:.c=.o)

LIBS = -lreadline

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Done!$(RESET)"

%.o: %.c
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@$(RM) $(OBJS)

fclean: clean
	@echo "$(RED)Cleaning program...$(RESET)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
