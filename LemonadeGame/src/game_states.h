#pragma once

#include "lemonade_game.h"
#include "render_params.h"
#include "action.h"
#include "input_handler.h"
#include <memory>

class GameState : public std::enable_shared_from_this<GameState>
{
	friend class Game;

protected:
	std::weak_ptr<Game> m_game;
	std::vector<std::shared_ptr<InputHandler>> m_input_handlers;

public:
	// -- Getters -- //

	std::weak_ptr<Game> get_game() const { return m_game; }

	std::shared_ptr<InputHandler> get_current_input_handler() const
	{
		if (m_input_handlers.empty())
		{
			return nullptr;
		}

		return m_input_handlers.back();
	}

	virtual void update() {}
	virtual void render() const {}

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

	virtual void handle_action(std::shared_ptr<Action> action)
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

	void push_input_handler(std::shared_ptr<InputHandler> input_handler)
	{
		if (!input_handler)
			return;

		m_input_handlers.push_back(input_handler);

		input_handler->m_game_state = weak_from_this();
	}

	void reset_input_handlers(std::shared_ptr<InputHandler> input_handler)
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
};
