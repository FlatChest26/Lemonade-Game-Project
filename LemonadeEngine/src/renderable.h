#pragma once

#ifndef LEMONADE_GAME_SRC_RENDERABLE_H
#define LEMONADE_GAME_SRC_RENDERABLE_H

#include <iostream>
#include <tuple>

#include "color.h"

inline constexpr glyph_t DISTANT_DOT = 250;
inline constexpr glyph_t PLAYER_CHAR = '@';

inline const color_t falling_bg = color::get( "light_cyan" );

typedef	std::tuple<glyph_t, color_t, color_t> TupleRenderable;

struct Renderable
{
	// -- Variables -- //

	glyph_t glyph { '?' };
	color_t fg { DEFAULT_FG_COLOR };
	color_t bg { DEFAULT_BG_COLOR };

	// -- Operators -- //

	constexpr bool operator==( const Renderable& other ) const
	{
		return glyph == other.glyph && fg == other.fg && bg == other.bg;
	}
	constexpr bool operator==( const glyph_t& other ) const
	{
		return glyph == other;
	}
	constexpr bool operator==( const color_t& other ) const
	{
		return fg == other;
	}

	constexpr Renderable& operator=( const Renderable& other )
	{
		glyph = other.glyph;
		fg = other.fg;
		bg = other.bg;

		return *this;
	}

	constexpr Renderable& operator=( const glyph_t& other )
	{
		glyph = other;
		return *this;
	}

	constexpr Renderable& operator=( const color_t& other )
	{
		fg = other;
		return *this;
	}

	constexpr Renderable& operator=( const TupleRenderable& other )
	{
		glyph = std::get<0>( other );
		fg = std::get<1>( other );
		bg = std::get<2>( other );

		return *this;
	}

	constexpr TupleRenderable as_tuple() const
	{
		return TupleRenderable( glyph, fg, bg );
	}
	constexpr operator TupleRenderable() const
	{
		return as_tuple();
	}
};


inline std::ostream& operator<<( std::ostream& os, const Renderable& renderable )
{
	return os << static_cast<char>( renderable.glyph );
}
inline std::wostream& operator<<( std::wostream& os, const Renderable& renderable )
{
	return os << static_cast<wchar_t>( renderable.glyph );
}

inline Renderable SHROUD( DEFAULT_GLPYH, DEFAULT_FG_COLOR, DEFAULT_BG_COLOR );

#endif //  !LEMONADE_GAME_SRC_RENDERABLE_H