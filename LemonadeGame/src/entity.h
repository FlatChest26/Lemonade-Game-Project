#pragma once

#include "render_params.h"
#include "thing.h"
#include "time_units.h"
#include "inventory.h"
#include "action.h"

#include "movable.h"

#include "snowy_macros.h"
#include "game_states.h"


#include <variant>
#include <vector>

class TileMap;
class World;
class Game;
class WorldGameState;

using EntityLocation = std::variant<std::weak_ptr<World>, std::weak_ptr<Entity>>;
using ActionQueue = std::vector<std::shared_ptr<Action>>;

using EntityFlags = uint64_t;

namespace EntityFlag {
	constexpr EntityFlags NONE = 0;
	constexpr EntityFlags CAN_FALL = 1 << 0;
	constexpr EntityFlags IS_BLOCKING = 1 << 1;
	constexpr EntityFlags CAN_COLLIDE = 1 << 2;
	constexpr EntityFlags CAN_HOLD_ITEMS = 1 << 3;
	constexpr EntityFlags CAN_BE_HELD = 1 << 4;
	constexpr EntityFlags IS_MAP_BOUND = 1 << 5;
}

/* An entity represents any "thing" which has a position in the world. */
class Entity : public std::enable_shared_from_this<Entity>, public Movable
{
protected:

	// A shared pointer to the "thing" this entity represents.
	std::shared_ptr<Thing> m_thing;

	// What stores the entity. Usually the world but can also be another entity.
	EntityLocation m_location;
	
	// This entity's inventory
	std::shared_ptr<Inventory> m_inventory;

	// A queue of actions this entity wants to take.
	ActionQueue m_action_queue;

	std::shared_ptr<Action> m_current_action{nullptr};
	TimeUnit m_time_spent_on_current_action{ 0.0 };

	// Flags for this entity
	EntityFlags m_entity_flags;

public:
	Entity( std::shared_ptr<Thing> thing, Transform transform = { DEFAULT_POSITION, DEFAULT_SIZE, NORTH }, const std::weak_ptr<World>& world = {}, EntityFlags entity_flags = 0);
	~Entity();

public:

	// -- Getters -- //

	// Return variant of this entity's location. Can be either a weak pointer to the world or a weak pointer to another entity.
	EntityLocation get_location() const { return m_location; }

	// Returns a weak pointer to the world holding this entity.
	std::weak_ptr<World> get_world() const;

	// Returns a weak pointer to the entity carrying this entity, if one exists.
	std::weak_ptr<Entity> get_entity_holder() const;
	std::weak_ptr<World> get_world_holder() const;

	// Returns a weak pointer to the world game state
	std::weak_ptr<GameState> get_game_state() const;
	std::weak_ptr<WorldGameState> get_world_game_state() const;

	// Returns a weak pointer to the game.
	std::weak_ptr<Game> get_game() const;

	// Returns a pointer to the player entity.
	Entity* get_player() const;

	// Return a pointer to the tile map this entity is on.
	TileMap* get_tile_map() const;

	// Thing //

	std::shared_ptr<Thing> get_thing() const { ASSERT( m_thing, "entity doesn't exist" );  return m_thing; }

	virtual ThingID thing_ID() const { return get_thing()->ID; }

	virtual constexpr Renderable get_renderable() const { return get_thing()->get_renderable(); }

	virtual constexpr Noun get_noun() const { return get_thing()->get_noun(); }
	virtual constexpr Name get_name() const { return get_thing()->get_name(); }

public:
	// -- Checks -- //

	bool operator==( Entity other );
	virtual bool is_player() const;


	virtual bool can_be_held() const;
	virtual bool can_hold_items() const;

	virtual bool is_map_bound() const;
	virtual bool can_collide() const;

	virtual bool is_blocking() const;

	virtual bool can_fall() const;

	virtual bool has_action() const { return m_action_queue.size() > 0; }
	virtual bool has_current_action() const { return m_current_action != nullptr; }

	virtual std::shared_ptr<Action> get_current_action() const { return m_current_action; }

	// -- Utilities -- //

	virtual bool is_on_floor() const;

	bool hold_entity(std::shared_ptr<Entity> entity);
	void release_entity(std::shared_ptr<Entity> entity);

	bool add_action(std::shared_ptr<Action> action, bool back = true );
	std::shared_ptr<Action> get_next_action();

	virtual void start_next_action();

	virtual void start_action(std::shared_ptr<Action> action);
	virtual void continue_action(TimeUnit delta);
	virtual void complete_action();

	virtual void on_action_complete(std::shared_ptr<Action> completed_action, TimeUnit time_took_to_complete) {}


public:
	// -- Setters -- //

	// Location //

	bool set_location( std::weak_ptr<World> world );
	bool set_location( std::weak_ptr<Entity> entity );

	// -- Game Loop -- //

	virtual void step( TimeUnit time_delta );
	virtual void update();
	virtual void render( RenderParams params ) const;
};
