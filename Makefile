# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iouardi <iouardi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/14 16:41:34 by iouardi           #+#    #+#              #
#    Updated: 2022/03/23 18:28:10 by iouardi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=            pipex
TEST_FILE=        Test.c
FLAGS=            -Wall -Werror -Wextra
CC=                cc
SRC_FILES = mandatory/pipex.c
OBJ_FILES=			$(SRC_FILES:.c=.o)
OBJ_FILES_BONUS=	$(SRC_FILES_BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@make -C mandatory/libft
	@cp mandatory/libft/libft.a ./
	$(CC) $(OBJ_FILES) -o $(NAME) libft.a
%.o: %.c
	$(CC) -Wall -Wextra -Werror -c $< -o $@

clean:
	find . -name "*.o" -delete
	find . -name "*.a" -delete

fclean: clean
	rm -f $(NAME)

re: fclean all
