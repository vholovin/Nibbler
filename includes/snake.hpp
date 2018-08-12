#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "obj.hpp"

#include <vector>

typedef struct 	s_move
{
	bool		up;
	bool		down;
	bool		left;
	bool		right;
}				t_move;

class snake
{
	public:
			snake(unsigned h, unsigned w, char n);
			snake(snake const & ref);
			~snake(void);

snake &		operator=(snake const & ref);

void		move(t_move direction);
int 		eat_foods(std::vector<obj*> foods);
bool		eat_bonus(std::vector<obj*> bonus);
bool		intersect_walls(std::vector<obj*> walls);

unsigned	GetLevel(void) const;
unsigned	GetScore(void) const;
unsigned	GetLen(void) const;

std::vector<obj*> 	pos;

	private:
			snake(void);
t_move		_move;
unsigned	_score;
char		_number;

};

std::ostream &	operator<<(std::ostream & os, snake const & ref);

#endif