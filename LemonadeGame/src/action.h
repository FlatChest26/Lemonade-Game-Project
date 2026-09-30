#pragma once

#include "time_units.h"
#include "transforms.h"
#include <memory>
#include <vector>

class Action;
class Entity;
class World;
class WorldGameState;
class TileMap;

struct ActionResponse
{
	std::string response;

	operator std::string() const { return response; }
	operator const char* () const { return response.c_str(); }
};

struct ActionResult
{
	bool success{ false };
	ActionResponse response{""};
	std::vector<std::shared_ptr<Action>> next_actions{ };

	constexpr bool has_next_action() const { return !next_actions.empty(); }

	std::shared_ptr<Action> get_next_action()
	{
		if (has_next_action())
		{
			auto next_action = std::move(next_actions.front());
			next_actions.erase(next_actions.begin());

			return next_action;
		}
		return nullptr;
	}
};

class Action
{
protected:
	std::shared_ptr<Entity> m_actor{ nullptr };
	TimeUnit m_action_length{ 0.0 };

public:
	Action(const TimeUnit& action_length = TimeUnit(0.0), std::shared_ptr<Entity> actor = nullptr): 
		m_action_length(action_length), m_actor(actor)
	{}

	virtual ActionResult perform();

	virtual bool is_action_complete(TimeUnit time) const
	{
		return time >= m_action_length;
	}

	virtual TimeUnit get_action_length() const
	{
		return m_action_length;
	}

	virtual std::string get_action_name() const
	{
		return "default action";
	}

	virtual bool check_action() { return true; }

	virtual std::shared_ptr<Entity> get_actor() const;
	virtual std::weak_ptr<World> get_world() const;
	virtual std::shared_ptr<TileMap> get_tile_map() const;
	virtual std::weak_ptr<WorldGameState> get_world_game_state() const;
};

class WaitAction : public Action
{
public:
	WaitAction(const TimeUnit& action_length = TimeUnit(1.0), std::shared_ptr<Entity> actor = nullptr) :
		Action(action_length, actor)
	{
	}

	virtual ActionResult perform() override
	{
		return ActionResult(true);
	}

	virtual std::string get_action_name() const override
	{
		return "wait action";
	}
};