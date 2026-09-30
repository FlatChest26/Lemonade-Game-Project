#include "direction_action.h"
#include "location_actions.h"
#include "misc_actions.h"

#include "world.h"

#include "snowy_macros.h"
#include <memory>

ActionResult BumpAction::perform()
{
	auto tile_map = get_tile_map();

	if (tile_map && tile_map->in_bounds(destination()))
	{
		auto dest = destination();

		if (tile_map->get_tile(dest)->can_open())
		{
			return ActionResult{ true, "", { std::make_shared<OpenAction>(TimeUnit(1.0), get_actor(), dest) } };
		}
	}

	if (auto world = get_world().lock())
	{
		auto blocking = world->get_blocking_entities_at(destination());
		for (auto& entity : blocking)
		{
			if (entity != get_actor() /* && entity->is_animate() && !entity->fixed_in_place()*/)
			{
				return ActionResult{ true, "", { std::make_shared<SwapPlacesAction>(TimeUnit(1.0), get_actor(), entity) } };
			}
		}
	}

	return ActionResult{ true, "", { std::make_shared<MovementAction>(TimeUnit(1.0), m_actor, m_dx, m_dy, m_dz) } };
}

ActionResult MovementAction::perform()
{
	auto world = get_world().lock();

	if (!world)
	{
		CERR("No world found");
		return ActionResult(false);
	}

	if (m_actor->can_fall() && !m_actor->is_on_floor())
	{
		return ActionResult(false);
	}

	Coord start_x = m_actor->posx();
	Coord start_y = m_actor->posy();
	Coord start_z = m_actor->posz();

	Coord final_x = start_x + m_dx;
	Coord final_y = start_y + m_dy;
	Coord final_z = start_z + m_dz;

	if (m_actor->is_map_bound())
	{
		if (auto tile_map = m_actor->get_tile_map())
		{
			Coord bounds_x = tile_map->get_width();
			Coord bounds_y = tile_map->get_height();
			Coord bounds_z = tile_map->get_depth();

			final_x = std::max(0, std::min(final_x, bounds_x - 1));
			final_y = std::max(0, std::min(final_y, bounds_y - 1));
			final_z = std::max(0, std::min(final_z, bounds_z - 1));
		}
	}

	if (m_actor->can_collide())
	{
		bool has_collision = false;

		if (world->is_blocked(final_x, final_y, final_z, m_actor, m_dz))
		{
			has_collision = true;
		}

		// Attempt to slide along wall
		if (has_collision)
		{
			if (abs(m_dx) > 0 && world->is_blocked(final_x, start_y, start_z, m_actor, m_dz))
			{
				final_x = start_x;
			}

			if (abs(m_dy) > 0 && world->is_blocked(start_x, final_y, start_z, m_actor, m_dz))
			{
				final_y = start_y;
			}

			if (abs(m_dz) > 0 && world->is_blocked(start_x, start_y, final_z, m_actor, m_dz))
			{
				final_z = start_z;
			}
		}

		if (world->is_blocked(final_x, final_y, final_z, m_actor, m_dz))
		{
			return ActionResult(false);
		}
	}

	if (m_dz > 0 && m_actor->can_fall())
	{
		if (!get_tile_map()->get_tile(start_x, start_y, start_z)->can_ascend())
		{
			return ActionResult(false);
		}
	}

	m_actor->set_position(Position(final_x, final_y, final_z));

	if (start_x == final_x && start_y == final_y && start_z == final_z)
	{
		return ActionResult(false);
	}

	return ActionResult(true);
}
