#pragma once

#include "action.h"

class SwapPlacesAction : public Action
{
protected:
	std::shared_ptr<Entity> target;

public:
	SwapPlacesAction(const TimeUnit& action_length = TimeUnit(0.0), const std::shared_ptr<Entity>& actor = { nullptr }, const std::shared_ptr<Entity>& target = { nullptr }) :
		Action(action_length, actor), target( target )
	{}

	virtual bool check_action() override;
	virtual ActionResult perform();

};
