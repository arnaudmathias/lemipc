# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amathias <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/03 11:09:29 by amathias          #+#    #+#              #
#    Updated: 2018/01/04 15:47:54 by amathias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC =	main.c				\
		alloc.c				\
		game.c				\
		game_utils.c		\
		messages.c			\
		pathfinding.c		\
		pathfinding_utils.c	\
		ia.c				\
		lpos.c				\
		calc_utils.c		\
		utils.c				\

SRC_GRAPH = main_graph.c	\
			alloc_graph.c	\
			utils.c			\
			messages.c		\
			calc_utils.c	\

SRC_FILES = $(addprefix ./src/,$(SRC))
SRC_FILES_GRAPH = $(addprefix ./src/,$(SRC_GRAPH))
OBJ = $(SRC_FILES:.c=.o)
OBJ_GRAPH = $(SRC_FILES_GRAPH:.c=.o)
NAME = lemipc
NAME = lemipc-graph
CC = clang
RM = rm -f
CFLAGS = -Wall -Werror -Wextra

LIB_PATH = ./libft/
LIB_NAME = -lft
LIB = $(addprefix -L,$(LIB_PATH))
LFLAGS = $(LIB) $(LIB_NAME)

INC_PATH = ./include
INC = $(addprefix -I,$(INC_PATH))

all: lib lemipc lemipc-graph

lib:
	make -C ./libft

lemipc: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o $@

lemipc-graph: $(OBJ_GRAPH)
	$(CC) $(CFLAGS) $(OBJ_GRAPH) $(LFLAGS) -o $@

%.o: %.c
	$(CC) $(INC) -o $@ -c $^ $(CFLAGS)

clean:
	$(RM) $(OBJ)
	$(RM) $(OBJ_GRAPH)
	make clean -C ./libft

fclean: clean
	$(RM) lemipc
	$(RM) lemipc-graph
	make fclean -C ./libft

re: fclean all

debug: CFLAGS += -g -D _DEBUG
debug:
	$(RM) $(OBJ)
	$(RM) $(OBJ_GRAPH)
	make debug -C ./libft
	make $(NAME)

