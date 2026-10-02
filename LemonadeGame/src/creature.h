#pragma once

#ifndef LEMONADE_GAME_SRC_CREATURE_H
#define LEMONADE_GAME_SRC_CREATURE_H

#include "entity.h"
#include "creature_soul.h"
#include "anatomy.h"
#include "species.h"

extern const EntityFlags DEFAULT_CREATURE_FLAGS;

class Creature : public Entity
{
protected:
	bool m_alive{ true };
	std::unique_ptr<Anatomy> m_anatomy{ nullptr };
	const Species* m_species;

public:
	struct initializer
	{
		CreatureSoul* soul{ nullptr };
		const Species* species{ nullptr };

		std::vector<BodyMod> body_mods;

		Transform transform = { DEFAULT_POSITION, DEFAULT_SIZE, NORTH };
	};

public:
	Creature(const initializer& init);

	virtual constexpr bool is_creature() const override { return true; }
	virtual constexpr Creature* as_creature() override { return this; }

	// -- Stats -- //

	virtual size_t default_inventory_slots() const override;
	virtual TimeUnit speed() const override;

	// -- Getters -- //

	virtual constexpr const Species* get_species() const { return m_species; }
	virtual constexpr Noun species_noun() const { return m_species->noun; }

	virtual Anatomy* get_anatomy() const { return m_anatomy.get(); }

	// -- Checks -- //

	constexpr bool is_alive() const { return m_alive; }
	constexpr bool is_dead() const { return !m_alive; }
};

#endif // !LEMONADE_GAME_SRC_CREATURE_H