#include "world_state.h"
#include "tile_map.h"
#include "tile_type.h"
#include "creature.h"
#include "game.h"
#include "body_plan.h"

#include "player_input_handler.h"

#include "map_builder.h"

WorldGameState::WorldGameState()
{
}

void WorldGameState::new_game()
{
	// Create World
	m_world = std::make_unique<World>(this);
	m_world->set_time(TimeDate{ .time_point = TimeUnit::from_hours(7.0), .day = 11, .month = 6, .year = 2025 });

	// Create Camera
	m_camera = std::make_unique<Camera>();

	// Create renderer

	m_renderer = std::make_unique<Renderer>(this);

	// Create Creatures

	auto PlayerSoul = new CreatureSoul(CreatureSoul::initializer{
		.ID = "PLAYER",
		.name = {"Player", "Lemonade Person"},
		.age = 24,
		.gender = gender::NON_SPECIFIC,
		.sexuality = sexuality::PANSEXUAL,
		.sexual_position = SexualPosition::VERSE_SWITCH,
		.description = "That is you."
	});

	auto SnowSoul = new CreatureSoul(CreatureSoul::initializer{
		.ID = "SNOW",
		.name = {"Snow", "Snow Oaks"},
		.age = 21,
		.gender = gender::FEMBOY,
		.sexuality = sexuality::PANSEXUAL,
		.sexual_position = SexualPosition::SUB_SWITCH,
		.description = "He is the cutest boy."
	});

	auto AdelineSoul = new CreatureSoul(CreatureSoul::initializer{
		.ID = "ADELINE",
		.name = {"Adeline", "Adeline Baudelaire"},
		.age = 22,
		.gender = gender::FEMININE,
		.sexuality = sexuality::PANSEXUAL,
		.sexual_position = SexualPosition::DOM_SWITCH,
		.description = "She is the most devious succubus."
	});

	const Species* human = SpeciesDB::get("HUMAN").get();
	const Species* demon = SpeciesDB::get("DEMON").get();

	auto Player = new Creature(Creature::initializer{
		.soul = PlayerSoul,
		.species = human,
		.body_mods = { body_mod::GirlCock },
		.transform = { Position(0, 0, 0), DEFAULT_SIZE, NORTH },
	});

	auto Snow = new Creature(Creature::initializer{
		.soul = SnowSoul,
		.species = human,
		.body_mods = { body_mod::MaleParts },
		.transform = { Position(7, 4, 0), DEFAULT_SIZE, NORTH },
	});

	auto Adeline = new Creature(Creature::initializer{
		.soul = AdelineSoul,
		.species = demon,
		.body_mods = { body_mod::FemaleParts },
		.transform = { Position(8, 4, 0), DEFAULT_SIZE, NORTH },
	});

	m_world->add_entity(std::unique_ptr<Creature>(Adeline));
	m_world->add_entity(std::unique_ptr<Creature>(Snow));
	m_player = m_world->add_entity(std::unique_ptr<Creature>(Player));

	// Create Input Handler
	handle_input_result(InputResult{ new PlayerInputHandler(this) });

	// Create Tile Map
	auto tile_map = std::make_unique<TileMap>(m_world.get());

	// Build Map
	MapBuilder::set_floor(tile_map.get(), &tile_types::GRASS, 0);
	MapBuilder::make_building(tile_map.get(), Position(5, 2, 0), Size(6, 6, 1), &tile_types::PLAIN_FLOOR, &tile_types::PLAIN_WALL, &tile_types::AIR, &tile_types::PLAIN_FLOOR);

	tile_map->set_tile(5, 3, 0, &tile_types::WINDOW);
	tile_map->set_tile(5, 5, 0, &tile_types::DOOR);
	tile_map->set_tile(9, 3, 0, &tile_types::UPWARD_STAIRS);
	tile_map->set_tile(9, 3, 1, &tile_types::DOWNWARD_STAIRS);

	// Create AI Manger
	m_ai_manager = std::make_unique<AIManager>(this);

	for (const auto& entity : m_world->get_entities())
	{
		if (!entity->is_animate())
			continue;

		if (entity == get_player())
			continue;

		m_ai_manager->add_brain_to_entity(entity);
	}

	m_world->set_tile_map(std::move(tile_map));

	// Update things
	m_camera_focus = Position(m_world->get_tile_map()->get_width() / 2, m_world->get_tile_map()->get_height() / 2, m_player->posz());
}

void WorldGameState::update(double delta_time)
{
	TimeUnit base_speed = TimeUnit::from_seconds(delta_time * 6);

	switch (m_run_speed)
	{
	case runspeed_flag::REAL_TIME:
	{
		step(base_speed);
		break;
	}
	case runspeed_flag::REAL_TIME_2X:
	{
		step(base_speed);
		step(base_speed);
		break;
	}
	case runspeed_flag::REAL_TIME_4X:
	{
		step(base_speed);
		step(base_speed);
		step(base_speed);
		step(base_speed);
		break;
	}
	case runspeed_flag::REAL_TIME_8X:
	{
		step(base_speed);
		step(base_speed);
		step(base_speed);
		step(base_speed);
		step(base_speed);
		step(base_speed);
		step(base_speed);
		step(base_speed);
		step(base_speed);
		break;
	}
	}
}

void WorldGameState::render() const
{
	if (auto renderer = get_renderer())
	{
		renderer->render(RenderParams{
			.camera = m_camera.get(),
			.use_fov = m_player != nullptr,
			.show_explored_tiles = get_game_settings().show_explored_tiles,
			.render_world = true,
			.render_entities = true,
			.render_tile_map = true,
			});
	}
}

void WorldGameState::step(TimeUnit time_delta)
{
	ASSERT(m_world, "WorldGameState::step() called but world is null");

	if (m_player)
	{
		// Update FOV for player
		if (m_player->has_field_of_view())
		{
			if (auto tile_map = m_world->get_tile_map())
			{
				tile_map->compute_fov(m_player->posx(), m_player->posy(), m_player->posz(), m_player->view_range());
			}
		}
	}

	m_world->step(time_delta);

	if (m_ai_manager)
	{
		m_ai_manager->step(time_delta);
	}

	auto game_settings = get_game_settings();

	if (game_settings.fixed_camera)
	{
		update_camera(Position(m_camera_focus.x, m_camera_focus.y, m_player ? m_player->posz() : m_camera_focus.z));
	}
	else
	{
		update_camera(m_player ? m_player->pos() : m_camera_focus);
	}

	if (auto game = m_game)
	{
		game->request_screen_update();
	}

	//COUT("Simulation stepped by " << time_delta.total_seconds() << " seconds");
}

void WorldGameState::update_camera(const Position& pos)
{
	if (!m_camera)
	{
		return;
	}

	m_camera->set_position(pos);

	m_camera->set_size(
		Size
		{
			(uint32_t)floor(output::get_console_width() * 0.8),
			(uint32_t)floor(output::get_console_height() * 0.8),
			CAMERA_VIEW_DEPTH
		}
	);
}

#include "snowy_macros.h"

void WorldGameState::handle_action(std::unique_ptr<Action> action)
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

	if (actor && action_result.success)
	{
		while (action_result.has_next_action())
		{
			if (actor->can_queue_action())
			{
				actor->queue_action(std::move(action_result.get_next_action()));

				if (actor == get_player())
				{
					handle_input_result(InputResult(InputResult::Type::Action));
				}
			}	
		}
	}
}

void WorldGameState::handle_input_result(const InputResult& result)
{
	if (result.type == InputResult::Type::Action /*&& get_run_speed() == runspeed_flag::TURN_BASED*/)
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
	if (m_last_msg == msg)
	{
		return;
	}

	m_last_msg = msg;

	std::cout << msg << std::endl;

	if (auto game = m_game)
	{
		game->request_screen_update();
	}
}