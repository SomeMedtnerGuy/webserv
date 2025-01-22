# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 09:49:04 by ndo-vale          #+#    #+#              #
#    Updated: 2024/12/29 10:33:47 by ndo-vale         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CPP = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
SRC_DIR = ./src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_DIR = obj
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CXXFLAGS) -s -o $(NAME) $(OBJS)
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CPP) -I./inc $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CPP) $(CXXFLAGS) -c $< -o $@

clean:
	rm -fr $(OBJ_DIR)
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re