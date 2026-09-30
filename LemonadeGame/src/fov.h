#pragma once

#ifndef SAI_SRC_FOV_H
#define SAI_SRC_FOV_H

#include "color.h"
#include "transforms.h"


class TileMap;

namespace FOV
{
	extern void compute_fov( TileMap* map, int player_x, int player_y, int player_z, int radius = 0 );
}

#endif // !SAI_SRC_FOV_H