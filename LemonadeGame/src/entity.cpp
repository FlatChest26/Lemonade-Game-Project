#include "entity.h"

#include "game.h"
#include "world_state.h"
#include "world.h"

#include "action.h"

using namespace std;

Entity::Entity( std::shared_ptr<Thing> thing, Transform transform, const std::weak_ptr<World>& world, EntityFlags entity_flags):
	Movable( transform ), m_thing( thing ), m_entity_flags(entity_flags)
{
	set_location( world );

	if (m_entity_flags & EntityFlag::CAN_HOLD_ITEMS)
		m_inventory = std::make_shared<Inventory>(weak_from_this());
}

Entity::~Entity()
{}

std::weak_ptr<World> Entity::get_world() const
{
	if (auto holder = get_world_holder().lock())
	{
		return holder;
	}

	if ( auto holder = get_entity_holder().lock() )
	{
		return holder->get_world();
	}

	return {};
}

std::weak_ptr<Entity> Entity::get_entity_holder() const
{
	if ( std::holds_alternative<weak_ptr<Entity>>( m_location ) )
	{
		return std::get<weak_ptr<Entity>>( m_location );
	}

	return {};
}

std::weak_ptr<World> Entity::get_world_holder() const
{
	if (std::holds_alternative<weak_ptr<World>>(m_location))
	{
		return std::get<weak_ptr<World>>(m_location);
	}

	return {};
}

std::weak_ptr<GameState> Entity::get_game_state() const
{
	if (auto world = get_world().lock())
	{
		return world->get_game_state();
	}

	return {};
}

std::weak_ptr<WorldGameState> Entity::get_world_game_state() const
{
	if ( auto world = get_world().lock() )
	{
		return world->get_world_game_state();
	}

	return {};
}

std::weak_ptr<Game> Entity::get_game() const
{
	if ( auto world = get_world().lock() )
	{
		return world->get_game();
	}

	return {};
}

Entity* Entity::get_player() const
{
	if ( auto world_game_state = get_world_game_state().lock() )
	{
		return world_game_state->get_player().get();
	}

	return nullptr;
}

TileMap* Entity::get_tile_map() const
{
	if (auto world = get_world().lock())
	{
		return world->get_tile_map().get();
	}

	return nullptr;
}

bool Entity::operator==( Entity other )
{
	return thing_ID() == other.thing_ID();
}

bool Entity::is_player() const
{
	if ( !get_player() )
	{
		return false;
	}

	return ( *this ) == ( *get_player() );
}

bool Entity::can_be_held() const
{
	return m_entity_flags & EntityFlag::CAN_BE_HELD;
}

bool Entity::can_hold_items() const
{
	
	return m_entity_flags & EntityFlag::CAN_HOLD_ITEMS;
}

bool Entity::set_location( weak_ptr<World> world )
{
	if (auto holder = get_entity_holder().lock())
	{
		holder->release_entity(shared_from_this());
	}

	m_location = world;

	return true;
}

bool Entity::set_location( weak_ptr<Entity> entity )
{
	if (auto holder = get_entity_holder().lock())
	{
		holder->release_entity(shared_from_this());
	}

	if ( auto e = entity.lock() )
	{
		m_location = entity;
	}

	return false;
}

bool Entity::is_map_bound() const
{
	return m_entity_flags & EntityFlag::IS_MAP_BOUND;
}

bool Entity::can_collide() const
{
	return m_entity_flags & EntityFlag::CAN_COLLIDE;
}

bool Entity::is_blocking() const
{
	return m_entity_flags & EntityFlag::IS_BLOCKING;
}

bool Entity::can_fall() const
{
	return m_entity_flags & EntityFlag::CAN_FALL;
}

bool Entity::is_on_floor() const
{
	auto tile_map = get_tile_map();
	if (!tile_map)
	{
		CERR("ERROR: No tile map found on entity " << m_thing->ID);
		return true;
	}

	auto tile = tile_map->get_tile(pos());

	if (!tile)
	{
		CERR("ERROR: No tile found at entity " << m_thing->ID);
		return false;
	}

	if (tile->has_floor() || tile->can_ascend() || tile->can_descend())
	{
		return true;
	}

	if (auto world = get_world().lock())
	{
		if (world->is_blocked(posx(), posy(), posz() - 1, shared_from_this(), -1))
		{
			return true;
		}
	}

	return false;
}

bool Entity::hold_entity(std::shared_ptr<Entity> entity)
{
	if (!entity)
		return false;

	if (!entity->can_be_held())
		return false;

	if (!can_hold_items())
		return false;

	if (!m_inventory || !m_inventory->add_item(entity))
		return false;

	entity->set_location(weak_from_this());

	return true;
}

void Entity::release_entity(std::shared_ptr<Entity> entity)
{
	if (!entity)
		return;

	if (!m_inventory)
		return;

	m_inventory->remove_item(entity);
}

bool Entity::add_action(std::shared_ptr<Action> action, bool back)
{
	if (!action)
		return false;

	if (back)
	{
		m_action_queue.push_back(action);
	}
	else
	{
		m_action_queue.insert(m_action_queue.begin(), action);
	}

	return true;
}

std::shared_ptr<Action> Entity::get_next_action()
{
	if (m_action_queue.empty())
		return nullptr;

	return m_action_queue.front();
}

void Entity::start_next_action()
{
	if (m_action_queue.empty())
		return;

	std::shared_ptr<Action> action = std::move(m_action_queue.front());
	m_action_queue.erase(m_action_queue.begin());

	start_action(action);
}

void Entity::start_action(std::shared_ptr<Action> action)
{
	m_current_action = action;
	m_time_spent_on_current_action = TimeUnit(0.0);

	if (m_current_action->is_action_complete(m_time_spent_on_current_action))
	{
		complete_action();
	}
}

void Entity::continue_action(TimeUnit delta)
{
	if (!m_current_action)
	{
		complete_action();
		return;
	}

	m_time_spent_on_current_action.add(delta);
	
	if (m_current_action->is_action_complete(m_time_spent_on_current_action))
	{
		complete_action();
	}
}

void Entity::complete_action()
{
	m_time_spent_on_current_action = TimeUnit(0.0);
	on_action_complete(m_current_action, m_time_spent_on_current_action);

	if (!m_current_action)
	{
		return;
	}

	if (auto world_state = get_world_game_state().lock())
	{
		world_state->handle_action(std::move(m_current_action));
	}
	else
	{
		m_current_action.reset();
	}

}

void Entity::step(TimeUnit time_delta)
{
	if (time_delta.total_seconds() > 0 && can_fall() && !is_on_floor())
	{
		move(0, 0, -1);
	}
	
	if (has_current_action())
	{
		continue_action(time_delta);
	}
	else if (get_next_action())
	{
		start_next_action();
		continue_action(time_delta);
	}
}

void Entity::update()
{
	if (can_hold_items() && !m_inventory)
	{
		m_inventory = std::make_shared<Inventory>(weak_from_this());
	}
}

void Entity::render( RenderParams params ) const
{
	int world_x { posx() }, world_y { posy() }, world_z { posz() };
	int draw_x { world_x }, draw_y { world_y }, depth { 0 };

	if ( auto& camera = params.camera )
	{
		if ( !camera->in_view( world_x, world_y, world_z ) )
			return;

		std::tie( draw_x, draw_y, depth ) = camera->get_screen( world_x, world_y, world_z ).as_tuple();
	}

	glyph_t draw_glyph {};
	color_t draw_fg {};
	color_t draw_bg {};

	for ( const auto& [offset_x, offset_y, offset_z] : get_offsets() ) // In the case of drawing an entity larger than 1x1x1
	{
		draw_glyph = m_thing->glyph();
		draw_fg = m_thing->fg();
		draw_bg = m_thing->bg();

		if ( is_player() )
		{
			draw_glyph = PLAYER_CHAR;
			draw_fg = DEFAULT_FG_COLOR;
		}

		if (auto tile_map = get_tile_map())
		{
			if (!tile_map->is_visible(world_x + offset_x, world_y + offset_y, world_z + offset_z))
			{
				continue;
			}
		}

		if ( depth - offset_z < 0 || depth - offset_z > 1 )
			continue;

		if ( depth - offset_z == 1 )
		{

			if (auto tile_map = get_tile_map())
			{
				if (auto tile = tile_map->get_tile(pos()))
				{
					if (tile->has_ceiling())
					{
						continue;
					}
				}
				if (auto tile_above = tile_map->get_tile(posx(), posy(), posz() + 1))
				{
					if (tile_above->has_floor())
					{
						continue;
					}
				}
			}
			draw_glyph = DISTANT_DOT;
		}

		if (!is_on_floor())
		{
			draw_bg = color::get("light_cyan");
		}

		

		output::put_rgb( draw_x + offset_x, draw_y + offset_y, draw_glyph, draw_fg, draw_bg );
	}
}