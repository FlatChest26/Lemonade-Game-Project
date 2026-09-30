#pragma once

#include "thing.h"
#include "tile_data.h"

class Tile : public Thing
{
protected:
	TileType* m_type{ nullptr };
	size_t m_texture_idx{ 0 };

	bool m_visible = false;
	bool m_explored = false;

public:
	constexpr bool is_tile() const override { return true; }
	constexpr Tile* as_tile() override { return this; }

	Tile(TileType* type, size_t idx = 0) : Thing(std::string(type->ID) + "_" + std::to_string(idx)), m_type(type)
	{
		if (m_type->random_texture)
		{
			m_texture_idx = (rand() + idx) % m_type->texture.size();
		}
		else
		{
			m_texture_idx = 0;
		}
	}
	
	constexpr TileType* get_type() const { return m_type; }

	constexpr Noun get_noun() const override { return m_type->noun; }
	constexpr Pronouns get_pronouns() const override { return m_type->pronouns; }

	virtual Renderable get_renderable() const override { return m_type->texture[m_texture_idx]; }


	// Flags

	constexpr bool is_walkable() const { return m_type->flags & TileFlag::IS_WALKABLE; }
	constexpr bool is_blocked() const { return !(m_type->flags & TileFlag::IS_WALKABLE); }

	constexpr bool has_floor() const { return m_type->flags & TileFlag::HAS_FLOOR; }
	constexpr bool has_ceiling() const { return m_type->flags & TileFlag::HAS_CEILING; }

	constexpr bool is_transparent() const { return m_type->flags & TileFlag::IS_TRANSPARENT; }
	constexpr bool is_opaque() const { return !(m_type->flags & TileFlag::IS_TRANSPARENT); }

	constexpr bool can_ascend() const { return m_type->flags & TileFlag::CAN_ASCEND; }
	constexpr bool can_descend() const { return m_type->flags & TileFlag::CAN_DESCEND; }

	constexpr bool can_open() const { return m_type->flags & TileFlag::CAN_OPEN; }
	constexpr bool can_close() const { return m_type->flags & TileFlag::CAN_CLOSE; }

	constexpr bool is_air() const { return m_type->flags & TileFlag::IS_AIR; }

	// Setters & Getters

	bool is_visible() const { return m_visible; }
	bool is_explored() const { return m_explored; }

	void set_visible(bool visible = true) { m_visible = visible; if (visible) set_explored(true); }
	void set_explored(bool explored = true) { m_explored = explored; }

	void set_texture_idx(size_t idx) { m_texture_idx = idx % m_type->texture.size(); }
	constexpr const std::vector<Renderable>& get_texture() const { return m_type->texture; }

	constexpr bool is_random_texture() const { return m_type->random_texture; }
	constexpr bool is_wall_texture() const { return m_type->wall_texture; }

	constexpr bool can_connect_with(Tile* other) const
	{
		if (!other)
		{
			return false;
		}

		for (const auto& connect_id : m_type->connects_with)
		{
			for (const auto& other_connect_id : other->get_type()->connect_flags)
			{
				if (connect_id == other_connect_id)
				{
					return true;
				}
			}
		}

		return false;
	}
};
