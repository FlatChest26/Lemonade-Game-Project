#include "player_input_handler.h"

#include "action.h"
#include "direction_action.h"
#include "game.h"

#include "world_state.h"

#include <SDL_keycode.h>
#include <map>
#include "transforms.h"

const std::map<SDL_KeyCode, Position> MOVE_KEYS
{
	{ SDLK_UP,		{0, -1, 0} },
	{ SDLK_DOWN,	{0, 1, 0} },
	{ SDLK_LEFT,	{-1, 0, 0} },
	{ SDLK_RIGHT,	{1, 0, 0} },
	{ SDLK_KP_1,	{-1, 1, 0} },
	{ SDLK_KP_2,	{0, 1, 0} },
	{ SDLK_KP_3,	{1, 1, 0} },
	{ SDLK_KP_4,	{-1, 0, 0} },
	{ SDLK_KP_6,	{1, 0, 0} },
	{ SDLK_KP_7,	{-1, -1, 0} },
	{ SDLK_KP_8,	{0, -1, 0} },
	{ SDLK_KP_9,	{1, -1, 0} }
};

InputResult PlayerInputHandler::ev_key_down(SDL_Event event)
{
	auto player = get_player();

	if (!player)
	{
		return InputResult();
	}

	// Don't add a new action if one is already being done.
	if (player->has_any_action())
	{
		return InputResult(InputResult::Type::Action);
	}

	auto scancode = event.key.keysym.scancode;
	auto sym = event.key.keysym.sym;
	auto mod = event.key.keysym.mod;

	if (mod & KMOD_SHIFT)
	{
		switch (sym)
		{
		case SDLK_PERIOD:
		{
			player->queue_action(std::make_unique<BumpAction>(player, (Coord)0, (Coord)0, (Coord)-1));

			return InputResult(InputResult::Type::Action);
		}
		case SDLK_COMMA:
		{
			player->queue_action(std::make_unique<BumpAction>(player, (Coord)0, (Coord)0, (Coord)1));

			return InputResult(InputResult::Type::Action);
		}
		}
	}

	switch (sym)
	{
	case SDLK_ESCAPE:
	{
		if (auto game = get_game())
		{
			game->stop_running();
		}

		break;
	}
	case SDLK_F1:
	{
		if (auto world_state = get_world_state())
		{
			world_state->set_run_speed(runspeed_flag::TURN_BASED);
		}

		break;
	}
	case SDLK_F2:
	{
		if (auto world_state = get_world_state())
		{
			world_state->set_run_speed(runspeed_flag::REAL_TIME);
		}

		break;
	}
	case SDLK_F3:
	{
		if (auto world_state = get_world_state())
		{
			world_state->set_run_speed(runspeed_flag::REAL_TIME_2X);
		}

		break;
	}
	case SDLK_F4:
	{
		if (auto world_state = get_world_state())
		{
			world_state->set_run_speed(runspeed_flag::REAL_TIME_4X);
		}

		break;
	}
	case SDLK_PERIOD: case SDLK_KP_5:
	{
		player->queue_action(std::make_unique<WaitAction>(TimeUnit::from_seconds(1.0), player));
		return InputResult(InputResult::Type::Action);
	}
	case SDLK_COMMA:
	{
		player->queue_action(std::make_unique<WaitAction>(TimeUnit::from_seconds(10.0), player));

		return InputResult(InputResult::Type::Action);
	}
	}

	for (auto [move_key, move_dir] : MOVE_KEYS)
	{
		if (sym == move_key)
		{
			Coord dz = 0;

			if (mod & KMOD_SHIFT)
			{
				dz = 1;
			}

			if (mod & KMOD_CTRL)
			{
				dz = -1;
			}

			player->queue_action(std::make_unique<BumpAction>(player, move_dir.x, move_dir.y, move_dir.z + dz));
			return InputResult(InputResult::Type::Action);
		}
	}

	return InputResult();
}