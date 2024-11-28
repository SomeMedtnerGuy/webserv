NAME = webserver
CPP = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
CYAN_BOLD=\033[1m\033[36m
BRANCO_BOLD=\033[1;37m
BOLD_YELLOW=\033[1m\033[33m
NC=\033[0m
SRC_DIR = ./src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_DIR = obj
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CXXFLAGS) -s -o $(NAME) $(OBJS)
	@echo "\n$(BOLD_YELLOW)--|$(CYAN_BOLD) COMPILATION OF ALL FILES IS COMPLETE $(BOLD_YELLOW)|--$(NC)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CPP) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	@$(CPP) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\n$(BOLD_YELLOW)--|$(BRANCO_BOLD) CLEANED ALL PROGRAM FILES $(BOLD_YELLOW)|--$(NC)\n"

fclean: clean
	@rm -f $(OBJS) $(NAME)
	@echo "\n$(BOLD_YELLOW)--|$(BRANCO_BOLD) ALL FILES HAVE BEEN CLEANED $(BOLD_YELLOW)|--$(NC)\n"

re: fclean all
	@make -s

.PHONY: all clean fclean re