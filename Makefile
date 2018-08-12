NAME		=	nibbler

COM			=	clang++
FLAGS		=	-Wall -Wextra -Werror
OPTIMA		=	-O3

DL_FLAG		=	-rdynamic
DL_LINK		=	-ldl

SRC_DIR		=	./srcs/
INC_DIR		=	./includes/
OBJ_DIR		=	./objs/

SRC_FILES	= 	ApiDisplay.cpp \
				snake.cpp \
				obj.cpp \
				menu.cpp \
				nibbler.cpp \
				main.cpp

INC_FILES 	= 	ApiDisplay.hpp \
				snake.hpp \
				obj.hpp \
				menu.hpp \
				nibbler.hpp

OBJ_FILES 	=	$(SRC_FILES:.cpp=.cpp.o)

SRCS 		=	$(addprefix $(SRC_DIR), $(SRC_FILES))
HDRS 		=	$(addprefix $(INC_DIR), $(INC_FILES))
OBJS 		= 	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

GREEN		=	\033[32m
RESET		=	\033[39m
YELLOW		=	\033[33m
RED 		=	\033[31m

all: obj $(NAME)

obj:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.cpp.o:$(SRC_DIR)%.cpp
	$(COM) $(FLAGS) $(OPTIMA) -c $< -o $@ -I $(INC_DIR)

$(NAME): ncurses sdl sfml $(OBJS) $(HDRS)
	@$(COM) $(FLAGS) $(OPTIMA) $(DL_FLAG) $(OBJS) -o $(NAME) $(DL_LINK)

ncurses:
	@make -C ./lib_ncurses/
	@echo "$(GREEN)Build Ncurses: OK$(RESET)"

sdl:
	@make -C ./lib_sdl/
	@echo "$(GREEN)Build SDL: OK$(RESET)"

sfml:
	@make -C ./lib_sfml/
	@echo "$(GREEN)Build SFML: OK$(RESET)"

clean:
	@make -C lib_ncurses clean
	@make -C lib_sdl clean
	@make -C lib_sfml clean
	@rm -rf $(OBJ)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@make -C lib_ncurses fclean
	@make -C lib_sdl fclean
	@make -C lib_sfml fclean
	@rm -rf $(NAME)
	@echo "$(RED)Attention: $(RESET)$(NAME) was removed"

re:		fclean all

.PHONY: all ncurses sdl sfml clean fclean re