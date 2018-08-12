#ifndef DISPLAY_SDL_HPP
#define DISPLAY_SDL_HPP

# include "ApiDisplay.hpp"

# include <SDL2/SDL.h>
# include <SDL2_ttf/SDL_ttf.h>

# define DELAY 		1

# define W_SCALE	10
# define H_SCALE	10

# define SIZE_BOARD	1
# define SIZE_INFO	10

# define BOARD_R	200
# define BOARD_G	200
# define BOARD_B	200

# define TEXT_R		255
# define TEXT_G		255
# define TEXT_B		0

# define PLAYER0_R	0
# define PLAYER0_G	0
# define PLAYER0_B	255

# define PLAYER1_R	255
# define PLAYER1_G	0
# define PLAYER1_B	0

# define FOOD_R		255
# define FOOD_G		0
# define FOOD_B		255

# define BONUS_R	0
# define BONUS_G	255
# define BONUS_B	0

class DisplaySDL : public ApiDisplay
{
	public:
					DisplaySDL(void);
					DisplaySDL(DisplaySDL const & ref);
virtual				~DisplaySDL(void);
DisplaySDL &		operator=(DisplaySDL const & ref);

unsigned			game_height;
unsigned			game_width;

unsigned			window_height;
unsigned			window_width;

virtual	void		InitDisplay(unsigned x, unsigned y);
virtual	void		ExitDisplay(void);

virtual	void		UpdateGame(menu *interface, std::vector<snake*> players, std::vector<obj*> foods, std::vector<obj*> bonus, std::vector<obj*> walls);
virtual	void		UpdateMenu(menu *interface, std::vector<snake*> players);

virtual int 		DelayDisplay(void);

 	private:
SDL_Window *		_window;
SDL_Renderer *		_renderer;

SDL_Event			_event;
const Uint8	*		_key_state;

TTF_Font *			_font;
SDL_Texture *		_message;
SDL_Rect			_message_rect;

void				UpdateInfoGame(std::vector<snake*> players);
void				UpdateInfoMenu(menu *interface, std::vector<snake*> players);

void				ResetWindow(void);
void				DrawCross(unsigned x, unsigned y, SDL_Color color);
void				DrawRect(unsigned x, unsigned y, SDL_Color color);
void				DrawRectFull(unsigned x, unsigned y, SDL_Color color);
void				GetTextAndRect(unsigned x, unsigned y, const char *text, SDL_Texture **texture, SDL_Rect *rect);
void				RenderPresent(void);

void				ExitButton(menu *interface);

};

std::ostream &	operator<<(std::ostream & os, DisplaySDL const & ref);

#endif