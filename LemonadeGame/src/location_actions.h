#pragma once

#ifndef SAI_SRC_LOCATION_ACTIONS_H
#define SAI_SRC_LOCATION_ACTIONS_H

#include "action.h"
#include "world.h"

class ActionWithLocation : public Action
{
protected:
	Coord loc_x{ 0 }, loc_y{ 0 }, loc_z{ 0 };

public:
	ActionWithLocation(const TimeUnit& action_length = TimeUnit(0.0), std::shared_ptr<Entity> actor = { nullptr }, const Coord& loc_x = 0, const Coord& loc_y = 0, const Coord& loc_z = 0):
		Action(action_length, actor), loc_x( loc_x ), loc_y( loc_y ), loc_z( loc_z )
	{}
	ActionWithLocation(const TimeUnit& action_length = TimeUnit(0.0), const std::shared_ptr<Entity>& actor = { nullptr }, const Position& location = Position(0, 0, 0)):
		ActionWithLocation(action_length, actor, location.x, location.y, location.z )
	{}

	// -- Getters -- //

	Position location() const { return Position( loc_x, loc_y, loc_z ); }

	Tile* get_tile_at_dest();

	// -- Misc -- //

	virtual bool check_action() override;
};


class OpenAction : public ActionWithLocation
{
public:
	OpenAction(const TimeUnit& action_length = TimeUnit(0.0), const std::shared_ptr<Entity>& actor = {nullptr}, const Coord& loc_x = 0, const Coord& loc_y = 0, const Coord& loc_z = 0) :
		ActionWithLocation(action_length, actor, loc_x, loc_y, loc_z )
	{}
	OpenAction(const TimeUnit& action_length = TimeUnit(0.0), const std::shared_ptr<Entity>& actor = { nullptr }, const Position& location = Position(0, 0, 0) ):
		ActionWithLocation(action_length, actor, location )
	{}

	virtual bool check_action() override;
	virtual ActionResult perform();
};

class CloseAction : public ActionWithLocation
{
public:
	CloseAction(const TimeUnit& action_length = TimeUnit(0.0), const std::shared_ptr<Entity>& actor = { nullptr }, const Coord& loc_x = 0, const Coord& loc_y = 0, const Coord& loc_z = 0):
		ActionWithLocation(action_length, actor, loc_x, loc_y, loc_z )
	{}
	CloseAction(const TimeUnit& action_length = TimeUnit(0.0), const std::shared_ptr<Entity>& actor = { nullptr }, const Position& location = Position(0, 0, 0)):
		ActionWithLocation(action_length, actor, location )
	{}

	virtual bool check_action() override;
	virtual ActionResult perform();
};

#endif // !SAI_SRC_LOCATION_ACTIONS_H