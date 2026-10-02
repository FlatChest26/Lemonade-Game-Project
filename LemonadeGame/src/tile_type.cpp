#include "tile_type.h"

TileType::TileType(const initializer& init) :
	m_ID(init.ID),
	m_description(init.description),
	m_texture(init.texture),
	m_flags(init.flags),
	m_random_texture(init.random_texture),
	m_wall_texture(init.wall_texture),
	m_connects_with(init.connects_with),
	m_connect_flags(init.connect_flags),
	m_transforms_to(init.transforms_to)
{
}