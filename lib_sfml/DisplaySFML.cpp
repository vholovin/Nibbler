#include "DisplaySFML.hpp"

extern "C"	ApiDisplay		*NewDisplay(void)
{
	return (new DisplaySFML());
}

/**************************************************************/

DisplaySFML::DisplaySFML(void)
{}

DisplaySFML::DisplaySFML(DisplaySFML const & ref)
{
	*this = ref;
}

DisplaySFML::~DisplaySFML(void)
{
}

DisplaySFML &		DisplaySFML::operator=(DisplaySFML const & ref)
{
	this->game_height = ref.game_height;
	this->game_width = ref.game_width;

	this->window_height = ref.window_height;
	this->window_width = ref.window_width;

	if (this != &ref)
		*this = ref;
	return *this;
}

/**************************************************************/

void				DisplaySFML::InitDisplay(unsigned h, unsigned w)
{
	this->game_height = h;
	this->game_width = w;

	this->window_height = this->game_height + 3 * SIZE_BOARD + SIZE_INFO;
	this->window_width = this->game_width + 3 * SIZE_BOARD;

	this->_window.create(sf::VideoMode(this->window_width * W_SCALE, this->window_height * H_SCALE), "nibbler-sfml", sf::Style::Default);

	if (!this->_font.loadFromFile("./lib_sfml/Fontin-Regular.ttf"))
	{
		std::cout << "Failed to create font" << std::endl;
		exit(1);
	}
	this->_window.setKeyRepeatEnabled(true);
}

void				DisplaySFML::ExitDisplay(void)
{
	this->_window.clear(sf::Color::Black);
	this->_window.setVisible(false);
	this->_window.close();
}

/**************************************************************/

void				DisplaySFML::UpdateGame(menu *interface, std::vector<snake*> players, std::vector<obj*> foods, std::vector<obj*> bonus, std::vector<obj*> walls)
{
	if (this->_window.isOpen())
	{
		this->ResetWindow();

		while (this->_window.pollEvent(this->_event))
		{
			this->ExitButton(interface);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				interface->SetPause(true);
		
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				interface->SetDirectionPlayer0(0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				interface->SetDirectionPlayer0(1);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				interface->SetDirectionPlayer0(2);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				interface->SetDirectionPlayer0(3);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				interface->SetDirectionPlayer1(0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				interface->SetDirectionPlayer1(1);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				interface->SetDirectionPlayer1(2);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				interface->SetDirectionPlayer1(3);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
				interface->SetLibNumber(0);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
				interface->SetLibNumber(1);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
				interface->SetLibNumber(2);
		}

		for (size_t j = 0; j < players[0]->pos.size(); j++)
			this->DrawRect(sf::Vector2f(players[0]->pos[j]->GetPosition().x + 2 * SIZE_BOARD, players[0]->pos[j]->GetPosition().y + 2 * SIZE_BOARD), sf::Color(PLAYER0_R, PLAYER0_G, PLAYER0_B));

		if (players.size() > 1)
		{
			for (size_t j = 0; j < players[1]->pos.size(); j++)
				this->DrawCross(sf::Vector2f(players[1]->pos[j]->GetPosition().x + 2 * SIZE_BOARD, players[1]->pos[j]->GetPosition().y + 2 * SIZE_BOARD), sf::Color(PLAYER1_R, PLAYER1_G, PLAYER1_B));
		}
		for (size_t i = 0; i < walls.size(); i++)
			this->DrawRectFull(sf::Vector2f(walls[i]->GetPosition().x + 2 * SIZE_BOARD, walls[i]->GetPosition().y + 2 * SIZE_BOARD), sf::Color(BOARD_R, BOARD_G, BOARD_B));

		for (size_t i = 0; i < foods.size(); i++)
			this->DrawRectFull(sf::Vector2f(foods[i]->GetPosition().x + 2 * SIZE_BOARD, foods[i]->GetPosition().y + 2 * SIZE_BOARD), sf::Color(FOOD_R, FOOD_G, FOOD_B));

		for (size_t i = 0; i < bonus.size(); i++)
			this->DrawRect(sf::Vector2f(bonus[i]->GetPosition().x + 2 * SIZE_BOARD, bonus[i]->GetPosition().y + 2 * SIZE_BOARD), sf::Color(BONUS_R, BONUS_G, BONUS_B));

		this->UpdateInfoGame(players);

		this->RenderPresent();
	}
}

void				DisplaySFML::UpdateMenu(menu *interface, std::vector<snake*> players)
{
	if (this->_window.isOpen())
	{
		this->ResetWindow();

		if (this->_window.pollEvent(this->_event))
		{
			this->ExitButton(interface);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				interface->SetEnter(true);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				interface->SetEnter(false);
				if (interface->GetChoise() != 0)
					interface->SetChoise(interface->GetChoise() - 1);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				interface->SetEnter(false);
				if (interface->GetChoise() != 2)
					interface->SetChoise(interface->GetChoise() + 1);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
				interface->SetLibNumber(0);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
				interface->SetLibNumber(1);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
				interface->SetLibNumber(2);
		}

		this->DrawRectFull(sf::Vector2f(this->game_width / 2 - 10, this->game_height / 2 - 4 + (5 * interface->GetChoise())), sf::Color(BONUS_R, BONUS_G, BONUS_B));

		if (interface->GetGame())
		{
			this->DrawText(sf::Vector2f(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 0)), interface->items_game[0], 24);
			this->DrawText(sf::Vector2f(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 1)), interface->items_game[1], 24);
			this->DrawText(sf::Vector2f(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 2)), interface->items_game[2], 24);
		}
		else
		{
			this->DrawText(sf::Vector2f(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 0)), interface->items_menu[0], 24);
			this->DrawText(sf::Vector2f(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 1)), interface->items_menu[1], 24);
			this->DrawText(sf::Vector2f(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 2)), interface->items_menu[2], 24);
		}

		this->UpdateInfoMenu(interface, players);

		this->RenderPresent();
		sf::sleep(sf::milliseconds(50));
	}
}

int 				DisplaySFML::DelayDisplay(void)
{
	return DELAY;
}

/**************************************************************/

void				DisplaySFML::UpdateInfoGame(std::vector<snake*> players)
{
	this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 10), "Player 1", 18);
	this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 8), "Level: " + std::to_string(players[0]->GetLevel()), 18);
	this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 6), "Score: " + std::to_string(players[0]->GetScore()), 18);
	this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 4), "Lenght: " + std::to_string(players[0]->GetLen()), 18);

	if (players.size() > 1)
	{
		this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 10), "Player 2", 18);
		this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 8), "Level: " + std::to_string(players[1]->GetLevel()), 18);
		this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 6), "Score: " + std::to_string(players[1]->GetScore()), 18);
		this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 4), "Lenght: " + std::to_string(players[1]->GetLen()), 18);
	}
}

void				DisplaySFML::UpdateInfoMenu(menu *interface, std::vector<snake*> players)
{
	if (interface->GetGame() == true)
	{
		this->DrawText(sf::Vector2f(this->window_width / 2 - 2.5, this->window_height - 10), "!PAUSE!", 18);

		this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 8), "Player 1", 18);
		this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 6), "Score: " + std::to_string(players[0]->GetScore()), 18);

		if (players.size() > 1)
		{
			this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 8), "Player 2", 18);
			this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 6), "Score: " + std::to_string(players[1]->GetScore()), 18);
		}
	}
	else
	{
		this->DrawText(sf::Vector2f(this->window_width / 2 - 3.5, this->window_height - 10), "!NIBBLER!", 18);

		this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 8), "Player 1", 18);
		this->DrawText(sf::Vector2f(this->window_width / 6 + 0, this->window_height - 6), "Last result: " + std::to_string(interface->GetLastScore0()), 18);

		this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 8), "Player 2", 18);
		this->DrawText(sf::Vector2f(this->window_width - this->window_width / 3 + 0, this->window_height - 6), "Last result: " + std::to_string(interface->GetLastScore1()), 18);
	}
}

/**************************************************************/

void 				DisplaySFML::ResetWindow(void)
{
	this->_window.clear(sf::Color::Black);

	this->DrawUnformed(sf::Vector2f(0, 0), sf::Vector2f(this->window_width, 1), sf::Color(BOARD_R, BOARD_G, BOARD_B));
	this->DrawUnformed(sf::Vector2f(0, this->window_height - SIZE_BOARD - SIZE_INFO), sf::Vector2f(this->window_width, this->window_height - SIZE_INFO), sf::Color(BOARD_R, BOARD_G, BOARD_B));
	this->DrawUnformed(sf::Vector2f(0, this->window_height - SIZE_BOARD), sf::Vector2f(this->window_width, this->window_height), sf::Color(BOARD_R, BOARD_G, BOARD_B));

	this->DrawUnformed(sf::Vector2f(0, 0), sf::Vector2f(1, this->window_height), sf::Color(BOARD_R, BOARD_G, BOARD_B));
	this->DrawUnformed(sf::Vector2f(this->window_width - SIZE_BOARD, 0), sf::Vector2f(this->window_width, this->window_height), sf::Color(BOARD_R, BOARD_G, BOARD_B));
}

void 				DisplaySFML::DrawCross(sf::Vector2f position, sf::Color color)
{
	sf::Vertex line0[2];

	line0[0].position = sf::Vector2f(position.x * W_SCALE, position.y * H_SCALE);
	line0[0].color  = color;

	line0[1].position = sf::Vector2f(position.x * W_SCALE + H_SCALE, position.y * H_SCALE + W_SCALE);
	line0[1].color = color;

	this->_window.draw(line0, 2, sf::Lines);


	sf::Vertex line1[2];

	line1[0].position = sf::Vector2f(position.x * W_SCALE, position.y * H_SCALE + H_SCALE);
	line1[0].color  = color;

	line1[1].position = sf::Vector2f(position.x * W_SCALE + W_SCALE, position.y * H_SCALE);
	line1[1].color = color;

	this->_window.draw(line1, 2, sf::Lines);
}

void				DisplaySFML::DrawRect(sf::Vector2f position, sf::Color color)
{
	sf::ConvexShape rect;
	unsigned		size; 

	size = SIZE_BOARD * 2;
	position.x = position.x * W_SCALE;
	position.y = position.y * H_SCALE;

	rect.setPointCount(4);

	rect.setPoint(0, sf::Vector2f(position.x + size, position.y + size));
	rect.setPoint(1, sf::Vector2f(position.x + W_SCALE - size, position.y + size));
	rect.setPoint(2, sf::Vector2f(position.x + W_SCALE - size, position.y + H_SCALE - size));
	rect.setPoint(3, sf::Vector2f(position.x + size, position.y + H_SCALE - size));

	rect.setFillColor(sf::Color::Black);

	rect.setOutlineThickness(size);
	rect.setOutlineColor(color);

	this->_window.draw(rect);
}

void				DisplaySFML::DrawRectFull(sf::Vector2f position, sf::Color color)
{
	sf::ConvexShape rect;

	position.x *= W_SCALE;
	position.y *= H_SCALE;

	rect.setPointCount(4);

	rect.setPoint(0, sf::Vector2f(position.x, position.y));
	rect.setPoint(1, sf::Vector2f(position.x + W_SCALE, position.y));
	rect.setPoint(2, sf::Vector2f(position.x + W_SCALE, position.y + H_SCALE));
	rect.setPoint(3, sf::Vector2f(position.x, position.y + H_SCALE));

	rect.setFillColor(color);

	this->_window.draw(rect);
}

void				DisplaySFML::DrawUnformed(sf::Vector2f position0, sf::Vector2f position1, sf::Color color)
{
	sf::ConvexShape rect;

	position0.x *= W_SCALE;
	position0.y *= H_SCALE;

	position1.x *= W_SCALE;
	position1.y *= H_SCALE;

	rect.setPointCount(4);

	rect.setPoint(0, sf::Vector2f(position0.x, position0.y));
	rect.setPoint(1, sf::Vector2f(position1.x, position0.y));
	rect.setPoint(2, sf::Vector2f(position1.x, position1.y));
	rect.setPoint(3, sf::Vector2f(position0.x, position1.y));

	rect.setFillColor(color);

	this->_window.draw(rect);
}

void				DisplaySFML::DrawText(sf::Vector2f position, std::string str, unsigned size)
{
	sf::Text text;

	position.x *= W_SCALE;
	position.y *= H_SCALE;

	text.setFont(this->_font);
	text.setString(str);
	text.setCharacterSize(size * W_SCALE / 10);
	text.setFillColor(sf::Color(TEXT_R, TEXT_G, TEXT_B));
	text.setPosition(position);

	this->_window.draw(text);
}

void 				DisplaySFML::RenderPresent(void)
{
	this->_window.display();
}

/**************************************************************/

void 				DisplaySFML::ExitButton(menu *interface)
{
	if (this->_event.type == sf::Event::Closed)
	{
		interface->SetPause(true);
		interface->SetExit(true);
	}	
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, DisplaySFML const & ref)
{
	os << &ref;
	return os;
}
