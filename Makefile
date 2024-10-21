# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 11:23:39 by aschmidt          #+#    #+#              #
#    Updated: 2024/10/03 14:02:35 by aschmidt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC	= cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRC	= $(addprefix src/, main.c /input/take_input.c /input/init_shell.c \
		/utils/envv_utils.c /utils/free_utils.c /utils/close_program.c \
		/utils/ft_strtok.c /parser/token.c /parser/token_maker.c \
		/parser/command.c /parser/token_to_command.c /parser/redirection.c \
		/parser/expand.c /parser/quotes.c execute/execute.c execute/signals.c \
		execute/builtin_cd.c execute/builtin_echo.c execute/builtin_pwd.c \
		execute/builtin_env.c execute/builtin_exit.c execute/builtin_export.c \
		execute/builtin_unset.c execute/builtins.c execute/handle_heredoc.c \
		execute/exec_path.c execute/variable_expansion_heredoc.c print.c \
		execute/execute_utils.c /execute/redirections.c)

LIBFT_PATH	= libft/

LIBFT_NAME	= libft.a

LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

OBJ		= $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT) $(LDFLAGS)

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
	@$(RM) $(OBJ)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
