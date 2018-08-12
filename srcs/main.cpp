#include "nibbler.hpp"

int		main(int ac, char **av)
{
	nibbler		game;

	if (ac != 4)
	{
		std::cout << "\033[33mUsage:\033[39m ./nibbler <HEIGHT> <WIDTH> <TYPE_OF_GRAPH: -ncurses -sdl -sfml>" << std::endl;
		exit(0);
	}
	if (atoi(av[1]) < 10 || atoi(av[1]) > 100)
	{
		std::cout << "\033[33mAttention!\033[39m Height must be from " << 10 << " to " << 100 << std::endl;
		exit(0);
	}

	if (atoi(av[2]) < 40 || atoi(av[2]) > 100)
	{
		std::cout << "\033[33mAttention!\033[39m Width must be from " << 40 << " to " << 100 << std::endl;
		exit(0);
	}

	if (strcmp("-ncurses", av[3]) != 0 && strcmp("-sdl", av[3]) != 0 && strcmp("-sfml", av[3]) != 0)
	{
		std::cout << "\033[32mAvaliable libraries:\033[39m: -ncurses -sdl -sfml" << std::endl;
		exit(0);
	}

	try
	{
		game.Play(atoi(av[1]), atoi(av[2]), av[3]);
	}
	catch (std::exception & e)
	{
		exit(0);
	}

	return 0;
}