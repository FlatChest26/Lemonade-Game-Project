#include "creature.h"

const EntityFlags DEFAULT_CREATURE_FLAGS = EntityFlag::NONE
	| EntityFlag::CAN_COLLIDE 
	| EntityFlag::IS_BLOCKING 
	| EntityFlag::IS_MAP_BOUND 
	| EntityFlag::CAN_FALL
;

Creature::Creature(std::shared_ptr<CreatureData> data, Transform transform, const std::weak_ptr<World>& world) :
	Entity(data, transform, world, DEFAULT_CREATURE_FLAGS)
{
}
