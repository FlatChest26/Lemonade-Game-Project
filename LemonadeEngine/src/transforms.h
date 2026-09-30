#pragma once

#ifndef LEMONADE_ENGINE_SRC_TRANSFORMS_H
#define LEMONADE_ENGINE_SRC_TRANSFORMS_H

#include <cstdint>
#include <iostream>
#include <tuple>
#include <math.h>
#include <vector>
#include <string>

#include "enum_traits.h"

template<typename T>
struct Vec3D
{
	using _Num_Type = T;
	using TupleVec3D = std::tuple<_Num_Type, _Num_Type, _Num_Type>;
	using TupleVec2D = std::tuple<_Num_Type, _Num_Type>;
	using Vec3DType = Vec3D<_Num_Type>;

public:
	// -- Variables -- //

	_Num_Type x { 0 }, y { 0 }, z { 0 };

public:
	// -- Constructors -- //

	constexpr Vec3D():
		x( static_cast<_Num_Type>( 0 ) ), y( static_cast<_Num_Type>( 0 ) ), z( static_cast<_Num_Type>( 0 ) )
	{}

	constexpr Vec3D( const _Num_Type& x, const _Num_Type& y, const _Num_Type& z ) :
		x( x ), y( y ), z( z )
	{}

	constexpr Vec3D( const TupleVec3D& vec3d ) :
		Vec3D( std::get<0>( vec3d ), std::get<1>( vec3d ), std::get<2>( vec3d ) )
	{}

	constexpr Vec3D( const _Num_Type& u ) :
		x( u ), y( u ), z( u )
	{}

	constexpr Vec3D( const Vec3DType& other ) :
		x( other.x ), y( other.y ), z( other.z )
	{}

	constexpr Vec3D( Vec3DType&& other ) noexcept:
		x( other.x ), y( other.y ), z( other.z )
	{}
public:
	// -- Getters -- //

	constexpr TupleVec3D as_tuple() const
	{
		return TupleVec3D( x, y, z );
	}
	constexpr TupleVec2D as_2d_tuple() const
	{
		return TupleVec2D( x, y );
	}

public:
	// Casts //

	constexpr operator TupleVec3D() const
	{
		return as_tuple();
	}
	constexpr operator TupleVec2D() const
	{
		return as_2d_tuple();
	}
	constexpr operator Vec3D<int32_t>() const
	{
		return { static_cast<int32_t>( x ), static_cast<int32_t>( y ), static_cast<int32_t>( z ) };
	}
	constexpr operator Vec3D<uint32_t>() const
	{
		return { static_cast<uint32_t>( x ), static_cast<uint32_t>( y ), static_cast<uint32_t>( z ) };
	}

public:
	// Equality //

	constexpr bool operator==( const Vec3DType& other ) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

public:
	// Not Equals //

	constexpr bool operator!=( const Vec3DType& other ) const
	{
		return x != other.x && y != other.y && z != other.z;
	}

public:
	// Equals Assignment //

	constexpr Vec3DType& operator=( const Vec3DType& other )
	{
		x = other.x; y = other.y; z = other.z;
		return *this;
	}

	template<typename T, typename U>
	constexpr Vec3D<T>& operator=( const U& other )
	{
		x = static_cast<T>( other );
		y = static_cast<T>( other );
		z = static_cast<T>( other );

		return *this;
	}

public:
	// Addition //

	constexpr Vec3DType operator+( const Vec3DType& other )
	{
		return  { x + other.x, y + other.y, z + other.z };
	}

	template<typename T, typename U>
	constexpr Vec3D<T> operator+( const U& other )
	{
		return  { x + static_cast<T>( other ), y + static_cast<T>( other ), z + static_cast<T>( other ) };
	}

public:
	// Addition Assignment //

	constexpr Vec3DType& operator+=( const Vec3DType& other )
	{
		x += other.x; y += other.y; z += other.z;
		return *this;
	}

	template<typename T, typename U>
	constexpr Vec3D<T>& operator+=( const U& other )
	{
		x += static_cast<T>( other );
		y += static_cast<T>( other );
		z += static_cast<T>( other );

		return *this;
	}

public:
	// Subtraction //

	constexpr Vec3DType operator-( const Vec3DType& other )
	{
		return  { x - other.x, y - other.y, z - other.z };
	}

	template<typename T, typename U>
	constexpr Vec3D<T> operator-( const U& other )
	{
		return  { x - static_cast<T>( other ), y - static_cast<T>( other ), z - static_cast<T>( other ) };
	}

public:
	// Subtraction Assignment //

	constexpr Vec3DType& operator-=( const Vec3DType& other )
	{
		x -= other.x; y -= other.y; z -= other.z;
		return *this;
	}

	template<typename T, typename U>
	constexpr Vec3D<T>& operator-=( const U& other )
	{
		x -= static_cast<T>( other );
		y -= static_cast<T>( other );
		z -= static_cast<T>( other );

		return *this;
	}

public:
	// Multiplication //

	constexpr Vec3DType operator*( const Vec3DType& other )
	{
		return  { x * other.x, y * other.y, z * other.z };
	}

	template<typename T, typename U>
	constexpr Vec3D<T> operator*( const U& other )
	{
		return  { x * static_cast<T>( other ), y * static_cast<T>( other ), z * static_cast<T>( other ) };
	}

public:
	// Multiplication Assignment //

	constexpr Vec3DType& operator*=( const Vec3DType& other )
	{
		x *= other.x; y *= other.y; z *= other.z;
		return *this;
	}

	template<typename T, typename U>
	constexpr Vec3D<T>& operator*=( const U& other )
	{
		x *= static_cast<T>( other );
		y *= static_cast<T>( other );
		z *= static_cast<T>( other );

		return *this;
	}

public:
	// Division / //

	constexpr Vec3DType operator/( const Vec3DType& other )
	{
		return  { x / other.x, y / other.y, z / other.z };
	}

	template<typename T, typename U>
	constexpr Vec3D<T> operator/( const U& other )
	{
		return  { x / static_cast<T>( other ), y / static_cast<T>( other ), z / static_cast<T>( other ) };
	}

public:
	// Division Assignment /= //

	constexpr Vec3DType& operator/=( const Vec3DType& other )
	{
		x /= other.x; y /= other.y; z /= other.z;
		return *this;
	}

	template<typename T, typename U>
	constexpr Vec3D<T>& operator/=( const U& other )
	{
		x /= static_cast<T>( other );
		y /= static_cast<T>( other );
		z /= static_cast<T>( other );

		return *this;
	}

public:
	// Modulus % //

	constexpr Vec3DType operator%( const Vec3DType& other )
	{
		return  { x % other.x, y % other.y, z % other.z };
	}

	template<typename T, typename U>
	constexpr Vec3D<T> operator%( const U& other )
	{
		return  { x % static_cast<T>( other ), y % static_cast<T>( other ), z % static_cast<T>( other ) };
	}

public:
	// Modulus Assignment %= //

	constexpr Vec3DType& operator%=( const Vec3DType& other )
	{
		x %= other.x; y %= other.y; z %= other.z;
		return *this;
	}

	template<typename T, typename U>
	constexpr Vec3D<T>& operator%=( const U& other )
	{
		x %= static_cast<T>( other );
		y %= static_cast<T>( other );
		z %= static_cast<T>( other );

		return *this;
	}

public:
	// Comparison //

	constexpr bool operator<( const Vec3DType& other )
	{
		return  x < other.x && y < other.y && z < other.z;
	}

	constexpr bool operator<=( const Vec3DType& other )
	{
		return x <= other.x && y <= other.y && z <= other.z;
	}

	constexpr bool operator>( const Vec3DType& other )
	{
		return  x > other.x && y > other.y && z > other.z;
	}

	constexpr bool operator>=( const Vec3DType& other )
	{
		return x >= other.x && y >= other.y && z >= other.z;
	}
};

template <typename VEC_TYPE = int32_t>
inline float vec3d_length( const Vec3D<VEC_TYPE>& vec3d, bool squared = false )
{
	if ( squared )
	{
		return std::fabs( static_cast<float>( vec3d.x * vec3d.x ) + static_cast<float>( vec3d.y * vec3d.y ) + static_cast<float>( vec3d.z * vec3d.z ) );
	}
	else
	{
		return std::fabs( std::sqrtf( static_cast<float>( vec3d.x * vec3d.x ) + static_cast<float>( vec3d.y * vec3d.y ) + static_cast<float>( vec3d.z * vec3d.z ) ) );
	}
}

// Absolute coordinate. 1 Coord = 1/24 ChunkCoord.
using Coord = int32_t;
// Absolute position. 1 Position = 1/24 ChunkPosition.
using Position = Vec3D<Coord>;
using TuplePosition = std::tuple<Coord, Coord, Coord>;

using Length = uint32_t;
using Size = Vec3D<Length>;
using TupleSize = std::tuple<Length, Length, Length>;

namespace std
{
	template <typename VEC_TYPE = int32_t>
	inline string to_string( const Vec3D<VEC_TYPE>& vec3d )
	{
		return "(" + to_string( vec3d.x ) + ", " + to_string( vec3d.y ) + ", " + to_string( vec3d.z ) + ")";
	}

	template <typename VEC_TYPE = int32_t>
	inline wstring to_wstring( const Vec3D<VEC_TYPE>& vec3d )
	{
		return "(" + to_wstring( vec3d.x ) + ", " + to_wstring( vec3d.y ) + ", " + to_wstring( vec3d.z ) + ")";
	}
}

template <typename VEC_TYPE = int32_t>
inline std::ostream& operator<<( std::ostream& os, const Vec3D<VEC_TYPE>& vec3d )
{
	return os << std::to_string<VEC_TYPE>( vec3d );
}

template <typename VEC_TYPE = int32_t>
inline std::wostream& operator<<( std::wostream& os, const Vec3D<VEC_TYPE>& vec3d )
{
	return os << std::to_wstring<VEC_TYPE>( vec3d );
};

enum Direction : uint8_t
{
	NO_DIRECTION = 0,

	NORTH = 1 << 0,
	SOUTH = 1 << 1,

	EAST = 1 << 2,
	WEST = 1 << 3,

	ABOVE = 1 << 4,
	BELOW = 1 << 5,

	NORTHEAST = NORTH | EAST,
	NORTHWEST = NORTH | WEST,

	SOUTHEAST = SOUTH | EAST,
	SOUTHWEST = SOUTH | WEST,

	NORTH_ABOVE = NORTH | ABOVE,
	NORTH_BELOW = NORTH | BELOW,
	NORTHEAST_ABOVE = NORTH | EAST | ABOVE,
	NORTHWEST_ABOVE = NORTH | WEST | ABOVE,
	NORTHEAST_BELOW = NORTH | EAST | BELOW,
	NORTHWEST_BELOW = NORTH | WEST | BELOW,

	SOUTH_ABOVE = SOUTH | ABOVE,
	SOUTH_BELOW = SOUTH | BELOW,
	SOUTHEAST_ABOVE = SOUTH | EAST | ABOVE,
	SOUTHWEST_ABOVE = SOUTH | WEST | ABOVE,
	SOUTHEAST_BELOW = SOUTH | EAST | BELOW,
	SOUTHWEST_BELOW = SOUTH | WEST | BELOW,

	EAST_ABOVE = EAST | ABOVE,
	EAST_BELOW = EAST | BELOW,

	WEST_ABOVE = WEST | ABOVE,
	WEST_BELOW = WEST | BELOW,
};

IS_FLAG_ENUM( Direction )

namespace transforms
{
	inline constexpr Position dir_as_pos( const Direction& dir )
	{
		return Position {
			( dir & WEST ) ? -1  : ( dir & EAST ) ? 1  : 0,
			( dir & NORTH ) ? -1 : ( dir & SOUTH ) ? 1 : 0,
			( dir & BELOW ) ? -1 : ( dir & ABOVE ) ? 1 : 0
		};
	}

	inline constexpr Direction pos_as_dir( const Position& pos )
	{
		return Direction {
			( pos.x < 0 ) ? WEST  : ( pos.x > 0 ) ? EAST  : NO_DIRECTION |
			( pos.y < 0 ) ? NORTH : ( pos.y > 0 ) ? SOUTH : NO_DIRECTION |
			( pos.z < 0 ) ? BELOW : ( pos.z > 0 ) ? ABOVE : NO_DIRECTION
		};
	}

	inline constexpr Direction pos_as_dir( const Coord& x, const Coord& y, const Coord& z )
	{
		return pos_as_dir( Position( x, y, z ) );
	}

	inline constexpr Direction a = pos_as_dir( 1, 0, -1 );
}

namespace std
{
	inline string to_string( Direction dir )
	{
		return
			string( dir & Direction::NORTH ? "north " : "" ) +
			string( dir & Direction::SOUTH ? "south " : "" ) +
			string( dir & Direction::WEST  ? "west " : "" ) +
			string( dir & Direction::EAST  ? "east " : "" ) +
			string( dir & Direction::ABOVE ? "above" : "" ) +
			string( dir & Direction::BELOW ? "below" : "" );
	}

	inline ostream& operator<<( ostream& os, Direction dir )
	{
		return os << to_string( dir );
	}
}

inline constexpr Position DEFAULT_POSITION = Position { 0, 0, 0 };
inline constexpr Size DEFAULT_SIZE = Size { 1, 1, 1 };
inline constexpr Direction DEFAULT_DIRECTION = NO_DIRECTION;

struct Transform
{
public:
	// -- Variables -- //
	Position position { DEFAULT_POSITION };
	Size size { DEFAULT_SIZE };
	Direction direction { DEFAULT_DIRECTION };

	constexpr Transform():
		position( DEFAULT_POSITION ), size( DEFAULT_SIZE ), direction( DEFAULT_DIRECTION )
	{}

	constexpr Transform( const Position& position, const Size& size, const Direction& direction ) :
		position( position ), size( size ), direction( direction )
	{}

	constexpr Transform( const Position& position, const Size& size ) :
		Transform( position, size, DEFAULT_DIRECTION )
	{}

	constexpr Transform( const Position& position, const Direction& direction ) :
		Transform( position, DEFAULT_SIZE, direction )
	{}

	constexpr Transform( const Size& size, const Direction& direction ) :
		Transform( DEFAULT_POSITION, size, direction )
	{}

	constexpr Transform( const Position& position ) :
		Transform( position, DEFAULT_SIZE, DEFAULT_DIRECTION )
	{}

	constexpr Transform( const Size& size ) :
		Transform( DEFAULT_POSITION, size, DEFAULT_DIRECTION )
	{}

	constexpr Transform( const Direction& direction ) :
		Transform( DEFAULT_POSITION, DEFAULT_SIZE, direction )
	{}

	constexpr bool operator==( const Transform& other ) const
	{
		return position == other.position && size == other.size && direction == other.direction;
	}

	constexpr Transform& operator=( const Transform& other )
	{
		position = other.position;
		size = other.size;
		direction = other.direction;

		return *this;
	}
};

namespace transforms
{
	inline constexpr  std::vector<Position> get_surrounding_directions( bool include_z = false )
	{
		if ( include_z )
		{
			return std::vector<Position>
			{
				Position( 1, 0, -1 ),
					Position( 1, 1, -1 ),
					Position( 0, 1, -1 ),
					Position( -1, 1, -1 ),
					Position( -1, 0, -1 ),
					Position( -1, -1, -1 ),
					Position( 0, -1, -1 ),
					Position( 1, -1, -1 ),

					Position( 1, 0, 0 ),
					Position( 1, 1, 0 ),
					Position( 0, 1, 0 ),
					Position( -1, 1, 0 ),
					Position( -1, 0, 0 ),
					Position( -1, -1, 0 ),
					Position( 0, -1, 0 ),
					Position( 1, -1, 0 ),

					Position( 1, 0, 1 ),
					Position( 1, 1, 1 ),
					Position( 0, 1, 1 ),
					Position( -1, 1, 1 ),
					Position( -1, 0, 1 ),
					Position( -1, -1, 1 ),
					Position( 0, -1, 1 ),
					Position( 1, -1, 1 )
			};
		}
		else
		{
			return std::vector<Position>
			{
				Position( 1, 0, 0 ),
					Position( 1, 1, 0 ),
					Position( 0, 1, 0 ),
					Position( -1, 1, 0 ),
					Position( -1, 0, 0 ),
					Position( -1, -1, 0 ),
					Position( 0, -1, 0 ),
					Position( 1, -1, 0 )
			};
		}
	}

	inline constexpr std::vector<Position> get_surrounding_locations( Position origin, bool include_z = false )
	{
		if ( include_z )
		{
			return std::vector<Position>
			{
				origin + Position( 1, 0, -1 ),
					origin + Position( 1, 1, -1 ),
					origin + Position( 0, 1, -1 ),
					origin + Position( -1, 1, -1 ),
					origin + Position( -1, 0, -1 ),
					origin + Position( -1, -1, -1 ),
					origin + Position( 0, -1, -1 ),
					origin + Position( 1, -1, -1 ),

					origin + Position( 1, 0, 0 ),
					origin + Position( 1, 1, 0 ),
					origin + Position( 0, 1, 0 ),
					origin + Position( -1, 1, 0 ),
					origin + Position( -1, 0, 0 ),
					origin + Position( -1, -1, 0 ),
					origin + Position( 0, -1, 0 ),
					origin + Position( 1, -1, 0 ),

					origin + Position( 1, 0, 1 ),
					origin + Position( 1, 1, 1 ),
					origin + Position( 0, 1, 1 ),
					origin + Position( -1, 1, 1 ),
					origin + Position( -1, 0, 1 ),
					origin + Position( -1, -1, 1 ),
					origin + Position( 0, -1, 1 ),
					origin + Position( 1, -1, 1 )
			};
		}
		else
		{
			return std::vector<Position>
			{
				origin + Position( 1, 0, 0 ),
					origin + Position( 1, 1, 0 ),
					origin + Position( 0, 1, 0 ),
					origin + Position( -1, 1, 0 ),
					origin + Position( -1, 0, 0 ),
					origin + Position( -1, -1, 0 ),
					origin + Position( 0, -1, 0 ),
					origin + Position( 1, -1, 0 )
			};
		}
	}
}

#endif // !LEMONADE_ENGINE_SRC_TRANSFORMS_H