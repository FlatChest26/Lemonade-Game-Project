#pragma once

#ifndef LEMONADE_ENGINE_SRC_MOVABLE_H
#define LEMONADE_ENGINE_SRC_MOVABLE_H

#include "transforms.h"

/* A class that represents an object with a transform. Also provides useful utilities for working with said transform. */
class Movable
{
protected:
	Transform m_transform{};

public:
	Movable() :
		m_transform({ DEFAULT_POSITION, DEFAULT_SIZE, NORTH })
	{
	}

	Movable(Transform transform) :
		m_transform(transform)
	{
	}

	virtual ~Movable()
	{
	}

public:
	// -- Getters -- //

	// Transform //

	/* Returns a reference to this thing's transform.*/
	virtual Transform& transform() { return m_transform; }
	/* Returns a constant reference to this thing's transform.*/
	virtual const Transform& transform() const { return m_transform; }
	/* Returns a copy of this thing's transform.*/
	virtual Transform get_transform() const { return m_transform; }

	// Position

	/* Returns a reference to this thing's position.*/
	virtual Position& pos() { return m_transform.position; }
	/* Returns a constant reference to this thing's position.*/
	virtual const Position& pos() const { return m_transform.position; }
	/* Returns a copy of this thing's position.*/
	virtual Position get_pos() const { return m_transform.position; }

	/* Returns a reference to this thing's x position. */
	virtual Coord& posx() { return m_transform.position.x; }
	/* Returns a constant reference to this thing's x position. */
	virtual const Coord& posx() const { return m_transform.position.x; }
	/* Returns a copy of this thing's x position. */
	virtual Coord get_posx() const { return m_transform.position.x; }

	/* Returns a reference to this thing's y position. */
	virtual Coord& posy() { return m_transform.position.y; }
	/* Returns a constant reference to this thing's y position. */
	virtual const Coord& posy() const { return m_transform.position.y; }
	/* Returns a copy of this thing's y position. */
	virtual Coord get_posy() const { return m_transform.position.y; }

	/* Returns a reference to this thing's z position. */
	virtual Coord& posz() { return m_transform.position.z; }
	/* Returns a constant reference to this thing's z position. */
	virtual const Coord& posz() const { return m_transform.position.z; }
	/* Returns a copy of this thing's z position. */
	virtual Coord get_posz() const { return m_transform.position.z; }

	// Size

	/* Returns a reference to this thing's size.*/
	virtual Size& size() { return m_transform.size; }
	/* Returns a constant reference to this thing's size.*/
	virtual const Size& size() const { return m_transform.size; }
	/* Returns a copy of this thing's size.*/
	virtual Size get_size() const { return m_transform.size; }

	/* Returns a reference to this thing's width.*/
	virtual Length& width() { return m_transform.size.x; }
	/* Returns a constant reference to this thing's width.*/
	virtual const Length& width() const { return m_transform.size.x; }
	/* Returns a copy of this thing's width.*/
	virtual Length get_width() const { return m_transform.size.x; }

	/* Returns a reference to this thing's height.*/
	virtual Length& height() { return m_transform.size.y; }
	/* Returns a constant reference to this thing's height.*/
	virtual const Length& height() const { return m_transform.size.y; }
	/* Returns a copy of this thing's height.*/
	virtual Length get_height() const { return m_transform.size.y; }

	/* Returns a reference to this thing's depth.*/
	virtual Length& depth() { return m_transform.size.z; }
	/* Returns a constant reference to this thing's depth.*/
	virtual const Length& depth() const { return m_transform.size.z; }
	/* Returns a copy of this thing's depth.*/
	virtual Length get_depth() const { return m_transform.size.z; }

	// Direction

	/* Returns a reference to this thing's direction.*/
	virtual Direction& direction() { return transform().direction; }
	/* Returns a constant reference to this thing's direction.*/
	virtual const Direction& direction() const { return transform().direction; }
	/* Returns a copy of this thing's direction.*/
	virtual Direction get_direction() const { return direction(); }

public:
	// -- Setters -- //

	// Transform //

	/* Sets this thing's transform. */
	virtual void set_transform(Transform t) { m_transform = t; }
	/* Sets this thing's transform. */
	virtual void set_transform(Position p, Size s, Direction dir = Direction::NORTH) { return set_transform(Transform(p, s, dir)); }

	// Position

	/* Sets this thing's position. */
	virtual void set_position(Position position) { m_transform.position = position; }
	/* Sets this thing's position. */
	virtual void set_position(Coord x = 0, Coord y = 0, Coord z = 0) { return set_position(Position(x, y, z)); }

	// Size

	/* Sets this thing's size. */
	virtual void set_size(Size size) { m_transform.size = size; }
	/* Sets this thing's size. */
	virtual void set_size(Length width = 0, Length height = 0, Length depth = 0) { return set_size(Size(width, height, depth)); }

	// Direction

	/* Faces this thing in the specified direction. */
	virtual void face_direction(Direction dir) { m_transform.direction = dir; }
	/* Faces this thing in the specified direction. */
	virtual void face_direction(Position pos) { face_direction(transforms::pos_as_dir(pos)); }
	/* Faces this thing in the specified direction. */
	virtual void face_direction(Coord x = 0, Coord y = 0, Coord z = 0) { face_direction(transforms::pos_as_dir(x, y, z)); }

public:

	// -- Utility -- //

	/* Changes this thing's position by the specified amount. Returns true if the move is valid. */
	virtual bool move(Coord dx = 0, Coord dy = 0, Coord dz = 0)
	{
		posx() += dx; posy() += dy; posz() += dz;
		return true;
	}

	/* Changes this thing's position by the specified amount. Returns true if the move is valid. */
	virtual bool move(Position delta) { return move(delta.x, delta.y, delta.z); }
	/* Changes this thing's position by the specified amount. Returns true if the move is valid. */
	virtual bool move(Direction dir, Coord distance = 1) { return move((Position)transforms::dir_as_pos(dir) * distance); }

	/* Returns true if the point is inside this thing. */
	virtual bool contains_point(int x, int y, int z) const
	{
		return posx() <= x && x < posx() + (int)width() && posy() <= y && y < posy() + (int)height() && posz() <= z && z < posz() + (int)depth();
	}
	/* Returns true if the point is inside this thing. */
	virtual bool contains_point(Position pos) const { return contains_point(pos.x, pos.y, pos.z); }

	/* Returns the points inside this thing, with the top-left-lower corner at (0, 0, 0). */
	virtual std::vector<Position> get_offsets() const
	{
		if (m_transform.size == DEFAULT_SIZE) return { {0, 0, 0} };

		std::vector<Position> offsets;
		offsets.reserve((size_t)width() * (size_t)height() * (size_t)depth());

		for (int offset_z = 0; offset_z < (int)depth(); offset_z++)
			for (int offset_y = 0; offset_y < (int)height(); offset_y++)
				for (int offset_x = 0; offset_x < (int)width(); offset_x++)
					offsets.emplace_back(offset_x, offset_y, offset_z);

		return offsets;
	}

	/* Returns the points inside this thing. */
	virtual std::vector<Position> get_points() const
	{
		if (m_transform.size == DEFAULT_SIZE) return { m_transform.position };

		std::vector<Position> points;
		points.reserve((size_t)width() * (size_t)height() * (size_t)depth());

		for (int offset_z = 0; offset_z < (int)depth(); offset_z++)
			for (int offset_y = 0; offset_y < (int)height(); offset_y++)
				for (int offset_x = 0; offset_x < (int)width(); offset_x++)
					points.emplace_back(posx() + offset_x, posy() + offset_y, posz() + offset_z);

		return points;
	}
};

#endif // !LEMONADE_ENGINE_SRC_MOVABLE_H