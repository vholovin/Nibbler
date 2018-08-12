#ifndef DISPLAY_NCURSES_HPP
#define DISPLAY_NCURSES_HPP

#include "ApiDisplay.hpp"

#include <ncurses.h>

# define DELAY 			20

#define	SIZE_INFO		6
#define SIZE_BOARD		2

#define	BOARD_PAIR		1
#define TEXT_PAIR		2
#define FOOD_PAIR		3
#define BONUS_PAIR		4
#define	PLAYER0_PAIR	5
#define PLAYER1_PAIR	6

class DisplayNcurses : public ApiDisplay
{
	public:
					DisplayNcurses(void);
					DisplayNcurses(DisplayNcurses const & ref);
virtual				~DisplayNcurses(void);
DisplayNcurses &	operator=(DisplayNcurses const & ref);

unsigned			height;
unsigned			width;

virtual	void		InitDisplay(unsigned h, unsigned w);
virtual void		ExitDisplay(void);

virtual void		UpdateGame(menu *interface, std::vector<snake*> players, std::vector<obj*> foods, std::vector<obj*> bonus, std::vector<obj*> walls);
virtual	void		UpdateMenu(menu *interface, std::vector<snake*> players);

virtual int 		DelayDisplay(void);

	private:
WINDOW *			_win;
WINDOW *			_game;
WINDOW *			_info;

void				SetWindow(void);

void				UpdateInfoGame(std::vector<snake*> players);
void				UpdateInfoMenu(menu *interfase, std::vector<snake*> players);

void				ClearGame(void);
void 				ClearInfo(void);

};

std::ostream &	operator<<(std::ostream & os, DisplayNcurses const & ref);

#endif