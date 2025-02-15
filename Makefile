# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 11:43:02 by ndo-vale          #+#    #+#              #
#    Updated: 2025/02/13 11:36:18 by ndo-vale         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
SRC_DIR		= src/
SRCS		= $(addprefix $(SRC_DIR), test.cpp \
					Connection.cpp TimeoutController.cpp \
					ConfigFile.cpp Location.cpp Server.cpp ServerSettings.cpp utils.cpp) 
OBJ_DIR		= obj/
OBJS		= $(SRCS:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -s -o $(NAME) $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) -I./inc $(CXXFLAGS) -c $< -o $@

clean:
	rm -fr $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re