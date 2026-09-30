#pragma once

#ifndef SAI_SRC_COLORS_H
#define SAI_SRC_COLORS_H

#include <string>
#include <libtcod/color.h>

#include "snowy_database.h"

typedef TCOD_color_t	color_t;
typedef int				glyph_t;

#define DEFAULT_FG_COLOR	color::get("white")
#define DEFAULT_BG_COLOR	color::get("black")
constexpr glyph_t DEFAULT_GLPYH = ' ';

namespace color
{
	inline constexpr size_t COLORS_AMOUNT = 25;

	inline snowy::StaticDatabase<color_t, COLORS_AMOUNT> colors_db
	{
		{ "black",			{ 0x00, 0x00, 0x00 } },
		{ "white",			{ 0xFF, 0xFF, 0xFF } },
		{ "gray",			{ 0x80, 0x80, 0x80 } },
		{ "dark_gray",		{ 0x34, 0x34, 0x34 } },
		{ "light_gray",		{ 0xB6, 0xB6, 0xB6 } },
		{ "red",			{ 0xFF, 0x00, 0x00 } },
		{ "dark_red",		{ 0x80, 0x00, 0x00 } },
		{ "light_red",		{ 0xFF, 0x80, 0x80 } },
		{ "green",			{ 0x00, 0xFF, 0x00 } },
		{ "dark_green",		{ 0x00, 0x80, 0x00 } },
		{ "light_green",	{ 0x80, 0xFF, 0x80 } },
		{ "blue",			{ 0x00, 0x00, 0xFF } },
		{ "dark_blue",		{ 0x00, 0x00, 0x80 } },
		{ "light_blue",		{ 0x80, 0x80, 0xFF } },
		{ "yellow",			{ 0xFF, 0xFF, 0x00 } },
		{ "brown",			{ 0x80, 0x80, 0x00 } },
		{ "light_yellow",	{ 0xFF, 0xFF, 0x80 } },
		{ "magenta",		{ 0xFF, 0x00, 0xFF } },
		{ "dark_magenta",	{ 0x80, 0x00, 0x80 } },
		{ "light_magenta",	{ 0xFF, 0x80, 0xFF } },
		{ "cyan",			{ 0x00, 0xFF, 0xFF } },
		{ "dark_cyan",		{ 0x00, 0x80, 0x80 } },
		{ "light_cyan",		{ 0x80, 0xFF, 0xFF } },
		{ "unexplored",		{ 0, 0, 0 } },
		{ "explored",		{ 50, 45, 40 } },
	};

	inline constexpr color_t get( const snowy::StringID& color_name )
	{
		if ( !colors_db.contains( color_name ) )
		{
			CERR("Unrecognized color: " << color_name);
			return DEFAULT_FG_COLOR;
		}

		return colors_db[color_name];
	}

	inline constexpr std::string get_name_of_color( const color_t& color )
	{
		if ( !colors_db.contains( color ) )
		{
			return "<undefined color>";
		}

		return colors_db.find_key( color );
	}
}

#endif // !SAI_SRC_COLORS_H