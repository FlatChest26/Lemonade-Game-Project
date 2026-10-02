#include "lemonade_game.h"
#include "game_loop.h"
#include "raw_master.h"

using namespace std;

int main(int argc, const char* argv[])
{
	if (initialize(argc, argv))
	{
		raw_master::load_raws();
		new_game();
		run();
	}

	return EXIT_SUCCESS;
}