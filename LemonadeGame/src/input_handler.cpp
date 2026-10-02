#include "input_handler.h"

#include "game_states.h"
#include "game.h"
#include "world_state.h"

GameState* InputHandler::get_game_state()
{
	return m_game_state;
}

WorldGameState* InputHandler::get_world_state()
{
	if (auto game_state = get_game_state())
	{
		if (game_state->is_world_game_state())
		{
			return game_state->as_world_game_state();
		}
	}

	return {};
}

Entity* InputHandler::get_player()
{
	if (auto world_state = get_world_state())
	{
		return world_state->get_player();
	}

	return nullptr;
}

World* InputHandler::get_world()
{
	if (auto world_state = get_world_state())
	{
		return world_state->get_world();
	}

	return nullptr;
}

Camera* InputHandler::get_camera()
{
	if (auto world_state = get_world_state())
	{
		return world_state->get_camera();
	}

	return nullptr;
}

InputHandler::InputHandler(GameState* game_state) :
	m_game_state(game_state)
{
}

Game* InputHandler::get_game()
{
	if (auto game_state = get_game_state())
	{
		return game_state->get_game();
	}

	return {};
}