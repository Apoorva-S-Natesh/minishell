# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aschmidt <aschmidt@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/17 11:23:39 by aschmidt          #+#    #+#              #
#    Updated: 2024/09/17 11:35:47 by aschmidt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC	= cc

CCFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRC	= $(addprefix src/, main.c)

OBJS	= $(SRC:%.c=%.o)

NAME	= minishell

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
