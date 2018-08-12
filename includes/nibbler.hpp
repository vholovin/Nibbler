#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include <dlfcn.h>

#include <unistd.h>
#include <vector>

#include "ApiDisplay.hpp"

#include "menu.hpp"
#include "snake.hpp"
#include "obj.hpp"

#define	DELAY 1000
#define SPEED 4

class nibbler
{
	public:
		nibbler(void);
		nibbler(nibbler const & ref);
		~nibbler(void);

nibbler &			operator=(nibbler const & ref);

void 				Play(unsigned height, unsigned width, char *type);

	private:
menu				_interface;

unsigned long int	_time;
unsigned			_delay;

void*				_dl;
size_t				_dl_number;
ApiDisplay*			_display;

std::vector<obj*>	_walls;
std::vector<obj*> 	_foods;
std::vector<obj*>	_bonus;

std::vector<snake*>	_players;

void				OpenLib(const char *name);
void				ChangeLib(size_t number, unsigned h, unsigned w);

void				CreateWalls(unsigned h, unsigned w);
void				CreateFoods(unsigned h, unsigned w);
void 				CreateBonus(unsigned h, unsigned w);
void				IntersectPlayers(unsigned h, unsigned w);


void				ResetGame(void);
void				FreeMemory(void);

};

std::ostream &	operator<<(std::ostream & os, nibbler const & ref);

#endif