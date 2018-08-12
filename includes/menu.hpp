#ifndef MENU_HPP
#define MENU_HPP

#include "snake.hpp"

#include <vector>
#include <string>

class menu
{
	public:
			menu(void);
			menu(menu const & ref);
			~menu(void);

menu &		operator=(menu const & ref);

void		SetExit(bool const & ref);
bool		GetExit(void) const;

void		SetPause(bool const & ref);
bool		GetPause(void) const;

void		SetGame(bool const & ref);
bool		GetGame(void) const;

std::vector<std::string>	items_menu;
std::vector<std::string>	items_game;

void		SetChoise(size_t const & ref);
size_t		GetChoise(void) const;

void		SetEnter(bool const & ref);
bool		GetEnter(void) const;

void		SetLibNumber(size_t const & ref);
size_t		GetLibNumber(void) const;

void		SetDirectionPlayer0(unsigned const & direction);
t_move		GetDirectionPlayer0(void) const;

void		SetDirectionPlayer1(unsigned const & direction);
t_move		GetDirectionPlayer1(void) const;

void		SetLastScore0(unsigned const & ref);
unsigned	GetLastScore0(void) const;

void		SetLastScore1(unsigned const & ref);
unsigned	GetLastScore1(void) const;

	private:
bool		_exit;
bool		_pause;
bool		_game;

size_t		_choise;
bool		_enter;

t_move		_player0;
t_move		_player1;

unsigned	_score0;
unsigned	_score1;

size_t		_lib_number;

};

std::ostream &	operator<<(std::ostream & os, menu const & ref);

#endif