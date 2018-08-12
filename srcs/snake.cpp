#include "snake.hpp"

/**************************************************************/

snake::snake(void)
{}

snake::snake(unsigned h, unsigned w, char n) : _number(n)
{
	this->_score = 0;

	this->pos.push_back(new obj( h / 2 + 0, w / 2, this->_number, false));
	this->pos.push_back(new obj( h / 2 + 1, w / 2, this->_number, false));
	this->pos.push_back(new obj( h / 2 + 2, w / 2, this->_number, false));
	this->pos.push_back(new obj( h / 2 + 3, w / 2, this->_number, false));

	this->_move.up = true;
	this->_move.down = false;
	this->_move.left = false;
	this->_move.right = false;
}

snake::snake(snake const & ref)
{
	*this = ref;
}

snake::~snake(void)
{
	obj* buff;

	while (this->pos.size() > 0)
	{
		buff = this->pos[this->pos.size() - 1];
		delete buff;
		this->pos.pop_back();
	}
}

snake &				snake::operator=(snake const & ref)
{
	this->_move = ref._move;
	this->_score = ref._score;
	this->_number = ref._number;

	if (this != &ref)
		*this = ref;
	return *this;
}

/**************************************************************/

void				snake::move(t_move direction)
{
	if (direction.up == true && this->_move.down == false)
	{
			this->_move.up = true;
			this->_move.down = false;
			this->_move.left = false;
			this->_move.right = false;
	}
	else if (direction.down == true && this->_move.up == false)
	{
			this->_move.up = false;
			this->_move.down = true;
			this->_move.left = false;
			this->_move.right = false;
	}
	else if (direction.left == true && this->_move.right == false)
	{
			this->_move.up = false;
			this->_move.down = false;
			this->_move.left = true;
			this->_move.right = false;
	}
	else if (direction.right == true && this->_move.left == false)
	{
			this->_move.up = false;
			this->_move.down = false;
			this->_move.left = false;
			this->_move.right = true;
	}

	for (size_t i = pos.size() - 1; i > 0; i--)
		this->pos[i]->SetPosition(this->pos[i - 1]->GetPosition().x, this->pos[i - 1]->GetPosition().y);

	if (this->_move.up == true)
		this->pos[0]->SetPosition(this->pos[0]->GetPosition().x, this->pos[0]->GetPosition().y - 1);
	else if (this->_move.down == true)
		this->pos[0]->SetPosition(this->pos[0]->GetPosition().x, this->pos[0]->GetPosition().y + 1);
	else if (this->_move.left == true)
		this->pos[0]->SetPosition(this->pos[0]->GetPosition().x - 1, this->pos[0]->GetPosition().y);
	else if (this->_move.right == true)
		this->pos[0]->SetPosition(this->pos[0]->GetPosition().x + 1, this->pos[0]->GetPosition().y);
}

int			snake::eat_foods(std::vector<obj*> foods)
{
	for (size_t i = 0; i < foods.size(); i++)
	{
	 	if (foods[i]->GetPosition().x == this->pos[0]->GetPosition().x
	 			&& foods[i]->GetPosition().y == this->pos[0]->GetPosition().y)
	 	{
 			this->pos.push_back(new obj(this->pos[this->pos.size() - 1]->GetPosition().y, this->pos[this->pos.size() - 1]->GetPosition().x, this->_number, false));
	 		this->_score++;
	 		return (i);
	 	}
	}
	return (-1);
}

bool		snake::eat_bonus(std::vector<obj*> bonus)
{
	if (bonus[0]->GetPosition().x == this->pos[0]->GetPosition().x
			&& bonus[0]->GetPosition().y == this->pos[0]->GetPosition().y)
	{
		while (this->pos.size() > 3)
		{
			obj* buff = this->pos.back();
			delete buff;
			this->pos.pop_back();
		}
		this->_score += 10;
		return (true);
	}
	return (false);
}

bool		snake::intersect_walls(std::vector<obj*> walls)
{
	for (size_t i = 0; i < walls.size(); i++)
	{
		if (walls[i]->GetPosition().x == this->pos[0]->GetPosition().x
			&& walls[i]->GetPosition().y == this->pos[0]->GetPosition().y)
			return (true);
	}
	return (false);
}

/**************************************************************/

unsigned	snake::GetLevel(void) const
{
	unsigned level = ((this->_score / 10) < 25) ? this->_score / 10 : 25;
	return (level);
}

unsigned	snake::GetScore(void) const
{
	return (this->_score);
}

unsigned	snake::GetLen(void)	const
{
	return (this->pos.size());
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, snake const & ref)
{
	os << &ref;
	return os;
}
