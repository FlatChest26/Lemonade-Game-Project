#pragma once

#include "snowy_grammar.h"
#include "renderable.h"
#include "snowy_database.h"
#include "anatomy.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct Species
{
	std::string ID;
	Noun noun{ "unknown creature", "unknown creatures" };
	std::string adjective{ noun.singular };

	Renderable renderable{ '@', color::get("cyan"), color::get("black") };

	BodyPlan default_body_plan{ body_plan::GenericBody };
	std::vector<BodyMod> body_mods;

	std::string description{ "A generic species." };

	bool operator==(const Species& other) const { return other.ID == ID; }

	static Species from_json(const json& j);
};

using SpeciesDB = snowy::SingletonDatabase<std::shared_ptr<Species>>;

namespace species
{
	inline Species HUMAN
	{
		.ID = "HUMAN",
		.noun = { "human", "humans" },
		.renderable = {'U', color::get("cyan"), color::get("black") }
	};

	static inline void initialize_species_db()
	{
		SpeciesDB::init();
	}
}
