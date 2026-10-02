#include "inventory.h"

Inventory::Inventory(Entity* owner, size_t max_slots, std::vector<Entity*> items) :
	m_owner(owner), m_items(items), m_max_slots(max_slots)
{
}

bool Inventory::add_item(Entity* item)
{
	if (m_items.size() >= m_max_slots)
		return false;

	m_items.push_back(item);

	return true;
}

bool Inventory::remove_item(Entity* item)
{
	auto it = std::find(m_items.begin(), m_items.end(), item);

	if (it != m_items.end())
	{
		m_items.erase(it);
		return true;
	}

	return false;
}