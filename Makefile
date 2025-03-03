# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 11:43:02 by ndo-vale          #+#    #+#              #
#    Updated: 2025/03/03 10:57:19 by ndo-vale         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98 -g
ASAN		= -fsanitize=leak,address -fno-omit-frame-pointer
SRC_DIR		= src/
SRCS 		= $(wildcard $(SRC_DIR)*.cpp) #TODO WILDCARD IS FORBIDDEN
OBJ_DIR		= obj/
OBJS		= $(SRCS:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) -I./inc $(CFLAGS) -c $< -o $@

asan: fclean $(OBJS)
	$(CC) $(CFLAGS) $(ASAN) -o $(NAME) $(OBJS)

clean:
	rm -fr $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re