#pragma once

#include <vector>
#include <memory>

class Entity;

class Inventory
{
protected:
	std::weak_ptr<Entity> m_owner{ };

	std::vector<std::shared_ptr<Entity>> m_items;
	size_t m_max_slots{ 1 };

public:
	Inventory(std::weak_ptr<Entity> owner = {}, std::vector<std::shared_ptr<Entity>> items = {}, size_t max_slots = 1);

	std::weak_ptr<Entity> get_owner() const { return m_owner; }
	std::vector<std::shared_ptr<Entity>> get_items() const { return m_items; }
	size_t get_max_slots() const { return m_max_slots; }

	bool add_item(std::shared_ptr<Entity> item);
	bool remove_item(std::shared_ptr<Entity> item);
	void clear() { m_items.clear(); }
};
