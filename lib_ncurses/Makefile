NAME 		=	lib_ncurses.so

COM			=	clang++
FLAGS		=	-Wall -Wextra -Werror
OPTIMA		=	-O3

LIB_FLAG	=	-shared -nostartfiles -dynamiclib
LINK_NC		=	-lncurses

SRC_FILES	=	DisplayNcurses.cpp		\
				../srcs/ApiDisplay.cpp	\
				../srcs/menu.cpp		\
				../srcs/obj.cpp			\
				../srcs/snake.cpp

INC_FILES	=	DisplayNcurses.hpp

INC_DIR		=	../includes/

OBJ_FILES	= $(SRC_FILES:.cpp=.cpp.o)

all: $(NAME)

%.cpp.o:%.cpp
	$(COM) $(FLAGS) $(OPTIMA) -c $< -o $@ -I $(INC_DIR)

$(NAME): $(OBJ_FILES) $(INC_FILES)
		@$(COM) $(FLAGS) $(OPTIMA) $(LINK_NC) $(LIB_FLAG) $(OBJ_FILES) -o $(NAME)

clean:
	@rm -rf $(OBJ_FILES)

fclean:	clean
	@rm -rf $(NAME)

re:		fclean all

.PHONY: all clean fclean re