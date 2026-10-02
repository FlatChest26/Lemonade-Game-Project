#include "creature.h"

const EntityFlags DEFAULT_CREATURE_FLAGS = EntityFlag::NONE
| EntityFlag::CAN_COLLIDE
| EntityFlag::IS_BLOCKING
| EntityFlag::IS_MAP_BOUND
| EntityFlag::CAN_FALL
| EntityFlag::HAS_FOV
| EntityFlag::CAN_HOLD_ITEMS
| EntityFlag::CAN_BE_HELD
| EntityFlag::ANIMATE
;

Creature::Creature(const initializer& init) :
	Entity(init.soul, init.transform, nullptr, DEFAULT_CREATURE_FLAGS),
	m_species(init.species)
{
	if (init.species)
	{
		std::vector<BodyMod> combined_body_mods = init.species->body_mods;
		combined_body_mods.reserve(init.body_mods.size() + init.species->body_mods.size());
		combined_body_mods.insert(combined_body_mods.end(), init.body_mods.begin(), init.body_mods.end());

		m_anatomy = std::make_unique<Anatomy>(init.species->default_body_plan, combined_body_mods);
	}
	else
	{
		CERR("Creature: " << full_name() << " has no species assigned (and thus, no anatomy either)!");
	}
}

size_t Creature::default_inventory_slots() const
{
	return 2;
}

TimeUnit Creature::speed() const
{
	if (is_player())
	{
		return TimeUnit::from_seconds(0.7);
	}
	else
	{
		return TimeUnit::from_seconds(1.2);
	}
}