#pragma once

#include "game_states.h"
#include "world.h"
#include "time_units.h"

namespace runspeed_flag
{
	constexpr int TURN_BASED = 0;
	constexpr int REAL_TIME = 1;
	constexpr int REAL_TIME_2X = 2;
	constexpr int REAL_TIME_4X = 4;
	constexpr int REAL_TIME_8X = 8;
}

/* The main game state. Simulates the world of the game. */
class WorldGameState : public GameState
{
private:
	std::shared_ptr<Entity> m_player;
	std::shared_ptr<World> m_world;
	std::shared_ptr<Camera> m_camera;

	int m_run_speed = runspeed_flag::TURN_BASED;

	std::string last_msg;

public:
	WorldGameState();

public:
	// -- Getters -- //
	std::shared_ptr<Entity> get_player() const { return m_player; }
	std::shared_ptr<World> get_world() const { return m_world; }
	std::shared_ptr<Camera> get_camera() const { return m_camera; }

	constexpr int get_run_speed() const { return m_run_speed; }
	void set_run_speed(int run_speed) { m_run_speed = run_speed; }

	// -- Checks -- //

	virtual constexpr bool is_world_game_state() const override { return true; }

public:

	// -- Utilities -- //

	virtual void new_game();
	virtual void update() override;
	virtual void render() const override;

	void step( TimeUnit time_delta );

	void update_camera();

	virtual void handle_action(std::shared_ptr<Action> action) override;

	virtual void handle_input_result(const InputResult& result) override;

	void game_message(std::string msg);
};
