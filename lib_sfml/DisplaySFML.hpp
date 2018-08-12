#ifndef DISPLAY_SFML_HPP
#define DISPLAY_SFML_HPP

# include "ApiDisplay.hpp"

# include <SFML/System.hpp>
# include <SFML/Window.hpp>
# include <SFML/Graphics.hpp>

# define DELAY 		1

# define W_SCALE	10
# define H_SCALE	10

# define SIZE_BOARD	1
# define SIZE_INFO	10

# define BOARD_R	200
# define BOARD_G	200
# define BOARD_B	200

# define TEXT_R		255
# define TEXT_G		255
# define TEXT_B		0

# define PLAYER0_R	0
# define PLAYER0_G	0
# define PLAYER0_B	255

# define PLAYER1_R	255
# define PLAYER1_G	0
# define PLAYER1_B	0

# define FOOD_R		255
# define FOOD_G		0
# define FOOD_B		255

# define BONUS_R	0
# define BONUS_G	255
# define BONUS_B	0

class DisplaySFML : public ApiDisplay
{
	public:
					DisplaySFML(void);
					DisplaySFML(DisplaySFML const & ref);
virtual				~DisplaySFML(void);
DisplaySFML &		operator=(DisplaySFML const & ref);

unsigned			game_height;
unsigned			game_width;

unsigned			window_height;
unsigned			window_width;

virtual	void		InitDisplay(unsigned x, unsigned y);
virtual	void		ExitDisplay(void);

virtual	void		UpdateGame(menu *interface, std::vector<snake*> players, std::vector<obj*> foods, std::vector<obj*> bonus, std::vector<obj*> walls);
virtual	void		UpdateMenu(menu *interface, std::vector<snake*> players);

virtual int 		DelayDisplay(void);

	private:
sf::RenderWindow	_window;
sf::Event			_event;
sf::Font 			_font;

void				UpdateInfoGame(std::vector<snake*> players);
void				UpdateInfoMenu(menu *interface, std::vector<snake*> players);

void				ResetWindow(void);
void				DrawCross(sf::Vector2f position, sf::Color color);
void				DrawRect(sf::Vector2f position, sf::Color color);
void				DrawRectFull(sf::Vector2f position, sf::Color color);
void				DrawUnformed(sf::Vector2f position0, sf::Vector2f position1, sf::Color color);
void				DrawText(sf::Vector2f position, std::string str, unsigned size);
void				RenderPresent(void);

void				ExitButton(menu *interface);

};

std::ostream &	operator<<(std::ostream & os, DisplaySFML const & ref);

#endif