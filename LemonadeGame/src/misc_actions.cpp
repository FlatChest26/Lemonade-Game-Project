#include "misc_actions.h"
#include "entity.h"
#include "world.h"

using namespace std;

bool SwapPlacesAction::check_action()
{
	if (!Action::check_action())
		return false;

	if (target == nullptr)
		return false;

	if (auto world = get_world())
	{
		if (world->is_blocked(target->pos(), target, (target->posz() - get_actor()->posz())))
		{
			return false;
		}

		if (world->is_blocked(get_actor()->pos(), get_actor(), (get_actor()->posz() - target->posz())))
		{
			return false;
		}
	}

	return true;
}

ActionResult SwapPlacesAction::perform()
{
	Position orig_pos = get_actor()->pos();
	get_actor()->pos() = target->pos();
	target->pos() = orig_pos;

	return ActionResult{ true, fmt::format("{} swaps places with {}.", *(get_actor()->get_thing()), *(target->get_thing())) };
}