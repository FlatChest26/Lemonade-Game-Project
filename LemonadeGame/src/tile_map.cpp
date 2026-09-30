#include "tile_map.h"
#include "tile_data.h"
#include "fov.h"
#include <variant>

TileMap::TileMap(const std::weak_ptr<World>& world, const Length& width, const Length& height, const Length& depth) :
	m_world(world), m_width(width), m_height(height), m_depth(depth)
{
	m_tiles.reserve(width * height * depth);

	for (Coord x = 0; (Length) x < width; ++x) for (Coord y = 0; (Length) y < height; ++y) for (Coord z = 0; (Length) z < depth; ++z)
	{
		m_tiles.push_back(Tile(&tile_types::AIR, x * y * z));
	}

	update_all_tiles();
}

void TileMap::set_tile(const Coord& x, const Coord& y, const Coord& z, TileType* type)
{
	if (!in_bounds(x, y, z))
	{
		return; // Do nothing if tile out of bounds
	}

	m_tiles[get_index(x, y, z)] = Tile(type, x * y * z);

	update_tile_at(x, y, z);

	update_tile_at(x + 1, y, z);
	update_tile_at(x - 1, y, z);
	update_tile_at(x, y + 1, z);
	update_tile_at(x, y - 1, z);

	update_tile_at(x + 1, y + 1, z);
	update_tile_at(x + 1, y - 1, z);
	update_tile_at(x - 1, y + 1, z);
	update_tile_at(x - 1, y - 1, z);
}

void TileMap::update_tile_at(const Coord& x, const Coord& y, const Coord& z)
{
	if (!in_bounds(x, y, z))
	{
		return; // Do nothing if tile out of bounds
	}

	auto tile = get_tile(x, y, z);

	// Update wall textures
	if (m_tiles[get_index(x, y, z)].is_wall_texture())
	{
		int mask_value = 0;

		auto tile_above = get_tile(x, y - 1, z);
		auto tile_below = get_tile(x, y + 1, z);
		auto tile_left = get_tile(x - 1, y, z);
		auto tile_right = get_tile(x + 1, y, z);

		if (tile_above && tile->can_connect_with(tile_above))
		{
			mask_value += 1;
		}

		if (tile_below && tile->can_connect_with(tile_below))
		{
			mask_value += 2;
		}

		if (tile_left && tile->can_connect_with(tile_left))
		{
			mask_value += 4;
		}

		if (tile_right && tile->can_connect_with(tile_right))
		{
			mask_value += 8;
		}

		if (mask_value >= 0 && mask_value <= 16)
		{
			auto decoration_index = DECORATION_MASK[mask_value];

			if (decoration_index >= 0 && decoration_index < tile->get_texture().size())
			{
				tile->set_texture_idx(decoration_index);
			}
		}
		else
		{
			tile->set_texture_idx(0);
		}
	}
}

void TileMap::compute_fov(const Coord& pov_x, const Coord& pov_y, const Coord& pov_z, const Coord& radius)
{
	for (Coord x = 0; (Length)x < get_width(); ++x) for (Coord y = 0; (Length)y < get_height(); ++y) for (Coord z = 0; (Length)z < get_depth(); ++z)
	{
		fov_mark_visible(x, y, z, false);
	}

	FOV::compute_fov(this, pov_x, pov_y, pov_z, radius);
}

bool TileMap::fov_is_blocked(const Coord& x, const Coord& y, const Coord& z, const Position& pov) const
{
	if (!in_bounds(x, y, z))
		return true;

	if (auto tile = get_tile(x, y, z))
	{
		return tile->is_opaque();
	}

	return false;
}

void TileMap::fov_mark_visible(const Coord& x, const Coord& y, const Coord& z, bool visible)
{
	if (!in_bounds(x, y, z))
	{
		return;
	}

	if (auto tile = get_tile(x, y, z))
	{
		return tile->set_visible(visible);
	}

}

bool TileMap::is_visible(const Coord& x, const Coord& y, const Coord& z) const
{
	if (!in_bounds(x, y, z))
	{
		return false;
	}

	if (auto tile = get_tile(x, y, z))
	{
		return tile->is_visible();
	}

	return false;
}

bool TileMap::is_explored(const Coord& x, const Coord& y, const Coord& z) const
{
	if (!in_bounds(x, y, z))
	{
		return false;
	}

	if (auto tile = get_tile(x, y, z))
	{
		return tile->is_explored();
	}

	return false;
}

void TileMap::step(TimeUnit time_delta)
{
}

void TileMap::update()
{
}

void TileMap::render(RenderParams params) const
{
	int draw_x{ 0 }, draw_y{ 0 }, draw_z{ 0 };
	auto& camera = params.camera;

	int layer = 0;

	int view_left = 0;
	int view_right = get_width();

	int view_top = 0;
	int view_bottom = get_height();

	if (camera)
	{
		layer = camera->get_posz();

		view_left = std::max(view_left, camera->get_x1());
		view_right = std::min(view_right, camera->get_x2());

		view_top = std::max(view_top, camera->get_y1());
		view_bottom = std::min(view_bottom, camera->get_y2());
	}

	const Tile* draw_tile{};

	glyph_t draw_glyph{};
	color_t draw_fg{};
	color_t draw_bg{};

	for (int x = view_left; x < view_right; ++x) for (int y = view_top; y < view_bottom; ++y)
	{
		draw_tile = get_tile(x, y, layer);

		draw_glyph = draw_tile->glyph();
		draw_fg = draw_tile->fg();
		draw_bg = draw_tile->bg();

		if (camera)
		{
			if (!camera->in_view(x, y, layer))
				continue;

			std::tie(draw_x, draw_y, draw_z) = camera->get_screen(x, y, layer).as_tuple();
		}

		if (draw_tile->is_air())
		{
			auto tile_below = get_tile(x, y, layer - 1);

			if (tile_below->is_air())
			{
				draw_glyph = 177;
				draw_fg = color::get("light_cyan");
				draw_bg = color::get("black");
			}
			else
			{
				draw_glyph = DISTANT_DOT;
				draw_fg = tile_below->fg();
				draw_bg = tile_below->bg();
			}
		}

		if (!is_visible(x, y, layer))
		{
			if (is_explored(x, y, layer))
			{
				draw_fg = color::get("explored");
			}
			else
			{
				draw_fg = color::get("unexplored");
			}
		}

		output::put_rgb(draw_x, draw_y, draw_glyph, draw_fg, draw_bg);
	}
}
