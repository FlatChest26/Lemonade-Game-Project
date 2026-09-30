#include "lemonade_game.h"
#include "game_loop.h"

#include <vector>
#include <string>

#include "game.h"
#include "world_state.h"

static constexpr const char* GAME_TITLE = "Lemonade Game";
static std::shared_ptr<Game> game;

void new_game()
{
	// Make new game
	game = std::make_shared<Game>();

	// Make new world state and add it to the game
	game->handle_input_result( InputResult { std::make_shared<WorldGameState>() } );

	// Start new game on world state
	auto world_state = std::dynamic_pointer_cast<WorldGameState>(game->get_current_game_state());
	if (world_state) world_state->new_game();
}

void render()
{
	if ( !game->should_update_screen )
	{
		return;
	}
	
	output::clear_console();

	game->render();
	game->should_update_screen = false;

	output::present();
	
}

void update()
{
	game->update();
}

void handle_events()
{
	SDL_Event event;

	while ( SDL_PollEvent( &event ) )
	{
		output::convert_event( event );

		switch ( event.type )
		{
		case SDL_QUIT:
		{
			game->stop_running();

			return;
		}
		case SDL_WINDOWEVENT:
		{
			if ( event.window.event == SDL_WINDOWEVENT_RESIZED )
			{
				output::update_window();
			}

			game->request_screen_update();
			if (game->get_current_game_state()->is_world_game_state())
			{
				auto world_state = std::dynamic_pointer_cast<WorldGameState>(game->get_current_game_state());
				if (world_state) world_state->update_camera();
			}

			break;
		}
		}

		game->handle_event(event);
	}
}

#include <libtcod/timer.h>

bool run()
{
	auto timer = tcod::Timer();

	game->is_running = true;

	while ( game->is_running )
	{
		// Make sure to sync the timer to the target FPS
		auto delta_time = timer.sync(60);

		// Game loop

		handle_events();
		update();
		render();
		
		// Update window title if needed
		if ( game->should_update_window_title )
		{
			if ( cfg::settings::SHOW_FPS ) 
			{
				output::set_window_title( (std::string) GAME_TITLE + " | FPS: " + std::to_string( timer.get_mean_fps() ) );
			}
			else
			{
				output::set_window_title( GAME_TITLE );
				game->should_update_window_title = false;
			}
		}
	}

	return false;
}