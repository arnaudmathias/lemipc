# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amathias <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/03 11:09:29 by amathias          #+#    #+#              #
#    Updated: 2017/12/13 15:00:41 by amathias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC =	main.c			\
		alloc.c			\
		game.c			\
		messages.c		\
		ia.c			\
		utils.c			\

SRC_FILES = $(addprefix ./src/,$(SRC))
OBJ = $(SRC_FILES:.c=.o)
NAME = lemipc
CC = clang
RM = rm -f
CFLAGS = -Wall -Werror -Wextra -pedantic -g -fsanitize=undefined

LIB_PATH = ./libft/
LIB_NAME = -lft
LIB = $(addprefix -L,$(LIB_PATH))
LFLAGS = $(LIB) $(LIB_NAME)

INC_PATH = ./include
INC = $(addprefix -I,$(INC_PATH))

all: lib $(NAME)

lib:
	make -C ./libft

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o $@

%.o: %.c
	$(CC) $(INC) -o $@ -c $^ $(CFLAGS)

clean:
	$(RM) $(OBJ)
	make clean -C ./libft

fclean: clean
	$(RM) $(NAME)
	make fclean -C ./libft

re: fclean all

debug: CFLAGS += -g -D _DEBUG
debug:
	$(RM) $(OBJ)
	make debug -C ./libft
	make $(NAME)

