#pragma once

#include "lemonade_game.h"
#include "snowy_grammar.h"
#include "renderable.h"

using ThingID = std::string;

class CreatureData;
class Tile;

/* An abstraction of a "thing" that can be represented in the game. A grammar object with some visual presence. */
class Thing : public GrammarObject
{
public:
	ThingID ID;

	Thing( ThingID ID ): 
		ID( ID ) 
	{}

	virtual ~Thing() {}

public:
	// -- Getters -- //

	// Renderable //

	virtual Renderable get_renderable() const { return Renderable(); }

	virtual glyph_t glyph() const { return get_renderable().glyph; }
	virtual color_t fg() const { return get_renderable().fg; }
	virtual color_t bg() const { return get_renderable().bg; }


	// -- Type Checks -- //

	virtual constexpr bool is_creature() const { return false; }
	virtual constexpr CreatureData* as_creature() { return nullptr; }

	virtual constexpr bool is_tile() const { return false; }
	virtual constexpr Tile* as_tile() { return nullptr; }

	// Misc //

	std::string get_display_name() const { return full_name() + " " + char(glyph()); }
	operator std::string() const { return get_display_name(); }

};


inline std::ostream& operator<<(std::ostream& os, const Thing& thing)
{
	return os << thing.get_display_name();
};

inline std::wostream& operator<<(std::wostream& os, const Thing& thing)
{
	return os << thing.get_display_name().c_str();
};

namespace std
{
	inline string to_string(const Thing& thing)
	{
		return thing.get_display_name();
	}

	inline string to_string(Thing* thing)
	{
		return thing->get_display_name();
	}
}


namespace fmt
{
	template <typename T>
	struct formatter<T, std::enable_if_t<std::is_base_of<Thing, T>::value, char>> : formatter<std::string>
	{
		auto format(const Thing& thing, format_context& ctx) const
		{
			return formatter<std::string>::format(thing.nickname(), ctx);
		}
	};
}