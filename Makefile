# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joamonte <joamonte@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/21 15:05:33 by joamonte          #+#    #+#              #
#    Updated: 2024/11/21 15:07:32 by joamonte         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC = src/main.cpp \
		src/Server.cpp

OBJS = ${SRC:.cpp=.o}

INCLUDE = -I inc

CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

.cpp.o:
				${CC} ${CPPFLAGS} ${INCLUDE} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
				${CC} ${CPPFLAGS} ${INCLUDE} ${OBJS} -o ${NAME}

all:	${NAME}

clean:
				${RM} ${OBJS}

fclean: clean
				${RM} ${NAME}

re: clean all

.PHONY: all clean re