#include "ApiDisplay.hpp"

/**************************************************************/

ApiDisplay::ApiDisplay(void)
{}

ApiDisplay::ApiDisplay(ApiDisplay const & ref)
{
	*this = ref;
}

ApiDisplay::~ApiDisplay(void)
{}

ApiDisplay & 		ApiDisplay::operator=(ApiDisplay const & ref)
{
	if (this != &ref)
		*this = ref;
	return (*this);
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, ApiDisplay const & ref)
{
	os << &ref;
	return os;
}
