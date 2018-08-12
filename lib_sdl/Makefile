NAME 		=	lib_sdl.so

COM			=	clang++
FLAGS		=	-Wall -Wextra -Werror
OPTIMA		=	-O3

SDL_DIR		=	./SDL
SDL_FRM		=	-F $(SDL_DIR)
SDL_LINK	=	-framework SDL2 -framework SDL2_ttf

PWD			= 	$(shell pwd)
LIB_FLAG	=	-shared  -rpath $(PWD)/$(SDL_DIR) -nostartfiles -dynamiclib

SRC_FILES	=	DisplaySDL.cpp			\
				../srcs/ApiDisplay.cpp	\
				../srcs/menu.cpp		\
				../srcs/obj.cpp			\
				../srcs/snake.cpp

INC_FILES	=	DisplaySDL.hpp

INC_DIR		=	../includes

OBJ_FILES	= 	$(SRC_FILES:.cpp=.cpp.o)

GREEN		=	\033[32m
YELLOW		=	\033[33m
RED			=	\033[31m
RESET		=	\033[39m

SDL2 		=	SDL2
SDL2_TTF 	=	SDL2_ttf

all: sdl $(NAME)

%.cpp.o:%.cpp
	@ $(COM) $(FLAGS) $(OPTIMA) $(SDL_FRM) -c -o $@ $< -I $(INC_DIR)

$(NAME): $(OBJ_FILES)
	@ $(COM) $(FLAGS) $(OPTIMA) $(SDL_FRM) $(SDL_LINK) $(LIB_FLAG) $(OBJ_FILES) -o $(NAME)

sdl:
	@ mkdir -p $(SDL_DIR)

	@ test -f SDL/SDL2.framework/SDL2 && echo "$(YELLOW)File $(SDL2) already exist$(RESET)" || make sdl2_download
	@ test -f SDL/SDL2_ttf.framework/SDL2_ttf && echo "$(YELLOW)File $(SDL2_TTF) already exist$(RESET)" || make sdl2_ttf_download

sdl2_download:
	@ echo "$(GREEN)SDL: download SDL2.framework$(RESET)"
	@ curl -o sdl2.dmg https://www.libsdl.org/release/SDL2-2.0.8.dmg
	@ hdiutil attach sdl2.dmg;
	@ cd ; cp -R -i ../../../Volumes/SDL2/SDL2.framework $(PWD)/$(SDL_DIR)/SDL2.framework; hdiutil eject ../../../Volumes/SDL2;
	@ rm -rf sdl2.dmg

sdl2_ttf_download:
	@ echo "$(GREEN)SDL: download SDL2_ttf.framework$(RESET)"
	@ curl -o sdl2_ttf.dmg https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.dmg
	@ hdiutil attach sdl2_ttf.dmg;
	@ cd ; cp -R -i ../../../Volumes/SDL2_ttf/SDL2_ttf.framework $(PWD)/$(SDL_DIR)/SDL2_ttf.framework; hdiutil eject ../../../Volumes/SDL2_ttf;
	@ rm -rf sdl2_ttf.dmg


clean:
	@ rm -rf $(OBJ_FILES)
	@ echo "$(RED)SDL: Object removed$(RESET)"

	@ rm -fr $(SDL_DIR)
	@ echo "$(RED)SDL: Directory removed$(RESET)"

fclean:	clean
	@ rm -rf $(NAME)
	@ echo "$(RED)SDL: Binary file removed$(RESET)"
	
re:	fclean all

.PHONY: all sdl clean fclean re