#include "player_input_handler.h"

#include "action.h"
#include "direction_action.h"

#include "world_state.h"


std::weak_ptr<WorldGameState> PlayerInputHandler::get_world_state()
{
	if (auto game_state = get_game_state().lock())
	{
		if (game_state->is_world_game_state())
		{
			return std::dynamic_pointer_cast<WorldGameState>(game_state);
		}
	}

	return {};
}

std::shared_ptr<Entity> PlayerInputHandler::get_player()
{
	if (auto world_state = get_world_state().lock())
	{
		return world_state->get_player();
	}

	return nullptr;
}

std::shared_ptr<World> PlayerInputHandler::get_world()
{
	if (auto world_state = get_world_state().lock())
	{
		return world_state->get_world();
	}

	return nullptr;
}

std::shared_ptr<Camera> PlayerInputHandler::get_camera()
{
	if (auto world_state = get_world_state().lock())
	{
		return world_state->get_camera();
	}

	return nullptr;
}

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

	if (player->has_action())
		return InputResult(InputResult::Type::Action);

	auto scancode = event.key.keysym.scancode;
	auto sym = event.key.keysym.sym;
	auto mod = event.key.keysym.mod;

	
	if (mod & KMOD_SHIFT)
	{
		switch (sym)
		{
		case SDLK_PERIOD:
		{
			player->add_action(std::make_shared<BumpAction>(TimeUnit(0.0), player, (Coord) 0, (Coord) 0, (Coord) -1));

			return InputResult(InputResult::Type::Action);
		}
		case SDLK_COMMA:
		{
			player->add_action(std::make_shared<BumpAction>(TimeUnit(0.0), player, (Coord) 0, (Coord) 0, (Coord) 1));

			return InputResult(InputResult::Type::Action);
		}
		}
	}

	switch (sym)
	{
	case SDLK_F1:
	{
		COUT(player->pos());

		break;
	}
	case SDLK_PERIOD: case SDLK_KP_5:
	{
		player->add_action(std::make_shared<WaitAction>(TimeUnit(1.0), player));
		return InputResult(InputResult::Type::Action);
	}
	case SDLK_COMMA:
	{
		player->add_action(std::make_shared<WaitAction>(TimeUnit(10.0), player));

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

			player->add_action(std::make_shared<BumpAction>(TimeUnit(0.0), player, move_dir.x, move_dir.y, move_dir.z + dz));
			return InputResult(InputResult::Type::Action);
		}
	}

	return InputResult();
}
