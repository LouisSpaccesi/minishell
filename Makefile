# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fben-ham <fben-ham@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/01 20:25:47 by fben-ham          #+#    #+#              #
#    Updated: 2025/05/01 21:05:00 by fben-ham         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -Ibuilt-in -Ilibft
LDFLAGS = 
LDLIBS = -lreadline -lncurses

SRC_DIR = built-in
LIBFT_DIR = libft

SRCS_BASENAME = cd.c echo.c env.c exit.c pwd.c export.c unset.c \
                external_cmds.c path_utils.c redirection.c \
                exec_builtin.c pipes.c pipes_heredoc.c heredoc_pipes.c heredoc.c utils.c \
                cmd.c pipes_utils_cmd.c heredoc_pipes_utils.c \
                heredoc_utils.c mini_utils.c pipes_utils.c echo_utils.c path.c \
                exec_path.c signals.c tokenizer.c \
                env_utils.c export_utils.c redirection_utils.c \
                redirection_basic.c main.c

OBJS = $(addprefix $(SRC_DIR)/, $(SRCS_BASENAME:.c=.o))

LIBFT = $(LIBFT_DIR)/libft.a

.PHONY: all clean fclean re libft

# Default rule: Ensure libft is built before linking NAME
all: libft $(NAME)

# Target to specifically build libft (now used as a prerequisite)
libft: $(LIBFT)

# Rule to build libft archive
$(LIBFT):
	@echo "--- Making Libft ---"
	@make -C $(LIBFT_DIR) --no-print-directory

# Rule to link the final executable
$(NAME): $(OBJS) $(LIBFT)
	@echo "--- Linking $(NAME) ---"
	$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) $(LDLIBS) -o $(NAME)
	@echo "$(NAME) created successfully."

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/minishell.h $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "--- Cleaning Objects ---"
	@rm -f $(OBJS)
	@echo "--- Cleaning Libft ---"
	@make -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	@echo "--- Full Cleaning Libft ---"
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "--- Removing $(NAME) ---"
	@rm -f $(NAME)

re: fclean all