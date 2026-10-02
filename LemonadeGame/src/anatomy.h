#pragma once

#ifndef LEMONADE_GAME_SRC_ANATOMY_H
#define LEMONADE_GAME_SRC_ANATOMY_H

#include <algorithm>
#include <vector>

#include "body_part_type.h"
#include "body_part.h"
#include "body_plan.h"

struct BodyConnection
{
	BodyPart* parent;
	BodyPart* child;
	BodyPart::RelativePosition relative_position;
};

class Creature;

class Anatomy
{
protected:
	Creature* m_owner{};

	units::Length m_height{ AVERAGE_HEIGHT };
	units::Mass m_weight{ AVERAGE_WEIGHT };

	std::vector<std::unique_ptr<BodyPart>> m_body_parts{};
	std::vector<BodyConnection> m_connections{};
	std::vector<BodyConnection> m_relations{};

	BodyPart* root;

public:

	Anatomy(BodyPlan bd_plan, std::vector<BodyMod> bd_mods = {});

	BodyPart* add_body_part(const BodyPartPlan& bp_plan);
	bool add_body_connection(const BodyConnectionPlan& bc_plan);
	bool add_body_relation(const BodyConnectionPlan& bc_plan);

	bool remove_body_part(BodyPartID bp_ID);

	bool has_body_part_category(const BodyPartType::Category& category) const
	{
		return std::any_of(m_body_parts.begin(), m_body_parts.end(), [&](const auto& bp) { return bp->category() == category; });
	}

	bool has_body_part(const BodyPartID bp_ID) const
	{
		return std::any_of(m_body_parts.begin(), m_body_parts.end(), [&](const auto& bp) { return bp->ID() == bp_ID; });
	}

	bool has_body_part_type(BodyPartType* bp_type) const
	{
		return std::any_of(m_body_parts.begin(), m_body_parts.end(), [&](const auto& bp) { return bp->get_type() == bp_type; });
	}

	BodyPart* get_body_part_category(const BodyPartType::Category& category)
	{
		for (auto& bp : m_body_parts)
			if (bp->category() == category)
				return bp.get();

		return nullptr;
	}

	BodyPart* get_body_part(const BodyPartID bp_ID)
	{
		for (auto& bp : m_body_parts)
			if (bp->ID() == bp_ID)
				return bp.get();

		return nullptr;
	}

	BodyPart* get_body_part_type(BodyPartType* bp_type)
	{
		for (auto& bp : m_body_parts)
			if (bp->get_type() == bp_type)
				return bp.get();

		return nullptr;
	}
};

#endif // !LEMONADE_GAME_SRC_ANATOMY_H