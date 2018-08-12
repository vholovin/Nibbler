#include "nibbler.hpp"

#include <iostream>

nibbler::nibbler(void)
{
	this->_time = 0;
	this->_delay = DELAY;
}

nibbler::nibbler(nibbler const & ref)
{
	*this = ref;
}

nibbler::~nibbler(void)
{
	this->FreeMemory();

	this->_display->ExitDisplay();
	delete this->_display;

	dlclose(this->_dl);
}

nibbler &		nibbler::operator=(nibbler const & ref)
{
	this->_interface = ref._interface;

	this->_time = ref._time;
	this->_delay = ref._delay;

	this->_dl = ref._dl;
	this->_display = ref._display;

	this->_foods = ref._foods;
	this->_bonus = ref._bonus;

	this->_players = ref._players;

	if (this != &ref)
		*this = ref;
	return *this;
}

/**************************************************************/

void			nibbler::Play(unsigned height, unsigned width, char *type)
{
	this->OpenLib(type);
	this->_display->InitDisplay(height, width);

	while(this->_interface.GetExit() == false)
	{
		this->_display->UpdateMenu(&this->_interface, this->_players);
		this->ChangeLib(this->_interface.GetLibNumber(), height, width);

		if (this->_interface.GetEnter() == true && this->_interface.GetChoise() == 2)
			this->_interface.SetExit(true);

		if (this->_interface.GetGame() == false)
		{
			if (this->_interface.GetEnter() == true && this->_interface.GetChoise() == 0)
			{
				this->_players.push_back(new snake(height, width, '0'));
				this->_interface.SetGame(true);
				this->_interface.SetPause(false);
				this->_interface.SetEnter(false);
			}
			else if (this->_interface.GetEnter() == true && this->_interface.GetChoise() == 1)
			{
				this->_players.push_back(new snake(height, width + 10, '0'));
				this->_players.push_back(new snake(height, width - 10, '1'));
				this->_interface.SetGame(true);
				this->_interface.SetPause(false);
				this->_interface.SetEnter(false);
			}
		}
		else
		{
			if (this->_interface.GetEnter() == true && this->_interface.GetChoise() == 0)
			{
				this->_interface.SetPause(false);
				this->_interface.SetEnter(false);
			}
			else if (this->_interface.GetEnter() == true && this->_interface.GetChoise() == 1)
				this->ResetGame();
		}

		while(this->_interface.GetPause() == false)
		{
			this->_display->UpdateGame(&this->_interface, this->_players, this->_foods, this->_bonus, this->_walls);

			if (this->_players.size() == 1)
			{
				if (this->_walls.size() < this->_players[0]->GetScore())
					this->CreateWalls(height, width);
			}
			else if (this->_players.size() == 2)
			{
				if (this->_walls.size() < (this->_players[0]->GetScore() + this->_players[1]->GetScore()))
					this->CreateWalls(height, width);
			}

			if(this->_time % (5000 / SPEED) == 0 || this->_foods.size() == 0)
				this->CreateFoods(height, width);

			if (this->_time % (12500 / SPEED) == 0 && !this->_bonus.size())
				this->CreateBonus(height, width);
			if (this->_time % (17500 / SPEED) == 0 && this->_bonus.size())
			{
				obj* buff = this->_bonus.back();
				delete buff;
				this->_bonus.pop_back();
			}

			if(this->_time % ((26 - this->_players[0]->GetLevel()) / SPEED) == 0)
				this->_players[0]->move(this->_interface.GetDirectionPlayer0());
			
			if (this->_players.size() > 1)
				if (this->_time % ((26 - this->_players[1]->GetLevel()) / SPEED) == 0)
					this->_players[1]->move(this->_interface.GetDirectionPlayer1());

			this->IntersectPlayers(height, width);

			this->_delay = DELAY * this->_display->DelayDisplay();
			if (this->_players.size() > 1)
			{
				if (this->_players[0]->GetLevel() > this->_players[1]->GetLevel())
					this->_delay *= (1 - 0.001 * 50 * this->_players[0]->GetLevel());
				else
					this->_delay *= (1 - 0.001 * 50 * this->_players[1]->GetLevel());	
			}
			else
				this->_delay *= (1 - 0.001 * 50 * this->_players[0]->GetLevel());

			usleep(this->_delay);
			this->_time++;
		}

		this->_interface.SetEnter(false);
	}
}

/**************************************************************/

void			nibbler::CreateWalls(unsigned h, unsigned w)
{
	obj* buff;
	bool coincidence = true;

	while (coincidence == true)
	{
		coincidence = false;
		this->_walls.push_back(new obj(h, w, ' ', true));

		/*with himself*/
		for (size_t k = 0; k < this->_walls.size() - 1; k++)
		{
			if (this->_walls.back()->GetPosition().x == this->_walls[k]->GetPosition().x
				&&	this->_walls.back()->GetPosition().y == this->_walls[k]->GetPosition().y)
			{
				buff = this->_walls.back();
				delete buff;
				this->_walls.pop_back();
				coincidence = true;
				break ;
			}
		}

		/*with foods*/
		for (size_t k = 0; k < this->_foods.size(); k++)
		{
			if (this->_walls.back()->GetPosition().x == this->_foods[k]->GetPosition().x
				&&	this->_walls.back()->GetPosition().y == this->_foods[k]->GetPosition().y)
			{
				buff = this->_walls.back();
				delete buff;
				this->_walls.pop_back();
				coincidence = true;
				break ;
			}
		}

		/*with players*/
		for (size_t i = 0; i < this->_players.size(); i++)
		{
			for (size_t j = 0; j < this->_players[i]->pos.size(); j++)
			{
				if (this->_walls.back()->GetPosition().x == this->_players[i]->pos[j]->GetPosition().x
					&& this->_walls.back()->GetPosition().y == this->_players[i]->pos[j]->GetPosition().y)
				{
					buff = this->_walls.back();
					delete buff;
					this->_walls.pop_back();
					coincidence = true;
					break ;
				}
			}
			if (coincidence == true)
				break ;
		}
	}		
}

void			nibbler::CreateFoods(unsigned h, unsigned w)
{
	obj* buff;
	bool coincidence = true;

	unsigned len = this->_players[0]->pos.size();
	if (this->_players.size() > 1)
		len += this->_players[1]->pos.size();

	if ( this->_foods.size() < ((h + 1) * (w + 1) - len + 1) )
	{
		while (coincidence == true)
		{
			coincidence = false;
			this->_foods.push_back(new obj(h, w, '*', true));

			/*with himself*/
			for (size_t k = 0; k < this->_foods.size() - 1; k++)
			{
				if (this->_foods.back()->GetPosition().x == this->_foods[k]->GetPosition().x
					&&	this->_foods.back()->GetPosition().y == this->_foods[k]->GetPosition().y)
				{
					buff = this->_foods.back();
					delete buff;
					this->_foods.pop_back();
					coincidence = true;
					break ;
				}
			}

			/*with players*/
			for (size_t i = 0; i < this->_players.size(); i++)
			{
				for (size_t j = 0; j < this->_players[i]->pos.size(); j++)
				{
					if (this->_foods.back()->GetPosition().x == this->_players[i]->pos[j]->GetPosition().x
						&& this->_foods.back()->GetPosition().y == this->_players[i]->pos[j]->GetPosition().y)
					{
						buff = this->_foods.back();
						delete buff;
						this->_foods.pop_back();
						coincidence = true;
						break ;
					}
				}
				if (coincidence == true)
					break ;
			}

			/*with walls*/
			for (size_t k = 0; k < this->_walls.size(); k++)
			{
				if (this->_foods.back()->GetPosition().x == this->_walls[k]->GetPosition().x
					&&	this->_foods.back()->GetPosition().y == this->_walls[k]->GetPosition().y)
				{
					buff = this->_foods.back();
					delete buff;
					this->_foods.pop_back();
					coincidence = true;
					break ;
				}
			}

		}
	}
}

void			nibbler::CreateBonus(unsigned h, unsigned w)
{
	obj* buff;
	bool coincidence = true;

	while (coincidence == true)
	{
		coincidence = false;
		this->_bonus.push_back(new obj(h, w, '+', true));

		/*with players*/
		for (size_t i = 0; i < this->_players.size(); i++)
		{
			for (size_t j = 0; j < this->_players[i]->pos.size(); j++)
			{
				if (this->_bonus.back()->GetPosition().x == this->_players[i]->pos[j]->GetPosition().x
					&& this->_bonus.back()->GetPosition().y == this->_players[i]->pos[j]->GetPosition().y)
				{
					buff = this->_bonus.back();
					delete buff;
					this->_bonus.pop_back();
					coincidence = true;
					break ;
				}
			}
			if (coincidence == true)
				break ;
		}

		/*with walls*/
		for (size_t k = 0; k < this->_walls.size(); k++)
		{
			if (this->_foods.back()->GetPosition().x == this->_walls[k]->GetPosition().x
				&&	this->_foods.back()->GetPosition().y == this->_walls[k]->GetPosition().y)
			{
				buff = this->_foods.back();
				delete buff;
				this->_foods.pop_back();
				coincidence = true;
				break ;
			}
		}

	}
}

void			nibbler::IntersectPlayers(unsigned h, unsigned w)
{
	for (size_t i = 0; i < this->_players.size(); i++)
	{
		int del = this->_players[i]->eat_foods(this->_foods);
		if (del != -1)
		{
			obj* buff;
			buff = this->_foods[del];
			delete buff;
			this->_foods.erase(this->_foods.begin() + del);
		}

		if (this->_bonus.size() != 0 && this->_players[i]->eat_bonus(this->_bonus))
		{
			obj* buff;
			buff = this->_bonus[this->_bonus.size() - 1];
			delete buff;
			this->_bonus.pop_back();
		}

		if (this->_players[i]->intersect_walls(this->_walls))
		{
			this->ResetGame();
			break;
		}

		if (this->_players[i]->pos[0]->GetPosition().x < (0 - 1)
			|| this->_players[i]->pos[0]->GetPosition().y < (0 - 1)
			|| this->_players[i]->pos[0]->GetPosition().x > ((int)w - 1)
			|| this->_players[i]->pos[0]->GetPosition().y > ((int)h - 1))
		{
			this->ResetGame();
			break;
		}

		for (size_t j = 2; j < this->_players[i]->pos.size(); j++)
		{
			if (this->_players[i]->pos[0]->GetPosition().x == this->_players[i]->pos[j]->GetPosition().x
				&& this->_players[i]->pos[0]->GetPosition().y == this->_players[i]->pos[j]->GetPosition().y)
			{
				this->ResetGame();
				break;
			}
		}

		if (this->_players.size() > 1)
		{
			size_t k = (i == 0) ? 1 : 0;
			for (size_t j = 0; j < this->_players[k]->pos.size(); j++)
			{
				if (this->_players[i]->pos[0]->GetPosition().x == this->_players[k]->pos[j]->GetPosition().x
					&& this->_players[i]->pos[0]->GetPosition().y == this->_players[k]->pos[j]->GetPosition().y)
				{
					this->ResetGame();
					break;
				}
			}
		}
	}
}

/**************************************************************/

void			nibbler::ResetGame(void)
{
	this->_time = 0;
	this->_delay = DELAY;

	this->_interface.SetLastScore0(this->_players[0]->GetScore());
	if (this->_players.size() > 1)
		this->_interface.SetLastScore1(this->_players[1]->GetScore());

	this->FreeMemory();

	this->_interface.SetEnter(false);
	this->_interface.SetPause(true);
	this->_interface.SetGame(false);
	this->_interface.SetChoise(0);
	this->_interface.SetDirectionPlayer0(0);
	this->_interface.SetDirectionPlayer1(0);
}

void			nibbler::FreeMemory(void)
{
	obj*	buff_1;
	snake*	buff_2;

	while (this->_walls.size() > 0)
	{
		buff_1 = this->_walls.back();
		delete buff_1;
		this->_walls.pop_back();
	}

	while (this->_foods.size() > 0)
	{
		buff_1 = this->_foods.back();
		delete buff_1;
		this->_foods.pop_back();
	}

	while (this->_bonus.size() > 0)
	{
		buff_1 = this->_bonus.back();
		delete buff_1;
		this->_bonus.pop_back();
	}

	while (this->_players.size() > 0)
	{
		buff_2 = this->_players.back();
		delete buff_2;
		this->_players.pop_back();
	}
}

/**************************************************************/

void			nibbler::OpenLib(const char *name)
{
	ApiDisplay	*(*create)();


	if (strcmp("-ncurses", name) == 0)
	{
		this->_dl_number = 0;
		this->_dl = dlopen("./lib_ncurses/lib_ncurses.so", RTLD_LAZY | RTLD_LOCAL);
	}
	else if (strcmp("-sdl", name) == 0)
	{
		this->_dl_number = 1;
		this->_dl = dlopen("./lib_sdl/lib_sdl.so", RTLD_LAZY | RTLD_LOCAL);
	}
	else if (strcmp("-sfml", name) == 0)
	{
		this->_dl_number = 2;
		this->_dl = dlopen("./lib_sfml/lib_sfml.so", RTLD_LAZY | RTLD_LOCAL);
	}

	this->_interface.SetLibNumber(this->_dl_number);

	if (this->_dl == NULL)
	{
		std::cerr << "open_lib: dlopen : "<< dlerror() << std::endl;
		throw std::exception();
	}

	if (( create = reinterpret_cast<ApiDisplay* (*)()>(dlsym(this->_dl, "NewDisplay")) ) == NULL)
	{
		std::cerr << "open_lib: dlsym : " << dlerror() << std::endl;
		throw std::exception();
	}
	this->_display = create();
}

void 			nibbler::ChangeLib(size_t number, unsigned h, unsigned w)
{
	std::string name;

	if (this->_dl_number != number)
	{
		this->_display->ExitDisplay();
		delete this->_display;
		dlclose(this->_dl);
		if (number == 0)
			name = "-ncurses";
		else if (number == 1)
			name = "-sdl";
		else if (number == 2)
			name = "-sfml";
		this->OpenLib(name.c_str());
		this->_display->InitDisplay(h, w);
	}
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, nibbler const & ref)
{
	os << &ref;
	return os;
}
