#pragma once

#include "action.h"
#include "time_units.h"
#include "entity.h"

class ActionWithDirection : public Action
{
protected:
	Coord m_dx{ 0 };
	Coord m_dy{ 0 };
	Coord m_dz{ 0 };

public:
	ActionWithDirection(const TimeUnit& action_length = TimeUnit(0.0), std::shared_ptr<Entity> actor = nullptr, const Coord& dx = 0, const Coord& dy = 0, const Coord& dz = 0) :
		Action(action_length, actor), m_dx(dx), m_dy(dy), m_dz(dz)
	{}

	// -- Getters -- //

	Position delta_pos() const { return Position(m_dx, m_dy, m_dz); }
	Position destination() const { return Position(m_actor->posx() + m_dx, m_actor->posy() + m_dy, m_actor->posz() + m_dz); }

	Coord destx() const { return m_actor->posx() + m_dx; }
	Coord desty() const { return m_actor->posy() + m_dy; }
	Coord destz() const { return m_actor->posz() + m_dz; }
};

class BumpAction : public ActionWithDirection
{
public:
	BumpAction(const TimeUnit& action_length = TimeUnit(0.0), std::shared_ptr<Entity> actor = nullptr, const Coord& dx = 0, const Coord& dy = 0, const Coord& dz = 0) :
		ActionWithDirection(action_length, actor, dx, dy, dz)
	{}

	virtual ActionResult perform() override;

	virtual std::string get_action_name() const override
	{
		return "bump action";
	}
};


class MovementAction : public ActionWithDirection
{
public:
	MovementAction(const TimeUnit& action_length = TimeUnit(0.0), std::shared_ptr<Entity> entity = nullptr, const Coord& dx = 0, const Coord& dy = 0, const Coord& dz = 0) :
		ActionWithDirection(action_length, entity, dx, dy, dz)
	{}

	virtual ActionResult perform() override;

	virtual std::string get_action_name() const override
	{
		return "movement action";
	}
};


