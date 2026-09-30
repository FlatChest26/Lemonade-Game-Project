#pragma once

#include "thing.h"
#include "species.h"

class CreatureData : public Thing
{
protected:

	Name m_name;
	Age m_age;
	Gender m_gender;
	Sexuality m_sexuality;
	SexualPosition m_sexual_position;
	Species* m_species;
	color_t m_color;

public:

	constexpr bool is_creature() const override { return true; }
	constexpr CreatureData* as_creature() override { return this; }

	struct initializer
	{
		Name name {};
		Age age {};
		Gender gender {};
		Sexuality sexuality {};
		SexualPosition sexual_position {};
		Species* species {};
		color_t color { DEFAULT_FG_COLOR };
		//Anatomy body{};	
	};

	CreatureData( ThingID ID, const initializer& initializer = {} );

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
	bool is_attracted_to(CreatureData* other ) const;

	// Sexual Position
	
	constexpr SexualPosition get_sexual_position() const { return m_sexual_position; }

	// Species & Renderable

	constexpr Noun species_noun() const { return m_species->noun; }
	constexpr Noun get_noun() const override { return m_species->noun; }

	virtual Renderable get_renderable() const override;
	virtual color_t fg() const override { return m_color; }

};

inline bool has_compatible_attraction( CreatureData* first, CreatureData* second )
{
	return first->is_attracted_to( second ) && second->is_attracted_to( first );
}
