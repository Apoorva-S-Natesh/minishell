# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 11:23:39 by aschmidt          #+#    #+#              #
#    Updated: 2024/09/24 11:02:33 by aschmidt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC	= cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRC	= $(addprefix src/, main.c /input/take_input.c /input/init_shell.c /utils/envv_utils.c \
		/utils/free_utils.c /utils/close_program.c /parser/token.c)

LIBFT_PATH	= libft/

LIBFT_NAME	= libft.a

LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

OBJ		= $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME) $(LIBFT)

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
	@$(RM) $(OBJ)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
