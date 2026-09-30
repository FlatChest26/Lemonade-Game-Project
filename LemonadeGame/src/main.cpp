#include "lemonade_game.h"
#include "game_loop.h"

using namespace std;

int main( int argc, const char* argv[] )
{
	if ( initialize( argc, argv ) )
	{
		new_game();
		run();
	}

	return EXIT_SUCCESS;
}