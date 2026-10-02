#include "location_actions.h"
#include "tile_map.h"
#include "world.h"

using namespace std;

bool ActionWithLocation::check_action()
{
	return get_world() != nullptr;
}

Tile* ActionWithLocation::get_tile_at_dest()
{
	if (auto tm = get_tile_map())
		return tm->get_tile(m_loc_x, m_loc_y, m_loc_z);
	return &::tile_map::VOID_TILE;
}

static bool transform_tile(World* world, int loc_x, int loc_y, int loc_z)
{
	if (!world)
	{
		return false;
	}

	auto tile_map = world->get_tile_map();
	const auto tile = tile_map->get_tile(loc_x, loc_y, loc_z);

	auto transformed_tile = tile_types::get(tile->get_type()->get_transform_ID());

	if (!(transformed_tile->tile_flags() & TileFlag::IS_WALKABLE) && world->has_blocking_entities_at(loc_x, loc_y, loc_z))
		return false;

	tile_map->set_tile(loc_x, loc_y, loc_z, transformed_tile);

	return true;
}

static bool recursively_open_or_close_tiles(World* world, int loc_x, int loc_y, int loc_z, bool open)
{
	if (!world)
	{
		return false;
	}

	auto tile_map = world->get_tile_map();
	auto tile = tile_map->get_tile(loc_x, loc_y, loc_z);

	if (!transform_tile(world, loc_x, loc_y, loc_z))
		return false;

	for (auto& check_location : transforms::get_surrounding_locations(Position(loc_x, loc_y, loc_z)))
	{
		auto other_tile = tile_map->get_tile(check_location);

		if (tile->can_connect_with(other_tile))
		{
			if (open && other_tile->can_open())
			{
				if (!recursively_open_or_close_tiles(world, check_location.x, check_location.y, check_location.z, true))
				{
					transform_tile(world, loc_x, loc_y, loc_z);
					return false;
				}
			}

			if (!open && other_tile->can_close())
			{
				if (!recursively_open_or_close_tiles(world, check_location.x, check_location.y, check_location.z, false))
				{
					transform_tile(world, loc_x, loc_y, loc_z);
					return false;
				}
			}
		}
	}

	return true;
}

bool OpenAction::check_action()
{
	if (get_tile_map() == nullptr)
		return false;

	if (!get_tile_at_dest()->can_open())
		return false;

	return true;
}

ActionResult OpenAction::perform()
{
	return ActionResult(recursively_open_or_close_tiles(get_world(), m_loc_x, m_loc_y, m_loc_z, true));
}

bool CloseAction::check_action()
{
	if (get_tile_map() == nullptr)
		return false;

	if (!get_tile_at_dest()->can_close())
		return false;

	return true;
}

ActionResult CloseAction::perform()
{
	return ActionResult(recursively_open_or_close_tiles(get_world(), m_loc_x, m_loc_y, m_loc_z, false));
}