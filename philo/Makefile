# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: x03phy <x03phy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/11 17:50:18 by ebonutto          #+#    #+#              #
#    Updated: 2025/03/14 14:59:20 by x03phy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Werror -Wall -Wextra -g
INC_PATH = include/

SRC_PATH = src/
SRC = main.c \
		parsing.c \
		init.c \
		philosophers.c \
		time.c \
		actions.c \
		supervisor.c

OBJ_PATH = obj/
OBJ = $(addprefix $(OBJ_PATH), $(SRC:.c=.o))

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -I $(INC_PATH) -c $< -o $@ 

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re