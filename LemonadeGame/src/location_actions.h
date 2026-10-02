#pragma once

#ifndef LEMONADE_GAME_SRC_LOCATION_ACTIONS_H
#define LEMONADE_GAME_SRC_LOCATION_ACTIONS_H

#include "action.h"
#include "transforms.h"

class Tile;

class ActionWithLocation : public Action
{
protected:
	Coord m_loc_x{ 0 }, m_loc_y{ 0 }, m_loc_z{ 0 };

public:
	ActionWithLocation(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr, const Coord& loc_x = 0, const Coord& loc_y = 0, const Coord& loc_z = 0) :
		Action(action_length, actor), m_loc_x(loc_x), m_loc_y(loc_y), m_loc_z(loc_z)
	{
	}
	ActionWithLocation(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr, const Position& location = Position(0, 0, 0)) :
		ActionWithLocation(action_length, actor, location.x, location.y, location.z)
	{
	}

	// -- Getters -- //

	Coord loc_x() const { return m_loc_x; }
	Coord loc_y() const { return m_loc_y; }
	Coord loc_z() const { return m_loc_z; }
	Position location() const { return Position(m_loc_x, m_loc_y, m_loc_z); }

	Tile* get_tile_at_dest();

	// -- Misc -- //

	virtual bool check_action() override;
};

class OpenAction : public ActionWithLocation
{
public:
	OpenAction(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr, const Coord& loc_x = 0, const Coord& loc_y = 0, const Coord& loc_z = 0) :
		ActionWithLocation(action_length, actor, loc_x, loc_y, loc_z)
	{
	}
	OpenAction(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr, const Position& location = Position(0, 0, 0)) :
		ActionWithLocation(action_length, actor, location)
	{
	}

	virtual bool check_action() override;
	virtual ActionResult perform();
};

class CloseAction : public ActionWithLocation
{
public:
	CloseAction(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr, const Coord& loc_x = 0, const Coord& loc_y = 0, const Coord& loc_z = 0) :
		ActionWithLocation(action_length, actor, loc_x, loc_y, loc_z)
	{
	}
	CloseAction(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr, const Position& location = Position(0, 0, 0)) :
		ActionWithLocation(action_length, actor, location)
	{
	}

	virtual bool check_action() override;
	virtual ActionResult perform();
};

#endif // !LEMONADE_GAME_SRC_LOCATION_ACTIONS_H