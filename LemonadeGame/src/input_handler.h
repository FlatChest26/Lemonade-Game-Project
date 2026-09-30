#pragma once

#include <SDL_events.h>
#include <memory>

#include "snowy_macros.h"

#define DEF_HANDLER_EVENT(x) virtual InputResult ev_##x( SDL_Event event )
#define OVERRIDE_HANDLER_EVENT(x) virtual InputResult ev_##x( SDL_Event event ) override

class InputHandler;
class GameState;

struct InputResult
{
	enum class Type
	{
		None,
		Push_GameState,
		Pop_GameState,
		Reset_GameState,
		Push_InputHandler,
		Pop_InputHandler,
		Reset_InputHandler,
		Action,
	};

	Type type{ Type::None };

	std::shared_ptr<GameState> next_game_state{ nullptr };
	std::shared_ptr<InputHandler> next_input_handler{ nullptr };

	InputResult() :
		type(Type::None), next_game_state(nullptr), next_input_handler(nullptr)
	{}

	InputResult(Type type, std::shared_ptr<GameState> game_state) :
		type(type), next_game_state(game_state)
	{}

	InputResult(Type type, std::shared_ptr<InputHandler> input_handler) :
		type(type), next_input_handler(input_handler)
	{}

	InputResult(Type type) :
		type(type), next_game_state(nullptr), next_input_handler(nullptr)
	{}

	InputResult(std::shared_ptr<GameState> game_state) :
		type(Type::Push_GameState), next_game_state(game_state), next_input_handler(nullptr)
	{}

	InputResult(std::shared_ptr<InputHandler> input_handler) :
		type(Type::Push_InputHandler), next_input_handler(input_handler)
	{}
};

class InputHandler
{
	friend class GameState;

protected:
	std::weak_ptr<GameState> m_game_state;
	bool m_allow_input_pass_through{ false };

protected:
	InputResult dispatch(SDL_Event event)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return ev_quit(event);

		case SDL_DISPLAYEVENT:
			return ev_display_event(event);
		case SDL_WINDOWEVENT:
			return ev_window_event(event);
		case SDL_SYSWMEVENT:
			return ev_syswm_event(event);

		case SDL_KEYDOWN:
			return ev_key_down(event);
		case SDL_KEYUP:
			return ev_key_up(event);

		case SDL_MOUSEMOTION:
			return ev_mouse_motion(event);
		case SDL_MOUSEBUTTONDOWN:
			return ev_mouse_button_down(event);
		case SDL_MOUSEBUTTONUP:
			return ev_mouse_button_up(event);
		case SDL_MOUSEWHEEL:
			return ev_mouse_wheel(event);
		}

		return InputResult{};
	}

public:

	InputHandler() :
		m_game_state()
	{}


public:

	// -- Getters & Setters -- //

	std::weak_ptr<GameState> get_game_state() { return m_game_state; }
	void set_game_state(std::weak_ptr<GameState> game_state) { m_game_state = game_state; }

	bool allow_input_pass_through() const { return m_allow_input_pass_through; }

	// -- Utilities -- //

	virtual InputResult handle_event(SDL_Event event)
	{
		return dispatch(event);
	}

protected:
	DEF_HANDLER_EVENT(quit) { return InputResult(); }

	DEF_HANDLER_EVENT(key_down) { return InputResult(); }
	DEF_HANDLER_EVENT(key_up) { return InputResult(); }

	DEF_HANDLER_EVENT(display_event) { return InputResult(); }
	DEF_HANDLER_EVENT(window_event) { return InputResult(); }
	DEF_HANDLER_EVENT(syswm_event) { return InputResult(); }

	DEF_HANDLER_EVENT(mouse_motion) { return InputResult(); }
	DEF_HANDLER_EVENT(mouse_button_down) { return InputResult(); }
	DEF_HANDLER_EVENT(mouse_button_up) { return InputResult(); }
	DEF_HANDLER_EVENT(mouse_wheel) { return InputResult(); }
};