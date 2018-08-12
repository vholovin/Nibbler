#include "DisplaySDL.hpp"

extern "C"	ApiDisplay		*NewDisplay(void)
{
	return (new DisplaySDL());
}

/**************************************************************/

DisplaySDL::DisplaySDL(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
		exit(1);
	}
}

DisplaySDL::DisplaySDL(DisplaySDL const & ref)
{
	*this = ref;
}

DisplaySDL::~DisplaySDL(void)
{
	// SDL_Quit();
}

DisplaySDL &		DisplaySDL::operator=(DisplaySDL const & ref)
{
	this->game_height = ref.game_height;
	this->game_width = ref.game_width;

	this->window_height = ref.window_height;
	this->window_width = ref.window_width;

	this->_window = ref._window;
	this->_renderer = ref._renderer;

	this->_event = ref._event;
	this->_key_state = ref._key_state;

	this->_font = ref._font;
	this->_message = ref._message;
	this->_message_rect = ref._message_rect;

	if (this != &ref)
		*this = ref;
	return *this;
}

/**************************************************************/

void		DisplaySDL::InitDisplay(unsigned h, unsigned w)
{
	this->game_height = h;
	this->game_width = w;

	this->window_height = this->game_height + 3 * SIZE_BOARD + SIZE_INFO;
	this->window_width = this->game_width + 3 * SIZE_BOARD;

	this->_window = SDL_CreateWindow("nibbler-sdl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->window_width * W_SCALE, this->window_height * H_SCALE, SDL_WINDOW_SHOWN);
	if(this->_window == nullptr)
	{
		std::cout << "Failed to create window : " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (this->_renderer == nullptr)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}
	SDL_RenderSetLogicalSize(this->_renderer, this->window_width * W_SCALE, this->window_height * H_SCALE);

	TTF_Init();
	this->_font = TTF_OpenFont("./lib_sdl/Fontin-Regular.ttf", 18);
	if (this->_font == NULL)
    {
		std::cout << "Failed to create font" << std::endl;
        exit(1);
    }

   	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void		DisplaySDL::ExitDisplay(void)
{
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->_renderer);
	TTF_CloseFont(this->_font);

	SDL_DestroyTexture(this->_message);
	SDL_DestroyRenderer(this->_renderer);
	SDL_DestroyWindow(this->_window);

	TTF_Quit();
}

/**************************************************************/

void		DisplaySDL::UpdateGame(menu *interface, std::vector<snake*> players, std::vector<obj*> foods, std::vector<obj*> bonus, std::vector<obj*> walls)
{
	this->ResetWindow();

	if (SDL_PollEvent(&this->_event))
	{
		this->ExitButton(interface);

		if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_ESCAPE])
			interface->SetPause(true);
		
		if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_UP])
			interface->SetDirectionPlayer0(0);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_DOWN])
			interface->SetDirectionPlayer0(1);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_LEFT])
			interface->SetDirectionPlayer0(2);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_RIGHT])
			interface->SetDirectionPlayer0(3);

		if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_W])
			interface->SetDirectionPlayer1(0);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_S])
			interface->SetDirectionPlayer1(1);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_A])
			interface->SetDirectionPlayer1(2);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_D])
			interface->SetDirectionPlayer1(3);

		if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_1])
			interface->SetLibNumber(0);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_2])
			interface->SetLibNumber(1);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_3])
			interface->SetLibNumber(2);
	}

	for (size_t j = 0; j < players[0]->pos.size(); j++)
		this->DrawRect(players[0]->pos[j]->GetPosition().x + 2 * SIZE_BOARD, players[0]->pos[j]->GetPosition().y + 2 * SIZE_BOARD, (SDL_Color){PLAYER0_R, PLAYER0_G, PLAYER0_B, SDL_ALPHA_OPAQUE});

	if (players.size() > 1)
	{
		for (size_t j = 0; j < players[1]->pos.size(); j++)
			this->DrawCross(players[1]->pos[j]->GetPosition().x + 2 * SIZE_BOARD, players[1]->pos[j]->GetPosition().y + 2 * SIZE_BOARD, (SDL_Color){PLAYER1_R, PLAYER1_G, PLAYER1_B, SDL_ALPHA_OPAQUE});
	}

	for (size_t i = 0; i < walls.size(); i++)
		this->DrawRectFull(walls[i]->GetPosition().x + 2 * SIZE_BOARD, walls[i]->GetPosition().y + 2 * SIZE_BOARD, (SDL_Color){BOARD_R, BOARD_G, BOARD_B, SDL_ALPHA_OPAQUE});

	for (size_t i = 0; i < foods.size(); i++)
		this->DrawRectFull(foods[i]->GetPosition().x + 2 * SIZE_BOARD, foods[i]->GetPosition().y + 2 * SIZE_BOARD, (SDL_Color){FOOD_R, FOOD_G, FOOD_B, SDL_ALPHA_OPAQUE});

	for (size_t i = 0; i < bonus.size(); i++)
		this->DrawRect(bonus[i]->GetPosition().x + 2 * SIZE_BOARD, bonus[i]->GetPosition().y + 2 * SIZE_BOARD, (SDL_Color){ BONUS_R, BONUS_G, BONUS_B, SDL_ALPHA_OPAQUE});

	this->UpdateInfoGame(players);

	this->RenderPresent();
}

void		DisplaySDL::UpdateMenu(menu *interface, std::vector<snake*> players)
{
	this->ResetWindow();

	while (SDL_PollEvent(&this->_event))
	{
		this->ExitButton(interface);

		if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_RETURN])
			interface->SetEnter(true);
		
		if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_UP])
		{
			interface->SetEnter(false);
			if (interface->GetChoise() != 0)
				interface->SetChoise(interface->GetChoise() - 1);
		}
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_DOWN])
		{
			interface->SetEnter(false);
			if (interface->GetChoise() != 2)
				interface->SetChoise(interface->GetChoise() + 1);
		}

		if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_1])
			interface->SetLibNumber(0);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_2])
			interface->SetLibNumber(1);
		else if (SDL_KEYDOWN && this->_key_state[SDL_SCANCODE_3])
			interface->SetLibNumber(2);
	}

	this->DrawRectFull(this->game_width / 2 - 10, this->game_height / 2 - 4 + (5 * interface->GetChoise()), (SDL_Color){BONUS_R, BONUS_G, BONUS_B, SDL_ALPHA_OPAQUE});

	if (interface->GetGame())
	{
		this->GetTextAndRect(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 0), interface->items_game[0].c_str(), &this->_message, &this->_message_rect);
		SDL_RenderCopy(this->_renderer, this->_message, NULL, &this->_message_rect);

		this->GetTextAndRect(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 1), interface->items_game[1].c_str(), &this->_message, &this->_message_rect);
		SDL_RenderCopy(this->_renderer, this->_message, NULL, &this->_message_rect);

		this->GetTextAndRect(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 2), interface->items_game[2].c_str(), &this->_message, &this->_message_rect);
		SDL_RenderCopy(this->_renderer, this->_message, NULL, &this->_message_rect);
	}
	else
	{
		this->GetTextAndRect(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 0), interface->items_menu[0].c_str(), &this->_message, &this->_message_rect);
		SDL_RenderCopy(this->_renderer, this->_message, NULL, &this->_message_rect);

		this->GetTextAndRect(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 1), interface->items_menu[1].c_str(), &this->_message, &this->_message_rect);
		SDL_RenderCopy(this->_renderer, this->_message, NULL, &this->_message_rect);

		this->GetTextAndRect(this->game_width / 2 - 5, this->game_height / 2 - 5 + (5 * 2), interface->items_menu[2].c_str(), &this->_message, &this->_message_rect);
		SDL_RenderCopy(this->_renderer, this->_message, NULL, &this->_message_rect);
	}

	this->UpdateInfoMenu(interface, players);

	this->RenderPresent();
}

/**************************************************************/

void				DisplaySDL::UpdateInfoGame(std::vector<snake*> players)
{
	SDL_Texture *	player = NULL;
	SDL_Rect 		player_rect;

	std::string		info_str;
	SDL_Texture *	info = NULL;
	SDL_Rect 		info_rect;

	this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 10, "Player 1", &player, &player_rect);
	SDL_RenderCopy(this->_renderer, player, NULL, &player_rect);

	info_str = "Level: " + std::to_string(players[0]->GetLevel());
	this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 8, info_str.c_str(), &info, &info_rect);
	SDL_RenderCopy(this->_renderer, info, NULL, &info_rect);

	info_str = "Score: " + std::to_string(players[0]->GetScore());
	this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 6, info_str.c_str(), &info, &info_rect);
	SDL_RenderCopy(this->_renderer, info, NULL, &info_rect);

	info_str = "Length: " + std::to_string(players[0]->GetLen());
	this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 4, info_str.c_str(), &info, &info_rect);
	SDL_RenderCopy(this->_renderer, info, NULL, &info_rect);

	if (players.size() > 1)
	{
		this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 10, "Player 2", &player, &player_rect);
		SDL_RenderCopy(this->_renderer, player, NULL, &player_rect);

		info_str = "Level: " + std::to_string(players[1]->GetLevel());
		this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 8, info_str.c_str(), &info, &info_rect);
		SDL_RenderCopy(this->_renderer, info, NULL, &info_rect);

		info_str = "Score: " + std::to_string(players[1]->GetScore());
		this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 6, info_str.c_str(), &info, &info_rect);
		SDL_RenderCopy(this->_renderer, info, NULL, &info_rect);

		info_str = "Length: " + std::to_string(players[1]->GetLen());
		this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 4, info_str.c_str(), &info, &info_rect);
		SDL_RenderCopy(this->_renderer, info, NULL, &info_rect);
	}

	SDL_DestroyTexture(player);
	SDL_DestroyTexture(info);
}

void				DisplaySDL::UpdateInfoMenu(menu *interface, std::vector<snake*> players)
{
	SDL_Texture *	main = NULL;
	SDL_Rect 		main_rect;

	SDL_Texture *	player = NULL;
	SDL_Rect 		player_rect;

	std::string		score_str;
	SDL_Texture *	score = NULL;
	SDL_Rect 		score_rect;

	if (interface->GetGame() == true)
	{
		this->GetTextAndRect(this->window_width / 2 - 2.5, this->window_height - 10, "!PAUSE!", &main, &main_rect);
		SDL_RenderCopy(this->_renderer, main, NULL, &main_rect);

		this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 8, "Player 1", &player, &player_rect);
		SDL_RenderCopy(this->_renderer, player, NULL, &player_rect);

		score_str = "Score: " + std::to_string(players[0]->GetScore());
		this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 6, score_str.c_str(), &score, &score_rect);
		SDL_RenderCopy(this->_renderer, score, NULL, &score_rect);


		if (players.size() > 1)
		{
			this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 8, "Player 2", &player, &player_rect);
			SDL_RenderCopy(this->_renderer, player, NULL, &player_rect);

			score_str = "Score: " + std::to_string(players[1]->GetScore());
			this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 6, score_str.c_str(), &score, &score_rect);
			SDL_RenderCopy(this->_renderer, score, NULL, &score_rect);
		}
	}
	else
	{
		this->GetTextAndRect(this->window_width / 2 - 3.5, this->window_height - 10, "!NIBBLER!", &main, &main_rect);
		SDL_RenderCopy(this->_renderer, main, NULL, &main_rect);

		this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 8, "Player 1", &player, &player_rect);
		SDL_RenderCopy(this->_renderer, player, NULL, &player_rect);

		score_str = "Last result: " + std::to_string(interface->GetLastScore0());
		this->GetTextAndRect(this->window_width / 6 + 0, this->window_height - 6, score_str.c_str(), &score, &score_rect);
		SDL_RenderCopy(this->_renderer, score, NULL, &score_rect);

		this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 8, "Player 2", &player, &player_rect);
		SDL_RenderCopy(this->_renderer, player, NULL, &player_rect);

		score_str = "Last result: " + std::to_string(interface->GetLastScore1());
		this->GetTextAndRect(this->window_width - this->window_width / 3 + 0, this->window_height - 6, score_str.c_str(), &score, &score_rect);
		SDL_RenderCopy(this->_renderer, score, NULL, &score_rect);
	}

	SDL_DestroyTexture(main);
	SDL_DestroyTexture(player);
	SDL_DestroyTexture(score);
}

int 				DisplaySDL::DelayDisplay(void)
{
	return DELAY;
}

/**************************************************************/

void 				DisplaySDL::ResetWindow(void)
{
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
	SDL_RenderClear(this->_renderer);
	this->_key_state = SDL_GetKeyboardState(NULL);

	for (size_t i = 0; i < this->window_width; i++)
	{
		DrawRectFull(i, 0, (SDL_Color){BOARD_R, BOARD_G, BOARD_B, SDL_ALPHA_OPAQUE});
		DrawRectFull(i, this->window_height - SIZE_BOARD - SIZE_INFO, (SDL_Color){BOARD_R, BOARD_G, BOARD_B, SDL_ALPHA_OPAQUE});
		DrawRectFull(i, this->window_height - SIZE_BOARD, (SDL_Color){BOARD_R, BOARD_G, BOARD_B, SDL_ALPHA_OPAQUE});
	}
	for (size_t i = 0; i < this->window_height; i++)
	{
		DrawRectFull(0, i, (SDL_Color){BOARD_R, BOARD_G, BOARD_B, SDL_ALPHA_OPAQUE});
		DrawRectFull(this->window_width - SIZE_BOARD, i, (SDL_Color){BOARD_R, BOARD_G, BOARD_B, SDL_ALPHA_OPAQUE});
	}
}

void				DisplaySDL::DrawCross(unsigned x, unsigned y, SDL_Color color)
{
	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(this->_renderer, x * W_SCALE, y * H_SCALE, (x + 1) * W_SCALE, (y + 1) * H_SCALE);
	SDL_RenderDrawLine(this->_renderer, (x + 1) * W_SCALE, y * H_SCALE, x * W_SCALE, (y + 1) * H_SCALE);
}

void				DisplaySDL::DrawRect(unsigned x, unsigned y, SDL_Color color)
{
	SDL_Rect rect;

	rect.x = x * W_SCALE;
	rect.y = y * H_SCALE;
	rect.w = W_SCALE;
	rect.h = H_SCALE;
	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(this->_renderer, &rect);
}

void				DisplaySDL::DrawRectFull(unsigned x, unsigned y, SDL_Color color)
{
	SDL_Rect rect;

	rect.x = x * W_SCALE;
	rect.y = y * H_SCALE;
	rect.w = W_SCALE;
	rect.h = H_SCALE;
	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(this->_renderer, &rect);
}

void				DisplaySDL::GetTextAndRect(unsigned x, unsigned y, const char *text, SDL_Texture **texture, SDL_Rect *rect)
{
	int text_width;
	int text_height;

	SDL_Surface *surface;
	SDL_Color Color = {TEXT_R, TEXT_G, TEXT_B, SDL_ALPHA_OPAQUE};

	surface = TTF_RenderText_Solid(this->_font, text, Color);
	*texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
    
    text_width = surface->w * W_SCALE / 10;
    text_height = surface->h * H_SCALE / 10;

    SDL_FreeSurface(surface);
    
    rect->x = x * W_SCALE;
    rect->y = y * H_SCALE;
    rect->w = text_width;
    rect->h = text_height;
}

void 				DisplaySDL::RenderPresent(void)
{
	SDL_RenderPresent(this->_renderer);
}

/**************************************************************/

void				DisplaySDL::ExitButton(menu *interface)
{
	if (this->_event.type == SDL_QUIT)
	{
		interface->SetPause(true);
		interface->SetExit(true);
	}
}

/**************************************************************/

std::ostream &		operator<<(std::ostream & os, DisplaySDL const & ref)
{
	os << &ref;
	return os;
}
