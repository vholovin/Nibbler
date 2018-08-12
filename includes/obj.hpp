#ifndef OBJ_HPP
#define OBJ_HPP

#include <iostream>

typedef struct  s_position
{
	int			x;
	int			y;
}				t_position;

class obj
{
	public:
			obj(unsigned h, unsigned w, char s, bool rand);
			obj(obj const & ref);
			~obj(void);

obj &		operator=(obj const & ref);

void		SetPosition(unsigned x, unsigned y);

t_position	GetPosition(void) const;
char		GetSymbol(void) const;

	private:
			obj(void);

t_position	_pos;
char		_symbol;

};

std::ostream &	operator<<(std::ostream & os, obj const & ref);

#endif