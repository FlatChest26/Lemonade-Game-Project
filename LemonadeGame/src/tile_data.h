#pragma once

#include "thing.h"
#include "renderable.h"
#include "enum_traits.h"

#include <vector>


inline const size_t DECORATION_MASK[17] = {
	0,
	6,
	6,
	6,
	5,
	4,
	2,
	8,
	5,
	3,
	1,
	7,
	5,
	9,
	10,
	11,
	0
};


using TileFlags = uint64_t;


namespace TileFlag {
	constexpr TileFlags NONE = 0;


	constexpr TileFlags IS_WALKABLE = 1 << 0;
	constexpr TileFlags IS_BLOCKED = !IS_WALKABLE;

	constexpr TileFlags IS_TRANSPARENT = 1 << 1;
	constexpr TileFlags IS_OPAQUE = !IS_TRANSPARENT;

	constexpr TileFlags HAS_FLOOR = 1 << 2;
	constexpr TileFlags HAS_CEILING = 1 << 3;

	constexpr TileFlags CAN_ASCEND = 1 << 4;
	constexpr TileFlags CAN_DESCEND = 1 << 5;

	constexpr TileFlags IS_AIR = 1 << 6;

	constexpr TileFlags CAN_OPEN = 1 << 7;
	constexpr TileFlags CAN_CLOSE = 1 << 8;
};

using TileTexture = std::vector<Renderable>;

struct TileType
{
	const char* ID;

	Noun noun{ "air", "air" };
	Pronouns pronouns{ pronouns::IT_ITS };
	TileTexture texture{ { ' ', color::get("cyan"), color::get("black") } };

	TileFlags flags{0};

	bool random_texture{ false };
	bool wall_texture{ false };

	std::vector<ThingID> connect_flags{};
	std::vector<ThingID> connects_with{};

	snowy::StringID transforms_to{ "" };
};

namespace tile_types
{
	inline TileType NO_TILE {
		.ID = "TILE_VOID",
		.noun = { "void", "void" },
		.texture = { { ' ', color::get("black"), color::get("black") } },
		.flags = TileFlag::IS_TRANSPARENT | TileFlag::HAS_FLOOR | TileFlag::HAS_CEILING
	};

	inline TileType AIR{
		.ID = "TILE_AIR",
		.noun = { "air", "air" },
		.texture = { { ' ', color::get("cyan"), color::get("black") } },
		.flags = TileFlag::IS_TRANSPARENT | TileFlag::IS_WALKABLE | TileFlag::IS_AIR
	};

	inline TileType GRASS{
		.ID = "TILE_GRASS",
		.noun = { "grass", "grass" },
		.texture = {
			{ ',', color::get("light_green"), color::get("black") }, 
			{ ',', color::get("dark_green"), color::get("black") },
			{ '\'', color::get("light_green"), color::get("black") },
			{ '\'', color::get("dark_green"), color::get("black") },
			{ ';', color::get("light_green"), color::get("black") },
			{ ';', color::get("dark_green"), color::get("black") }
		},
		.flags = TileFlag::IS_TRANSPARENT | TileFlag::IS_WALKABLE | TileFlag::HAS_FLOOR,
		.random_texture = true,
	};

	inline TileType PLAIN_WALL{
		.ID = "TILE_WALL",
		.noun = { "wall", "walls" },
		.texture = {
			{ '#', color::get("white"), color::get("black") },
			{ 201, color::get("white"), color::get("black") }, // ╔
			{ 187, color::get("white"), color::get("black") }, // ╗
			{ 200, color::get("white"), color::get("black") }, // ╚
			{ 188, color::get("white"), color::get("black") }, // ╝
			{ 205, color::get("white"), color::get("black") }, // ═
			{ 186, color::get("white"), color::get("black") }, // ║
			{ 204, color::get("white"), color::get("black") }, // ╠
			{ 185, color::get("white"), color::get("black") }, // ╣
			{ 202, color::get("white"), color::get("black") }, // ╩
			{ 203, color::get("white"), color::get("black") }, // ╦
			{ 206, color::get("white"), color::get("black") }, // ╬
		},
		.flags = TileFlag::IS_BLOCKED | TileFlag::IS_OPAQUE | TileFlag::HAS_FLOOR | TileFlag::HAS_CEILING,
		.wall_texture = true,
		.connect_flags = { "TILE_WALL" },
		.connects_with = { "TILE_WALL", "TILE_WINDOW" }
	};

	inline TileType WINDOW{
		.ID = "TILE_WINDOW",
		.noun = { "window", "windows" },
		.texture = {
			{ '#', color::get("light_cyan"), color::get("black") },
			{ 218, color::get("light_cyan"), color::get("black") }, // ┌
			{ 191, color::get("light_cyan"), color::get("black") }, // ┐
			{ 192, color::get("light_cyan"), color::get("black") }, // └
			{ 217, color::get("light_cyan"), color::get("black") }, // ┘
			{ 196, color::get("light_cyan"), color::get("black") }, // ─
			{ 179, color::get("light_cyan"), color::get("black") }, // │
			{ 195, color::get("light_cyan"), color::get("black") }, // ├
			{ 180, color::get("light_cyan"), color::get("black") }, // ┤
			{ 193, color::get("light_cyan"), color::get("black") }, // ┴
			{ 194, color::get("light_cyan"), color::get("black") }, // ┬
			{ 197, color::get("light_cyan"), color::get("black") }, // ┼
		},
		.flags = TileFlag::IS_BLOCKED | TileFlag::IS_TRANSPARENT | TileFlag::HAS_FLOOR | TileFlag::HAS_CEILING,
		.wall_texture = true,
		.connect_flags = { "TILE_WINDOW" },
		.connects_with = { "TILE_WALL", "TILE_WINDOW" }
	};

	inline TileType PLAIN_FLOOR{
		.ID = "TILE_PLAIN_FLOOR",
		.noun = { "floor", "floors" },
		.texture = { { '+', color::get("light_gray"), color::get("black") } },
		.flags = TileFlag::IS_WALKABLE | TileFlag::IS_TRANSPARENT | TileFlag::HAS_FLOOR,
	};

	inline TileType DOOR{
		.ID = "TILE_DOOR",
		.noun = { "door", "doors" },
		.texture = {
			{ '+', color::get("brown"), color::get("black") },
		},
		.flags = TileFlag::IS_BLOCKED | TileFlag::IS_OPAQUE | TileFlag::HAS_FLOOR | TileFlag::HAS_CEILING | TileFlag::CAN_OPEN,
		.transforms_to = "TILE_OPEN_DOOR"
	};

	inline TileType OPEN_DOOR {
		.ID = "TILE_OPEN_DOOR",
		.noun = { "open door", "open doors" },
		.texture = { { '/', color::get("brown"), color::get("black") } },
		.flags = TileFlag::IS_WALKABLE | TileFlag::IS_TRANSPARENT | TileFlag::HAS_FLOOR | TileFlag::HAS_CEILING | TileFlag::CAN_CLOSE,
		.transforms_to = "TILE_DOOR"
	};

	inline TileType UPWARD_STAIRS{
		.ID = "TILE_UPWARD_STAIRS",
		.noun = { "upward staircase", "upward staircases" },
		.texture = { { '<', color::get("white"), color::get("black") } },
		.flags = TileFlag::IS_WALKABLE | TileFlag::IS_TRANSPARENT | TileFlag::CAN_ASCEND,
	};

	inline TileType DOWNWARD_STAIRS{
		.ID = "TILE_DOWNWARD_STAIRS",
		.noun = { "downward staircase", "downward staircases" },
		.texture = { { '>', color::get("white"), color::get("black") } },
		.flags = TileFlag::IS_WALKABLE | TileFlag::IS_TRANSPARENT | TileFlag::CAN_DESCEND,
	};

	inline TileType UP_AND_DOWN_STAIRS{
		.ID = "TILE_STAIRCASE",
		.noun = { "staircase", "staircases" },
		.texture = { { 'X', color::get("white"), color::get("black") } },
		.flags = TileFlag::IS_WALKABLE | TileFlag::IS_TRANSPARENT | TileFlag::CAN_ASCEND | TileFlag::CAN_DESCEND,
	};

	inline constexpr size_t TILE_TYPE_COUNT = 11;

	inline snowy::StaticDatabase<TileType, TILE_TYPE_COUNT> tiles_db
	{
		{"TILE_VOID",				NO_TILE				},
		{"TILE_AIR",				AIR					},
		{"TILE_GRASS",				GRASS				},
		{"TILE_PLAIN_FLOOR",		PLAIN_FLOOR			},
		{"TILE_PLAIN_WALL",			PLAIN_WALL			},
		{"TILE_DOOR",				DOOR				},
		{"TILE_OPEN_DOOR",			OPEN_DOOR			},
		{"TILE_WINDOW",				WINDOW				},
		{"TILE_UPWARD_STAIRS",		UPWARD_STAIRS		},
		{"TILE_DOWNWARD_STAIRS",	DOWNWARD_STAIRS		},
		{"TILE_STAIRCASE",			UP_AND_DOWN_STAIRS	},
	};

	inline constexpr TileType* get(const snowy::StringID& tile_id)
	{
		if (!tiles_db.contains(tile_id))
		{
			CERR("Unrecognized tile: " << tile_id);
			return &NO_TILE;
		}

		return &tiles_db[tile_id];
	}
}