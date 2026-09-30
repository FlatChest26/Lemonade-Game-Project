#pragma once
#include "input_handler.h"
#include "world.h"
#include "entity.h"
#include "camera.h"

class WorldGameState;

class PlayerInputHandler : public InputHandler
{
public:
	PlayerInputHandler() :
		InputHandler()
	{}

protected:

	std::weak_ptr<WorldGameState> get_world_state();

	std::shared_ptr<Entity> get_player();
	std::shared_ptr<World> get_world();
	std::shared_ptr<Camera> get_camera();

protected:
	// -- Input Handling -- //

	OVERRIDE_HANDLER_EVENT(key_down);
};

