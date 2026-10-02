#pragma once

#ifndef LEMONADE_GAME_SRC_CREATURE_DATA_H
#define LEMONADE_GAME_SRC_CREATURE_DATA_H

#include "thing.h"
#include "species.h"

/* Data about a creature that exists even when the creature is not on screen. */
// It is mutable and unique to each creature

class Creature;

class CreatureSoul : public Thing
{
protected:
	Creature* m_physical_form;

	ThingID m_ID;
	std::string m_description;
	Name m_name;
	Age m_age;
	Gender m_gender;
	Sexuality m_sexuality;
	SexualPosition m_sexual_position;
	color_t m_color;

public:

	constexpr bool is_creature() const override { return true; }
	constexpr CreatureSoul* as_creature() override { return this; }

	struct initializer
	{
		ThingID ID;
		Name name{};
		Age age{};
		Gender gender{};
		Sexuality sexuality{};
		SexualPosition sexual_position{};
		color_t color{ DEFAULT_FG_COLOR };
		std::string description;
	};

	CreatureSoul(const initializer& initializer = {});

	virtual ThingID ID() const override { return m_ID; }
	virtual std::string description() const override { return m_description; }

	// Physical Form

	Creature* get_physical_form() const;
	bool has_physical_form() const;

	// Name

	constexpr Name get_name() const override { return m_name; }

	// Age

	// How old this thing is by its physical body.
	constexpr int physical_age() const { return m_age.physical_age; }
	// How old this thing has existed in the world.
	constexpr int chronological_age() const { return m_age.chronological_age; }

	// Gender

	// What to call this thing in a gendered context. (e.g. "man", "woman", "person")
	constexpr Noun gender_noun() const { return m_gender.noun; }
	// What pronouns to use for this thing
	constexpr Pronouns get_pronouns() const override { return m_gender.pronouns; }

	bool is_neuter() const { return m_gender.gender_type & Gender::type::NEUTER; }
	bool is_epicene() const { return m_gender.gender_type & Gender::type::EPICENE; }
	bool is_masculine() const { return m_gender.gender_type & Gender::type::MASCULINE; }
	bool is_feminine() const { return m_gender.gender_type & Gender::type::FEMININE; }

	// Sexuality

	constexpr Noun sexuality_noun() const { return m_sexuality.noun; }
	bool can_be_attracted_to(CreatureSoul* other) const;

	// Sexual Position

	constexpr SexualPosition get_sexual_position() const { return m_sexual_position; }

	// Species & Renderable

	virtual const Species* get_species() const;
	virtual constexpr Noun species_noun() const;

	constexpr Noun get_noun() const override
	{
		if (species_noun().empty())
			return gender_noun();
		else
			return species_noun();
	}

	virtual Renderable get_renderable() const override;
	virtual color_t fg() const override { return m_color; }
};

inline bool has_compatible_attraction(CreatureSoul* first, CreatureSoul* second)
{
	return first->can_be_attracted_to(second) && second->can_be_attracted_to(first);
}

#endif // !LEMONADE_GAME_SRC_CREATURE_DATA_H