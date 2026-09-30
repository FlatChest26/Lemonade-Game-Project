#include "inventory.h"

Inventory::Inventory(std::weak_ptr<Entity> owner, std::vector<std::shared_ptr<Entity>> items, size_t max_slots):
	m_owner(owner), m_items(items), m_max_slots(max_slots)
{
}

bool Inventory::add_item(std::shared_ptr<Entity> item)
{
	if (m_items.size() >= m_max_slots)
		return false;

	m_items.push_back(item);

	return true;
}

bool Inventory::remove_item(std::shared_ptr<Entity> item)
{
	auto it = std::find(m_items.begin(), m_items.end(), item);

	if (it != m_items.end())
	{
		m_items.erase(it);
		return true;
	}

	return false;
}