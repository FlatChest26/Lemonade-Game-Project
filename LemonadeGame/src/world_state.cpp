#include "world_state.h"
#include "tile_map.h"
#include "tile_data.h"
#include "creature.h"
#include "game.h"

#include "player_input_handler.h"

#include "map_builder.h"

WorldGameState::WorldGameState()
{}

void WorldGameState::new_game()
{
	// Create world
	m_world = std::make_shared<World>( weak_from_this() );

	// Create camera
	m_camera = std::make_shared<Camera>();

	// Create Creatures

	auto Player = std::shared_ptr<CreatureData>(
		new CreatureData(
			"PLAYER",
			CreatureData::initializer {
				.name = {"John", "John Lemonade"},
				.age = 24,
				.gender = gender::NON_SPECIFIC,
				.sexuality = sexuality::PANSEXUAL,
				.sexual_position = SexualPosition::VERSE_SWITCH,
				.species = &species::HUMAN
			}
		)
	);

	auto Snow = std::shared_ptr<CreatureData>(
		new CreatureData(
			"SNOW",
			CreatureData::initializer {
				.name = {"Snow", "Snow Oaks"},
				.age = 21,
				.gender = gender::FEMBOY,
				.sexuality = sexuality::PANSEXUAL,
				.sexual_position = SexualPosition::SUB_SWITCH,
				.species = &species::HUMAN
			}
		)
	);

	m_world->add_entity(std::shared_ptr<Creature>(new Creature(Snow, { {7, 4, 0}, DEFAULT_SIZE, NORTH })));
	m_player = m_world->add_entity(std::shared_ptr<Creature>(new Creature(Player, { Position(0, 0, 3), DEFAULT_SIZE, NORTH})));
	
	// Create Input Handler
	handle_input_result(InputResult{ std::make_shared<PlayerInputHandler>() });
	
	// Create Tile Map
	auto tile_map = std::make_shared<TileMap>(m_world);
	m_world->set_tile_map(tile_map);

	// Build Map
	MapBuilder::set_floor(tile_map.get(), &tile_types::GRASS, 0);
	MapBuilder::make_building(tile_map.get(), Position(5, 2, 0), Size(6, 6, 1), &tile_types::PLAIN_FLOOR, &tile_types::PLAIN_WALL, &tile_types::AIR, &tile_types::PLAIN_FLOOR);
	
	tile_map->set_tile(5, 3, 0, &tile_types::WINDOW);
	tile_map->set_tile(5, 5, 0, &tile_types::DOOR);
	tile_map->set_tile(9, 3, 0, &tile_types::UPWARD_STAIRS);
	tile_map->set_tile(9, 3, 1, &tile_types::DOWNWARD_STAIRS);

	// Update things

	update_camera();
	step( TimeUnit { } );
}

void WorldGameState::update()
{
	switch (m_run_speed)
	{
	case runspeed_flag::REAL_TIME:
	{
		step(1.0);
		break;
	}
	case runspeed_flag::REAL_TIME_2X:
	{
		step(1.0);
		step(1.0);
		break;
	}
	case runspeed_flag::REAL_TIME_4X:
	{
		step(1.0);
		step(1.0);
		step(1.0);
		step(1.0);
		break;
	}
	case runspeed_flag::REAL_TIME_8X:
	{
		step(1.0);
		step(1.0);
		step(1.0);
		step(1.0);
		step(1.0);
		step(1.0);
		step(1.0);
		step(1.0);
		break;
	}
	}
}

void WorldGameState::render() const
{
	if (m_world)
	{
		m_world->render(RenderParams{ .camera = m_camera, .use_fov = true });
	}
}

void WorldGameState::step( TimeUnit time_delta )
{
	if ( m_world )
	{
		m_world->step( time_delta );

		if (auto player = get_player())
		{
			if (auto tile_map = m_world->get_tile_map())
			{
				tile_map->compute_fov(player->posx(), player->posy(), player->posz(), 25);
			}
		}
	}

	update_camera();

	if ( auto g = m_game.lock() )
	{
		g->request_screen_update();
	}

	//COUT("Simulation stepped by " << time_delta.total_seconds() << " seconds");
}

void WorldGameState::update_camera()
{
	if (!m_camera)
	{
		return;
	}

	if ( m_player ) m_camera->set_position( m_player->pos() );

	m_camera->set_size( 
		Size {
			(uint32_t) floor( output::get_console_width() * 0.8 ),
			(uint32_t) floor( output::get_console_height() * 0.8 ),
			CAMERA_VIEW_DEPTH
		} 
	);
	
}

#include "snowy_macros.h"

void WorldGameState::handle_action(std::shared_ptr<Action> action)
{
	if (action == nullptr)
	{
		return;
	}

	if (!action->check_action())
	{
		return;
	}

	auto actor = action->get_actor();
	auto action_result = action->perform();

	if (action_result.response != "")
	{
		game_message(action_result.response);
	}

	if (action_result.success)
	{
		while (action_result.has_next_action())
		{
			actor->add_action(std::move(action_result.get_next_action()));

			if (actor == get_player())
			{
				handle_input_result(InputResult(InputResult::Type::Action));
			}
		}
	}
}

void WorldGameState::handle_input_result(const InputResult& result)
{
	if (result.type == InputResult::Type::Action /* && get_run_speed() == runspeed_flag::TURN_BASED */ )
	{
		auto player = get_player();
		if (player)
		{
			if (auto current_action = player->get_current_action())
			{
				step(current_action->get_action_length());
			}
			else if (auto next_action = player->get_next_action())
			{
				step(next_action->get_action_length());
			}

			return;
		}
	}

	GameState::handle_input_result(result);
}

void WorldGameState::game_message(std::string msg)
{
	if (last_msg == msg)
		return;

	last_msg = msg;

	std::cout << msg << std::endl;

	if (auto game = m_game.lock())
		game->request_screen_update();
}