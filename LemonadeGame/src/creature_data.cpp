#include "creature_data.h"

CreatureData::CreatureData( ThingID ID, const initializer& init ):
	Thing( ID ),
	m_name( init.name ),
	m_age( init.age ),
	m_gender( init.gender ),
	m_sexuality( init.sexuality ),
	m_sexual_position( init.sexual_position ),
	m_species( init.species ),
	m_color( init.color )
{}

bool CreatureData::is_attracted_to( CreatureData* other ) const
{
	auto has_attraction = [&]( SexualAttraction attraction )
	{
		return m_sexuality.attraction & attraction;
	};


	if ( has_attraction( SexualAttraction::ANY ) )
	{
		return true;
	}
	if ( has_attraction( SexualAttraction::NONE ) )
	{
		return false;
	}

	/*
		if( has_attraction( SexualAttraction::OPPOSITE ) )
		{
			return ( is_male() && other->is_female() ) || ( is_female() && other->is_male() );
		}
		if( has_attraction( SexualAttraction::SAME ) )
		{
			return ( is_male() && other->is_male() ) || ( is_female() && other->is_female() );
		}
		if( has_attraction( SexualAttraction::MALE ) )
		{
			return other->is_male();
		}
		if( has_attraction( SexualAttraction::FEMALE ) )
		{
			return other->is_female();
		}
	*/

	if ( has_attraction( SexualAttraction::NEUTER ) )
	{
		return other->is_neuter();
	}
	if ( has_attraction( SexualAttraction::EPICENE ) )
	{
		return other->is_epicene();
	}
	if ( has_attraction( SexualAttraction::MASCULINE ) )
	{
		return other->is_masculine();
	}
	if ( has_attraction( SexualAttraction::FEMININE ) )
	{
		return other->is_feminine();
	}

	return false;
}

Renderable CreatureData::get_renderable() const
{
	Renderable renderable = m_species->renderable;

	if (is_named())
	{
		renderable.glyph = nickname()[0];
		if (get_sexual_position() & SexualPosition::SUB)
		{
			renderable.glyph = tolower(nickname()[0]);
		}
		else
		{
			renderable.glyph = toupper(nickname()[0]);
		}
	}

	return renderable;
}
