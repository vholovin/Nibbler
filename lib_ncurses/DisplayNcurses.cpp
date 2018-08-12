#include "DisplayNcurses.hpp"

extern "C"	ApiDisplay		*NewDisplay(void)
{
	return (new DisplayNcurses());
}

/**************************************************************/

DisplayNcurses::DisplayNcurses(void)
{}

DisplayNcurses::DisplayNcurses(DisplayNcurses const & ref)
{
	*this = ref;
}

DisplayNcurses::~DisplayNcurses(void)
{
	delwin(this->_win);
	delwin(this->_game);
	delwin(this->_info);

	endwin();
}

DisplayNcurses &		DisplayNcurses::operator=(DisplayNcurses const & ref)
{
	this->height = ref.height;
	this->width = ref.width;

	this->_win = ref._win;
	this->_game = ref._game;
	this->_info = ref._info;

	if (this != &ref)
		*this = ref;
	return *this;
}

/**************************************************************/

void					DisplayNcurses::InitDisplay(unsigned h, unsigned w)
{
	this->height = h;
	this->width = w;

	initscr();
	noecho();
	refresh();

	curs_set(0);
	keypad(stdscr, true);

	if(!has_colors())
	{
		endwin();
		std::cout << "Terminal doesn't support color" << std::endl;
		exit(0);
	}

	start_color();
	init_pair(BOARD_PAIR, COLOR_RED, COLOR_WHITE);
	init_pair(TEXT_PAIR, COLOR_YELLOW, COLOR_BLACK);
	init_pair(FOOD_PAIR, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(BONUS_PAIR, COLOR_GREEN, COLOR_BLACK);
	init_pair(PLAYER0_PAIR, COLOR_BLUE, COLOR_BLACK);
	init_pair(PLAYER1_PAIR, COLOR_RED, COLOR_BLACK);

	this->SetWindow();
}

void				DisplayNcurses::ExitDisplay(void)
{
	refresh();

	wrefresh(this->_win);
	wrefresh(this->_game);
	wrefresh(this->_info);
}

/**************************************************************/

void				DisplayNcurses::SetWindow(void)
{
	this->_win = newwin(this->height + SIZE_INFO + SIZE_BOARD + 1, this->width + SIZE_BOARD + 1, 0, 0);
	this->_game = newwin(this->height + 1, this->width + 1, 1, 1);
	this->_info = newwin(SIZE_INFO - 1, this->width + 1, this->height + SIZE_BOARD + 1, 1);

	keypad(this->_win, true);
	keypad(this->_game, true);
	keypad(this->_info, true);

	nodelay(this->_win, true);
	nodelay(this->_game, true);
	nodelay(this->_info, true);

	wbkgd(this->_win,COLOR_PAIR(BOARD_PAIR));
	wattron(this->_win, COLOR_PAIR(BOARD_PAIR));

	box(this->_win, 0, 0);

	wmove(this->_win, this->height + SIZE_BOARD, 1);
	whline(this->_win, '*', this->width + 1);

	wrefresh(this->_win);
	wrefresh(this->_game);
	wrefresh(this->_info);
}

/**************************************************************/

void				DisplayNcurses::UpdateGame(menu *interface, std::vector<snake*> players, std::vector<obj*> foods, std::vector<obj*> bonus, std::vector<obj*> walls)
{
	this->ClearGame();

	int get_key = std::tolower(wgetch(this->_win));
	
	switch(get_key)
	{
		case 27: /*key esc*/
			interface->SetPause(true);
			break;

		case KEY_UP:
			interface->SetDirectionPlayer0(0);
			break;
		case KEY_DOWN:
			interface->SetDirectionPlayer0(1);
			break;
		case KEY_LEFT:
			interface->SetDirectionPlayer0(2);
			break;
		case KEY_RIGHT:
			interface->SetDirectionPlayer0(3);
			break;
		
		case 'w':
			interface->SetDirectionPlayer1(0);
			break;
		case 's':
			interface->SetDirectionPlayer1(1);
			break;
		case 'a':
			interface->SetDirectionPlayer1(2);
			break;
		case 'd':
			interface->SetDirectionPlayer1(3);
			break;

		case '1':
			interface->SetLibNumber(0);
			break;
		case '2':
			interface->SetLibNumber(1);
			break;
		case '3':
			interface->SetLibNumber(2);
			break;

		default:
			break;
	}

	for (size_t i = 0; i < players.size(); i++)
	{
		size_t color = (i == 0) ? PLAYER0_PAIR : PLAYER1_PAIR;
		wattron(this->_game, COLOR_PAIR(color));		
		for (size_t j = 0; j < players[i]->pos.size(); j++)
			mvwaddch(this->_game, players[i]->pos[j]->GetPosition().y + 1, players[i]->pos[j]->GetPosition().x + 1, players[i]->pos[j]->GetSymbol());
		wattroff(this->_game, COLOR_PAIR(color));
	}

	wattron(this->_game, COLOR_PAIR(BOARD_PAIR));
	for (size_t i = 0; i < walls.size(); i++)
		mvwaddch(this->_game, walls[i]->GetPosition().y + 1, walls[i]->GetPosition().x + 1, walls[i]->GetSymbol());
	wattroff(this->_game, COLOR_PAIR(BOARD_PAIR));

	wattron(this->_game, COLOR_PAIR(FOOD_PAIR));
	for (size_t i = 0; i < foods.size(); i++)
		mvwaddch(this->_game, foods[i]->GetPosition().y + 1, foods[i]->GetPosition().x + 1, foods[i]->GetSymbol());
	wattroff(this->_game, COLOR_PAIR(FOOD_PAIR));

	wattron(this->_game, COLOR_PAIR(BONUS_PAIR));
	for (size_t i = 0; i < bonus.size(); i++)
		mvwaddch(this->_game, bonus[i]->GetPosition().y + 1, bonus[i]->GetPosition().x + 1, bonus[i]->GetSymbol());
	wattroff(this->_game, COLOR_PAIR(BONUS_PAIR));

	wrefresh(this->_game);

	this->UpdateInfoGame(players);
}

void				DisplayNcurses::UpdateMenu(menu *interface, std::vector<snake*> players)
{
	this->ClearGame();

	int get_key = std::tolower(wgetch(this->_win));

	for (size_t i = 0; i <= 2; i++)
	{
		if (i == interface->GetChoise())
		{
			wattron(this->_game, COLOR_PAIR(BONUS_PAIR));
			mvwaddch(this->_game, height / 2 - 5 + (5 * i), width / 2 - 10, '>');
			wattroff(this->_game, COLOR_PAIR(BONUS_PAIR));
		}
		else
			mvwaddch(this->_game, height / 2 - 5 + (5 * i), width / 2 - 10, ' ');

		wattron(this->_game, COLOR_PAIR(TEXT_PAIR));
		if (interface->GetGame())
			mvwaddstr(this->_game, height / 2 - 5 + (5 * i), width / 2 - 5, interface->items_game[i].c_str());			
		else
			mvwaddstr(this->_game, height / 2 - 5 + (5 * i), width / 2 - 5, interface->items_menu[i].c_str());
		wattroff(this->_game, COLOR_PAIR(TEXT_PAIR));
	}

	switch (get_key)
	{
		case 10: /*key enter*/
			interface->SetEnter(true);
		break;

		case KEY_UP:
		{
			interface->SetEnter(false);
			if (interface->GetChoise() != 0)
				interface->SetChoise(interface->GetChoise() - 1);
		}
		break;

		case KEY_DOWN:
		{
			interface->SetEnter(false);
			if (interface->GetChoise() != 2)
				interface->SetChoise(interface->GetChoise() + 1);
		}
		break;

		case '1':
			interface->SetLibNumber(0);
			break;
		case '2':
			interface->SetLibNumber(1);
			break;
		case '3':
			interface->SetLibNumber(2);
			break;

		default:
		break;
	}

	wrefresh(this->_game);

	this->UpdateInfoMenu(interface, players);
}

int 				DisplayNcurses::DelayDisplay(void)
{
	return DELAY;
}

/**************************************************************/

void				DisplayNcurses::UpdateInfoGame(std::vector<snake*> players)
{
	this->ClearInfo();
	wattron(this->_info, COLOR_PAIR(TEXT_PAIR));

	mvwaddstr(this->_info, 0, width / 6 + 0, "PLAYER_0");
	mvwaddstr(this->_info, 2, width / 6 + 0, "LEVEL: ");
	mvwprintw(this->_info, 2, width / 6 + 6, "%d", players[0]->GetLevel());
	mvwaddstr(this->_info, 3, width / 6 + 0, "SCORE: ");
	mvwprintw(this->_info, 3, width / 6 + 6, "%d", players[0]->GetScore());
	mvwaddstr(this->_info, 4, width / 6 + 0, "LEN: ");
	mvwprintw(this->_info, 4, width / 6 + 4, "%d", players[0]->GetLen());

	if (players.size() > 1)
	{
		mvwaddstr(this->_info, 0, width - width / 3 + 0, "PLAYER_1");
		mvwaddstr(this->_info, 2, width - width / 3 + 0, "LEVEL: ");
		mvwprintw(this->_info, 2, width - width / 3 + 6, "%d", players[1]->GetLevel());
		mvwaddstr(this->_info, 3, width - width / 3 + 0, "SCORE: ");
		mvwprintw(this->_info, 3, width - width / 3 + 6, "%d", players[1]->GetScore());
		mvwaddstr(this->_info, 4, width - width / 3 + 0, "LEN: ");
		mvwprintw(this->_info, 4, width - width / 3 + 4, "%d", players[1]->GetLen());
	}

	wattroff(this->_info, COLOR_PAIR(TEXT_PAIR));
	wrefresh(this->_info);
}

void				DisplayNcurses::UpdateInfoMenu(menu *interface, std::vector<snake*> players)
{
	this->ClearInfo();	
	wattron(this->_info, COLOR_PAIR(TEXT_PAIR));

	if (interface->GetGame() == true)
	{
		mvwaddstr(this->_info, 1, width / 2 - 3, "!PAUSE!");

		mvwaddstr(this->_info, 2, width / 6 + 0, "PLAYER_0");
		mvwaddstr(this->_info, 3, width / 6 + 0, "SCORE: ");
		mvwprintw(this->_info, 3, width / 6 + 6, "%d", players[0]->GetScore());
		if (players.size() > 1)
		{
			mvwaddstr(this->_info, 2, width - width / 3 + 0, "PLAYER_1");
			mvwaddstr(this->_info, 3, width - width / 3 + 0, "SCORE: ");
			mvwprintw(this->_info, 3, width - width / 3 + 6, "%d", players[1]->GetScore());
		}
	}
	else
	{
		mvwaddstr(this->_info, 1, width / 2 - 5, "!NIBBLER!");

		mvwaddstr(this->_info, 2, width / 6 + 0, "PLAYER_0");
		mvwaddstr(this->_info, 3, width / 6 + 0, "LAST_RES: ");
		mvwprintw(this->_info, 3, width / 6 + 9, "%d", interface->GetLastScore0());

		mvwaddstr(this->_info, 2, width - width / 3 + 0, "PLAYER_1");
		mvwaddstr(this->_info, 3, width - width / 3 + 0, "LAST_RES: ");
		mvwprintw(this->_info, 3, width - width / 3 + 9, "%d", interface->GetLastScore1());

	}

	wattroff(this->_info, COLOR_PAIR(TEXT_PAIR));
	wrefresh(this->_info);
}

/**************************************************************/

void				DisplayNcurses::ClearGame(void)
{
	for (size_t i = 0; i <= height; i++)
		for (size_t j = 0; j <= width; j++)
			mvwaddch(this->_game, i, j, ' ');
}

void				DisplayNcurses::ClearInfo(void)
{
	for (size_t i = 0; i <= (SIZE_INFO - SIZE_BOARD); i++)
		for (size_t j = 0; j <= width; j++)
			mvwaddch(this->_info, i, j, ' ');
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, DisplayNcurses const & ref)
{
	os << &ref;
	return os;
}
