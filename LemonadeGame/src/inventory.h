#pragma once

#ifndef LEMONADE_GAME_SRC_INVENTORY_H
#define LEMONADE_GAME_SRC_INVENTORY_H

#include <vector>
#include <memory>

class Entity;

class Inventory
{
protected:
	Entity* m_owner{ };
	size_t m_max_slots{ 1 };

	std::vector<Entity*> m_items;

public:
	Inventory(Entity* owner = {}, size_t max_slots = 1, std::vector<Entity*> items = {});

	Entity* get_owner() const { return m_owner; }
	std::vector<Entity*> get_items() const { return m_items; }
	size_t get_max_slots() const { return m_max_slots; }

	void set_max_slots(size_t new_max) { m_max_slots = new_max; }

	bool add_item(Entity* item);
	bool remove_item(Entity* item);
	void clear() { m_items.clear(); }
};

#endif // !LEMONADE_GAME_SRC_INVENTORY_H