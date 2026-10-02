#pragma once

#ifndef LEMONADE_GAME_SRC_AI_HANDLER_H
#define LEMONADE_GAME_SRC_AI_HANDLER_H

#include "creature.h"
#include "time_units.h"

#include <memory>

class AIManager;

class AIBrain : std::enable_shared_from_this<AIBrain>
{
protected:
	AIManager* m_manager;
	Entity* m_entity;

public:
	AIBrain(AIManager* manager = nullptr, Entity* entity = nullptr) :
		m_manager(manager), m_entity(entity)
	{
	}

	void step(TimeUnit time_delta);

	Entity* get_entity() { return m_entity; }
};

class World;
class GameState;

class AIManager : std::enable_shared_from_this<AIManager>
{
protected:
	GameState* m_game_state;
	std::vector<std::unique_ptr<AIBrain>> m_brains;

public:
	AIManager(GameState* game_state = {}, std::vector<std::unique_ptr<AIBrain>> brains = {});

	bool add_brain_to_entity(Entity* entity);
	AIBrain* get_entity_brain(Entity* entity);

	void step(TimeUnit time_delta);
};

#endif // !LEMONADE_GAME_SRC_AI_HANDLER_H