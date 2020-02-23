#include <SDL.h>
#include "Game.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

int main(int argc, char * argv[])
{
	Game game;
	bool check = game.init();
	if (check)
	{
		game.runGame();
	}
	game.endGame();

	return 0;
}