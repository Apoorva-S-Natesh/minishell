# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asomanah <asomanah@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 11:23:39 by aschmidt          #+#    #+#              #
#    Updated: 2024/11/14 19:52:42 by asomanah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC	= cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRC	= $(addprefix src/, main.c /input/take_input.c /input/init_shell.c \
		/utils/envv_utils.c /utils/free_utils.c /utils/close_program.c \
		/parser/token.c /parser/token_maker.c \
		/parser/command.c /parser/token_to_command.c  \
		/parser/expand_value.c /parser/validate_tokens.c\
		/parser/expand.c /parser/quotes.c /parser/parsing_redirection.c \
		/execute/execute.c /execute/execution.c /execute/signals.c \
		/execute/builtin_cd.c /execute/builtin_echo.c /execute/builtin_pwd.c \
		/execute/builtin_env.c /execute/builtin_exit.c \
		/execute/builtin_unset.c /execute/builtins.c /execute/handle_heredoc.c \
		/execute/exec_path.c print.c /execute/export_utils.c\
		/execute/execute_utils.c /execute/redirections.c \
		/execute/heredoc_utils.c /execute/builtin_export.c \
		/execute/execute_child.c /execute/process_utils.c \
		/execute/export_set_env.c)

LIBFT_PATH	= libft/

LIBFT_NAME	= libft.a

LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

OBJ		= $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT) $(LDFLAGS)

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
	@$(RM) $(OBJ)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@$(RM) $(NAME)

re: fclean all

readline.supp:
	@echo "{" > readline.supp
	@echo "   readline_leak" >> readline.supp
	@echo "   Memcheck:Leak" >> readline.supp
	@echo "   ..." >> readline.supp
	@echo "   obj:*/libreadline.so*" >> readline.supp
	@echo "}" >> readline.supp

check_leaks: $(NAME) readline.supp
	@valgrind --leak-check=full --show-leak-kinds=all \
	--suppressions=readline.supp ./$(NAME)

.PHONY: all clean fclean re check_leaks
