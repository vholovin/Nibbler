NAME 		=	lib_sfml.so

COM 		=	clang++
FLAGS 		=	-Wall -Wextra -Werror
OPTIMA		=	-O3

SFML_DIR 	=	./SFML
SFML_INC 	=	$(SFML_DIR)/SFML-2.5.0-macOS-clang/include
SFML_FRM	=	$(SFML_DIR)/SFML-2.5.0-macOS-clang/Frameworks
SFML_LINK 	=	-F$(SFML_FRM) -framework SFML -framework sfml-graphics -framework sfml-window -framework sfml-system

PWD 		=	$(shell pwd)
LIB_FLAG	=	-shared -rpath $(PWD)/$(SFML_FRM) -nostartfiles -dynamiclib

SRC_FILES	=	DisplaySFML.cpp			\
				../srcs/ApiDisplay.cpp	\
				../srcs/menu.cpp		\
				../srcs/obj.cpp			\
				../srcs/snake.cpp

INC_FILES	=	DisplaySFML.hpp

INC_DIR		=	../includes

OBJ_FILES 	=	$(SRC_FILES:.cpp=.cpp.o)

GREEN		=	\033[32m
YELLOW		=	\033[33m
RED			=	\033[31m
RESET		=	\033[39m

all: sfml $(NAME)

%.cpp.o:%.cpp
	@ $(COM) $(FLAGS) $(OPTIMA) -c -o $@ $< -I $(INC_DIR) -I $(SFML_INC)

$(NAME): $(OBJ_FILES)
	@ $(COM) $(FLAGS) $(OPTIMA) -o $@ $(OBJ_FILES) $(SFML_LINK) $(LIB_FLAG) -I $(SFML_INC)

sfml:
	@ mkdir -p $(SFML_DIR)
	@ test -d SFML/SFML-2.5.0-macOS-clang && echo "$(YELLOW)File SFML already exist$(RESET)" || make sfml_download

sfml_download:
	@ echo "$(GREEN)SFML: download SFML files$(RESET)"

	@ curl -OL http://www.sfml-dev.org/files/SFML-2.5.0-macOS-clang.tar.gz
	@ tar zxvfC SFML-2.5.0-macOS-clang.tar.gz $(SFML_DIR)
	@ rm -rf SFML-2.5.0-macOS-clang.tar.gz
	@ cd $(SFML_FRM) && ln -s ../extlibs/freetype.framework

clean:
	@ rm -rf $(OBJ_FILES)
	@ echo "$(RED)SFML: Object removed$(RESET)"

	@ rm -rf $(SFML_DIR)
	@ echo "$(RED)SFML: Directory removed$(RESET)"

fclean: clean
	@ rm -rf $(NAME)
	@ echo "$(RED)SFML: Binary file removed$(RESET)"

re: fclean all

.PHONY: all sfml clean fclean re