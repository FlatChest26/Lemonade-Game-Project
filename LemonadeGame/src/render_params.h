#pragma once

#ifndef LEMONADE_GAME_SRC_RENDER_PARAMS_H
#define LEMONADE_GAME_SRC_RENDER_PARAMS_H

#include "camera.h"

struct RenderParams
{
	const Camera* camera{ nullptr };
	bool use_fov = true;
	bool show_explored_tiles = true;
	bool render_world = true;
	bool render_entities = true;
	bool render_tile_map = true;
};

#endif // !LEMONADE_GAME_SRC_RENDER_PARAMS_H