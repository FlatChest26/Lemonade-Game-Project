#pragma once

#include "render_params.h"

#include "entity.h"
#include "time_units.h"
#include "tile_map.h"
#include "game_states.h"

class Game;
class WorldGameState;

class World : public std::enable_shared_from_this<World>
{
private:
	std::weak_ptr<GameState> m_game_state;
	std::vector<std::shared_ptr<Entity>> m_entities;
	std::shared_ptr<TileMap> m_tile_map;

	// By default, starts at 12:00 PM on June 11, 2025
	TimeDate m_current_time = TimeDate
	{
		.time_point = TimeUnit(0, 0, 12),
		.day = 11,
		.month = 6,
		.year = 2025
	};

public:
	World( std::weak_ptr<GameState> game_state );


	// -- Getters -- //

	std::weak_ptr<GameState> get_game_state() const;
	std::weak_ptr<WorldGameState> get_world_game_state() const;
	std::weak_ptr<Game> get_game() const;
	std::shared_ptr<TileMap> get_tile_map() const { return m_tile_map; }

	// -- Utilities -- //

	std::shared_ptr<Entity> add_entity(std::shared_ptr<Entity> entity);
	std::vector<std::shared_ptr<Entity>> get_entities_at(const Coord& x, const Coord& y, const Coord& z);

	std::vector<std::shared_ptr<Entity>> get_entities() { return m_entities; }

	void set_tile_map(std::shared_ptr<TileMap> tile_map) { m_tile_map = tile_map; }


	// -- Checks -- //
	bool has_blocking_entities_at(const Position& pos) const;
	bool has_blocking_entities_at(const Coord& x, const Coord& y, const Coord& z) const
	{
		return has_blocking_entities_at(Position(x, y, z));
	}

	std::vector<std::shared_ptr<Entity>> get_blocking_entities_at(const Position& pos) const;
	std::vector<std::shared_ptr<Entity>> get_blocking_entities_at(const Coord& x, const Coord& y, const Coord& z) const
	{
		return get_blocking_entities_at(Position(x, y, z));
	}

	bool is_blocked(const Coord& x, const Coord& y, const Coord& z, std::shared_ptr<const Entity> fit = nullptr, const Coord& z_dir = 0);
	bool is_blocked(const Position pos, std::shared_ptr<const Entity> fit = nullptr, const Coord& z_dir = 0) 
	{
		return is_blocked(pos.x, pos.y, pos.z, fit, z_dir);
	}

	// -- Game Loop -- //

	void step( TimeUnit time_delta ); // Tick the world's time forward

	void render( RenderParams params );
};

