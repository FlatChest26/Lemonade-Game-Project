#pragma once

#ifndef LEMONADE_GAME_SRC_MAP_BUILDER_H
#define LEMONADE_GAME_SRC_MAP_BUILDER_H

#include "tile_map.h"

namespace MapBuilder
{
	inline void set_floor(TileMap* tile_map, TileType* floor_tile, Coord layer = 0)
	{
		for (Coord y = 0; (Length)y < tile_map->get_height(); y++) for (Coord x = 0; (Length)x < tile_map->get_width(); x++)
		{
			tile_map->set_tile(x, y, layer, floor_tile);
		}
	}

	inline void make_building(
		TileMap* tile_map,
		Position building_position,
		Size building_size,
		TileType* floor_tile,
		TileType* wall_tile,
		TileType* air_tile = nullptr,
		TileType* roof_tile = nullptr
	)
	{
		if (!air_tile)
		{
			air_tile = &tile_types::AIR;
		}

		int building_left = building_position.x;
		int building_right = building_position.x + building_size.x;

		int building_top = building_position.y;
		int building_bottom = building_position.y + building_size.y;

		int building_back = building_position.z;
		int building_front = building_position.z + building_size.z;

		TileType* place_tile;

		for (int z = building_back; z < building_front; z++) for (int y = building_top; y < building_bottom; y++) for (int x = building_left; x < building_right; x++)
		{
			place_tile = air_tile;

			if (floor_tile)
			{
				if (z == building_back)
				{
					place_tile = floor_tile;
				}
			}

			if (wall_tile)
			{
				if (x == building_left || x == building_right - 1 ||
					y == building_top || y == building_bottom - 1)
				{
					place_tile = wall_tile;
				}
			}

			tile_map->set_tile(x, y, z, place_tile);
		}

		if (roof_tile)
		{
			for (int y = building_top; y < building_bottom; y++) for (int x = building_left; x < building_right; x++)
			{
				tile_map->set_tile(x, y, building_front, roof_tile);
			}
		}
	}
}

#endif // !LEMONADE_GAME_SRC_MAP_BUILDER_H