#pragma once

#include "entity.h"
#include "creature_data.h"

extern const EntityFlags DEFAULT_CREATURE_FLAGS;

class Creature : public Entity
{
public:
	Creature(std::shared_ptr<CreatureData> data, Transform transform = { DEFAULT_POSITION, DEFAULT_SIZE, NORTH }, const std::weak_ptr<World>& world = {});
};