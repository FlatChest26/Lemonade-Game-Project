#pragma once

#ifndef LEMONADE_GAME_SRC_ACTION_H
#define LEMONADE_GAME_SRC_ACTION_H

#include "time_units.h"
#include <memory>
#include <vector>
#include <string>
#include <utility>

class Action;
class Entity;
class World;
class WorldGameState;
class TileMap;

using ActionQueue = std::vector<std::unique_ptr<Action>>;

struct ActionResponse
{
	std::string response;

	operator std::string() const { return response; }
	operator const char* () const { return response.c_str(); }
};

struct ActionResult
{
	bool success{ false };
	ActionResponse response{ "" };
	ActionQueue next_actions{};

	constexpr bool has_next_action() const { return !next_actions.empty(); }

	void add_next_action(std::unique_ptr<Action> action)
	{
		next_actions.push_back(std::move(action));
	}

	std::unique_ptr<Action> get_next_action()
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
	Entity* m_actor{ nullptr };
	TimeUnit m_action_length{ TimeUnit::none() };

public:
	Action(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr) :
		m_action_length(action_length), m_actor(actor)
	{
	}

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

	virtual Entity* get_actor() const;
	virtual World* get_world() const;
	virtual TileMap* get_tile_map() const;
	virtual WorldGameState* get_world_game_state() const;
};

class WaitAction : public Action
{
public:
	WaitAction(const TimeUnit& action_length = TimeUnit::from_seconds(1.0), Entity* actor = nullptr) :
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

#endif // !LEMONADE_GAME_SRC_ACTION_H