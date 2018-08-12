#include "obj.hpp"

/**************************************************************/

obj::obj(void)
{}

obj::obj(unsigned h, unsigned w, char s, bool rand) : _symbol(s)
{
	if (rand == true)
	{
		this->_pos.x = std::rand() % (w + 1) - 1;
		this->_pos.y = std::rand() % (h + 1) - 1;
	}
	else
	{
		this->_pos.x = w;
		this->_pos.y = h;
	}
}

obj::obj(obj const & ref)
{
	*this = ref;
}

obj::~obj(void) {}

obj &				obj::operator=(obj const & ref)
{
	this->_pos = ref._pos;
	this->_symbol = ref._symbol;

	if (this != &ref)
		*this = ref;
	return *this;
}

/**************************************************************/

void				obj::SetPosition(unsigned x, unsigned y)
{
	this->_pos.x = x;
	this->_pos.y = y;
}

t_position			obj::GetPosition(void) const
{
	return (this->_pos);
}

char				obj::GetSymbol(void) const
{
	return (this->_symbol);
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, obj const & ref)
{
	os << &ref;
	return os;
}
