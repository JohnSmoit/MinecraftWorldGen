#include "Game.h"

int main() 
{

	srand(time(NULL));
	Game* game = new Game();
	return game->Run();
}