#pragma once

#ifndef LEMONADE_GAME_SRC_PLAYER_INPUT_HANDLER_H
#define LEMONADE_GAME_SRC_PLAYER_INPUT_HANDLER_H

#include "input_handler.h"
#include "world.h"
#include "entity.h"
#include "camera.h"

class WorldGameState;

class PlayerInputHandler : public InputHandler
{
public:
	PlayerInputHandler(GameState* game_state = nullptr) :
		InputHandler(game_state)
	{
	}

protected:
	// -- Input Handling -- //

	OVERRIDE_HANDLER_EVENT(key_down);
};

#endif // !LEMONADE_GAME_SRC_PLAYER_INPUT_HANDLER_H