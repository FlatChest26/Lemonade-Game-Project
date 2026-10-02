#pragma once

#ifndef LEMONADE_GAME_SRC_GAME_STATES_H
#define LEMONADE_GAME_SRC_GAME_STATES_H

#include "lemonade_game.h"
#include "render_params.h"
#include "action.h"
#include "input_handler.h"
#include "snowy_macros.h"
#include "renderer.h"

#include <memory>

class WorldGameState;
struct GameSettings;

class GameState : public std::enable_shared_from_this<GameState>
{
	friend class Game;

protected:
	Game* m_game;

	std::unique_ptr<Renderer> m_renderer;
	std::vector<std::unique_ptr<InputHandler>> m_input_handlers;

public:
	bool is_fresh = true;

	// -- Getters -- //

	Game* get_game() const { return m_game; }
	GameSettings& get_game_settings() const;

	Renderer* get_renderer() const { return m_renderer.get(); }

	InputHandler* get_current_input_handler() const
	{
		if (m_input_handlers.empty())
		{
			return nullptr;
		}

		return m_input_handlers.back().get();
	}

	virtual void update(double delta_time) {}
	virtual void render() const;

public:
	// -- Input Handling -- //

	virtual InputResult handle_event(SDL_Event event)
	{
		if (auto input_handler = get_current_input_handler())
		{
			return input_handler->handle_event(event);
		}

		return InputResult();
	}

	virtual void handle_action(std::unique_ptr<Action> action)
	{
		CERR("This game state does not handle actions.");
	}

public:
	// -- Game State -- //

	virtual void handle_input_result(const InputResult& result)
	{
		switch (result.type)
		{
		case InputResult::Type::None:
		{
			return;
		}
		case InputResult::Type::Pop_InputHandler:
		{
			pop_input_handler();

			return;
		}
		case InputResult::Type::Push_InputHandler:
		{
			push_input_handler(result.next_input_handler);

			return;
		}
		case InputResult::Type::Reset_InputHandler:
		{
			reset_input_handlers(result.next_input_handler);

			return;
		}
		}
	}

protected:

	void pop_input_handler()
	{
		if (m_input_handlers.empty())
		{
			return;
		}

		m_input_handlers.pop_back();
	}

	void push_input_handler(InputHandler* input_handler)
	{
		if (!input_handler)
			return;

		auto new_input_handler = std::unique_ptr<InputHandler>(input_handler);
		m_input_handlers.emplace_back(std::move(new_input_handler));
		m_input_handlers.back()->m_game_state = this;
	}

	void reset_input_handlers(InputHandler* input_handler)
	{
		m_input_handlers.clear();

		if (input_handler)
		{
			push_input_handler(input_handler);
		}
	}

public:
	// -- Checks -- //

	virtual constexpr bool is_world_game_state() const { return false; }
	virtual constexpr WorldGameState* as_world_game_state() const { return nullptr; }
};

#endif // !LEMONADE_GAME_SRC_GAME_STATES_H