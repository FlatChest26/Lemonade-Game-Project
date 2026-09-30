#pragma once

#ifndef LEMONADE_GAME_SRC_RENDER_PARAMS_H
#define LEMONADE_GAME_SRC_RENDER_PARAMS_H

#include "camera.h"

struct RenderParams
{
	std::shared_ptr<const Camera> camera { nullptr };
	bool use_fov = true;
};

#endif // !LEMONADE_GAME_SRC_RENDER_PARAMS_H