#pragma once

#ifndef LEMONADE_GAME_SRC_MISC_ACTIONS_H
#define LEMONADE_GAME_SRC_MISC_ACTIONS_H

#include "action.h"

class SwapPlacesAction : public Action
{
protected:
	Entity* target;

public:
	SwapPlacesAction(const TimeUnit& action_length = TimeUnit::none(), Entity* actor = nullptr, Entity* target = nullptr) :
		Action(action_length, actor), target(target)
	{
	}

	virtual bool check_action() override;
	virtual ActionResult perform();
};

#endif // !LEMONADE_GAME_SRC_MISC_ACTIONS_H