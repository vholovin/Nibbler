#include "menu.hpp"

/**************************************************************/

menu::menu(void)
{
	this->_exit = false;
	this->_pause = true;
	this->_game = false;

	this->_choise = 0;
	this->_enter = false;

	this->items_menu.push_back("1 Player");
	this->items_menu.push_back("2 Players");
	this->items_menu.push_back("Exit");

	this->items_game.push_back("Continue");
	this->items_game.push_back("Restart");
	this->items_game.push_back("Exit");

	this->_score0 = 0;
	this->_score1 = 0;

	this->SetDirectionPlayer0(0);
	this->SetDirectionPlayer1(0);

	this->_lib_number = -1;
}


menu::menu(menu const & ref)
{
	*this = ref;
}

menu::~menu(void) {}

menu &				menu::operator=(menu const & ref)
{
	this->items_menu = ref.items_menu;
	this->items_game = ref.items_game;

	this->_exit = ref._exit;
	this->_pause = ref._pause;
	this->_game = ref._game;

	this->_choise = ref._choise;
	this->_enter = ref._enter;

	this->_player0 = ref._player0;
	this->_player1 = ref._player1;

	this->_score0 = ref._score0;
	this->_score1 = ref._score1;

	this->_lib_number = ref._lib_number;

	if (this != &ref)
		*this = ref;
	return *this;
}

/**************************************************************/

void		menu::SetExit(bool const & ref)
{
	this->_exit = ref;
}

bool		menu::GetExit(void) const
{
	return (this->_exit);
}

void		menu::SetPause(bool const & ref)
{
	this->_pause = ref;
}

bool		menu::GetPause(void) const
{
	return (this->_pause);
}

void		menu::SetGame(bool const & ref)
{
	this->_game = ref;
}

bool		menu::GetGame(void) const
{
	return (this->_game);
}

void		menu::SetChoise(size_t const & ref)
{
	this->_choise = ref;
}

size_t		menu::GetChoise(void) const
{
	return (this->_choise);
}

void		menu::SetEnter(bool const & ref)
{
	this->_enter = ref;
}

bool		menu::GetEnter(void) const
{
	return (this->_enter);
}

void		menu::SetLibNumber(size_t const & ref)
{
	this->_lib_number = ref;
}

size_t		menu::GetLibNumber(void) const
{
	return (this->_lib_number);
}

void		menu::SetLastScore0(unsigned const & ref)
{
	this->_score0 = ref;
}

unsigned	menu::GetLastScore0(void) const
{
	return (this->_score0);
}

void		menu::SetLastScore1(unsigned const & ref)
{
	this->_score1 = ref;
}

unsigned	menu::GetLastScore1(void) const
{
	return (this->_score1);
}

void		menu::SetDirectionPlayer0(unsigned const & direction)
{
	this->_player0.up = (direction == 0) ? true : false;
	this->_player0.down = (direction == 1) ? true : false;
	this->_player0.left = (direction == 2) ? true : false;
	this->_player0.right = (direction == 3) ? true : false;
}

void		menu::SetDirectionPlayer1(unsigned const & direction)
{
	this->_player1.up = (direction == 0) ? true : false;
	this->_player1.down = (direction == 1) ? true : false;
	this->_player1.left = (direction == 2) ? true : false;
	this->_player1.right = (direction == 3) ? true : false;
}

t_move		menu::GetDirectionPlayer0(void) const
{
	return(this->_player0);	
}

t_move		menu::GetDirectionPlayer1(void) const
{
	return(this->_player1);	
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, menu const & ref)
{
	os << &ref;
	return os;
}
