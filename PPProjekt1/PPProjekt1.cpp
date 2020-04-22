// PPProjekt1.cpp : Defines the entry point for the console application.
//
#include "Game.h"
#include <cstdlib>
#include <time.h>

int main()
{
	srand(time(NULL));
	Game game("Krzysztof Dubanowicz 172029");

	while(game.isRunning())
	{
		game.render();
		game.update();
	}
    return 0;
}

