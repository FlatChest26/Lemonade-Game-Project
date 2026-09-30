#pragma once

#include "snowy_grammar.h"
#include "renderable.h"
#include "snowy_database.h"


struct Species
{
	const char* ID;
	Noun noun;
	Renderable renderable;

	bool operator==( const Species& other ) const { return other.ID == ID; }
};

namespace species
{
	inline Species HUMAN
	{
		.ID = "HUMAN",
		.noun = { "human", "humans" },
		.renderable = {'U', color::get( "cyan" ), color::get( "black" ) }
	};

	inline Species DEMON
	{
		.ID = "DEMON",
		.noun = { "demon", "demons" },
		.renderable = {'&', color::get( "dark_red" ), color::get( "black" )}
	};

	inline Species ANGEL
	{
		.ID = "ANGEL",
		.noun = { "angel", "angels" },
		.renderable = {142, color::get( "yellow" ), color::get( "black" ) }
	};

	inline Species DRAGONBORN
	{
		.ID = "DRAGONBORN",
		.noun = { "dragonborn", "dragonborns" },
		.renderable = {'d', { 0xFF, 0x80, 0x00 }, color::get( "black" ) }
	};

	inline snowy::Database<Species> species_db
	{
		{ "HUMAN",		HUMAN		},
		{ "DEMON",		DEMON		},
		{ "ANGEL",		ANGEL		},
		{ "DRAGONBORN", DRAGONBORN	},
	};

	inline constexpr Species get( const snowy::StringID& species_name )
	{
		if ( !species_db.contains( species_name ) )
		{
			return HUMAN;
		}

		return species_db[species_name];
	}
}
