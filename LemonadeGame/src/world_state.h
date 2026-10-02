#pragma once

#ifndef LEMONADE_GAME_SRC_WORLD_STATE_H
#define LEMONADE_GAME_SRC_WORLD_STATE_H

#include "game_states.h"
#include "world.h"
#include "time_units.h"
#include "camera.h"

#include "ai_handler.h"

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
	std::unique_ptr<World> m_world;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<AIManager> m_ai_manager;

	Entity* m_player;
	Position m_camera_focus;

	int m_run_speed = runspeed_flag::TURN_BASED;

	std::string m_last_msg;

public:
	WorldGameState();

public:
	// -- Getters -- //
	Entity* get_player() const { return m_player; }
	World* get_world() const { return m_world.get(); }
	Camera* get_camera() const { return m_camera.get(); }
	AIManager* get_ai_manager() const { return m_ai_manager.get(); }

	constexpr int get_run_speed() const { return m_run_speed; }
	void set_run_speed(int run_speed) { m_run_speed = run_speed; }

	// -- Checks -- //

	virtual constexpr bool is_world_game_state() const override { return true; }
	virtual constexpr WorldGameState* as_world_game_state() const override { return (WorldGameState*)this; }

public:

	// -- Utilities -- //

	virtual void new_game();
	virtual void update(double delta_time) override;
	virtual void render() const override;

	void step(TimeUnit time_delta);

	void update_camera(const Position& pos = Position{ 0, 0, 0 });

	virtual void handle_action(std::unique_ptr<Action> action) override;

	virtual void handle_input_result(const InputResult& result) override;

	void game_message(std::string msg);
};

#endif // !LEMONADE_GAME_SRC_WORLD_STATE_H