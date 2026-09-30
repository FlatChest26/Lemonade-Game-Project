#include "world.h"
#include "world_state.h"

using namespace std;

World::World( std::weak_ptr<GameState> game_state ):
	m_game_state( game_state )
{}

shared_ptr<Entity> World::add_entity( shared_ptr<Entity> entity )
{
	m_entities.push_back( entity );
	entity->set_location( weak_from_this() );

	return entity;
}

std::vector<std::shared_ptr<Entity>> World::get_entities_at(const Coord& x, const Coord& y, const Coord& z)
{
	std::vector<std::shared_ptr<Entity>> entities_at_location;

	for (auto entity : m_entities)
	{
		if (entity->get_posx() == x && entity->get_posy() == y && entity->get_posz() == z)
		{
			entities_at_location.push_back(entity);
		}
	}

	return entities_at_location;
}

weak_ptr<GameState> World::get_game_state() const
{
	return m_game_state;
}

std::weak_ptr<WorldGameState> World::get_world_game_state() const
{
	if (auto game_state = get_game_state().lock())
	{
		if (game_state->is_world_game_state())
		{
			return std::dynamic_pointer_cast<WorldGameState>(game_state);
		}
	}

	return {};
}

weak_ptr<Game> World::get_game() const
{
	if ( auto g = m_game_state.lock() )
	{
		return g->get_game();
	}

	return {};
}

bool World::has_blocking_entities_at(const Position& pos) const
{
	auto blocking_entity = find_if(
		m_entities.begin(), m_entities.end(),
		[&pos](std::shared_ptr<Entity> entity) { return entity->is_blocking() && entity->contains_point(pos); }
	);

	return blocking_entity != m_entities.end();
}

std::vector<std::shared_ptr<Entity>> World::get_blocking_entities_at(const Position& pos) const
{
	vector<shared_ptr<Entity>> entities_at(m_entities.size());
	auto s = std::copy_if(
		m_entities.begin(), m_entities.end(),
		entities_at.begin(),
		[&pos](std::shared_ptr<Entity> entity) { return entity->is_blocking() && entity->contains_point(pos); }
	);

	entities_at.resize(std::distance(entities_at.begin(), s));

	return entities_at;
}

bool World::is_blocked(const Coord& x, const Coord& y, const Coord& z, std::shared_ptr<const Entity> fit, const Coord& z_dir)
{
	if (m_tile_map->is_blocked(x, y, z))
	{
		return true;
	}

	if (abs(z_dir) > 0)
	{
		if (z_dir > 0 && m_tile_map->get_tile(x, y, z)->has_floor())
			return true;

		if (z_dir < 0 && m_tile_map->get_tile(x, y, z)->has_ceiling())
			return true;
	}

	for (auto check_entity : get_entities_at(x, y, z))
	{
		if (check_entity == fit)
		{
			continue;
		}

		if (check_entity->is_blocking())
		{
			return true;
		}
	}

	return false;
}

void World::step( TimeUnit time_delta )
{
	for ( const auto& entity : m_entities )
	{
		entity->step( time_delta );
	}

	m_current_time.add( time_delta );
}

void World::render( RenderParams params )
{
	if (m_tile_map)
	{
		m_tile_map->render(params);
	}

	for ( const auto& entity : m_entities )
	{
		entity->render( params );
	}
}