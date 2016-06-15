// This file is part of Sherwin's Adventure Level Editor.
// 
// Copyright 2015-2016 by Andrew Clark (FL4SHK).
// 
// Sherwin's Adventure Level Editor is free software: you can redistribute
// it and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
// 
// Sherwin's Adventure Level Editor is distributed in the hope that it will
// be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with Sherwin's Adventure Level Editor.  If not, see
// <http://www.gnu.org/licenses/>.



#ifndef sprite_level_data_stuff_hpp
#define sprite_level_data_stuff_hpp


#include "sprite_type_stuff.hpp"

#include <array>
using namespace std;

enum sprite_spawn_state
{
	sss_not_active,
	sss_active,
	sss_dead
	
} __attribute__((aligned(4)));



class sprite_init_param_group
{
public:		// variables
	// The type of sprite
	sprite_type type;
	
	// The initial in-level x coordinate, divided by 16
	u32 initial_block_grid_x_coord;
	
	// The initial in-level y coordinate, divided by 16
	u32 initial_block_grid_y_coord;
	
	// Here is a s
	bool facing_right;
	
	// Here are extra parameters that are used in different ways depending
	// on the type of sprite.
	u32 extra_param_0;
	u32 extra_param_1;
	u32 extra_param_2;
	u32 extra_param_3;
	
	// I figure that it's best to merge the despawned/spawned/dead
	// information into the sprite_init_param_group class.  The default
	// value is "sss_not_active", which is what is wanted.
	sprite_spawn_state spawn_state;
	
public:		// functions
	inline sprite_init_param_group() : type(st_default),
		initial_block_grid_x_coord(0), initial_block_grid_y_coord(0),
		facing_right(false), extra_param_0(0), extra_param_1(0),
		extra_param_2(0), extra_param_3(0),
		spawn_state(sss_not_active)
	{
	}
	
	inline sprite_init_param_group
		( const sprite_init_param_group& to_copy ) : type(to_copy.type),
		initial_block_grid_x_coord(to_copy.initial_block_grid_x_coord), 
		initial_block_grid_y_coord(to_copy.initial_block_grid_y_coord),
		facing_right(to_copy.facing_right),
		extra_param_0(to_copy.extra_param_0), 
		extra_param_1(to_copy.extra_param_1),
		extra_param_2(to_copy.extra_param_2), 
		extra_param_3(to_copy.extra_param_3),
		spawn_state(to_copy.spawn_state)
	{
	}
	
	inline sprite_init_param_group( const sprite_type& s_type, 
		u32 s_initial_block_grid_x_coord, u32 s_initial_block_grid_y_coord,
		bool s_facing_right, u32 s_extra_param_0, u32 s_extra_param_1,
		u32 s_extra_param_2, u32 s_extra_param_3, 
		const sprite_spawn_state& s_spawn_state ) : type(s_type),
		initial_block_grid_x_coord(s_initial_block_grid_x_coord),
		initial_block_grid_y_coord(s_initial_block_grid_y_coord),
		facing_right(s_facing_right), extra_param_0(s_extra_param_0),
		extra_param_1(s_extra_param_1), extra_param_2(s_extra_param_2),
		extra_param_3(s_extra_param_3), spawn_state(s_spawn_state)
	{
	}
	
	
	inline sprite_init_param_group& operator = 
		( const sprite_init_param_group& to_copy )
	{
		type = to_copy.type;
		initial_block_grid_x_coord = to_copy.initial_block_grid_x_coord;
		initial_block_grid_y_coord = to_copy.initial_block_grid_y_coord;
		facing_right = to_copy.facing_right;
		extra_param_0 = to_copy.extra_param_0;
		extra_param_1 = to_copy.extra_param_1;
		extra_param_2 = to_copy.extra_param_2;
		extra_param_3 = to_copy.extra_param_3;
		spawn_state = to_copy.spawn_state;
		
		return *this;
	}
	
	// This function is included so that an std::forward_list of
	// sprite_init_param_group's can be sorted in ascending order by the
	// std::forward_list::sort() method.  The sorting is to be done based
	// on the initial_block_grid_x_coord parameter, followed by the
	// initial_block_grid_y_coord parameter.
	virtual inline bool operator < 
		( const sprite_init_param_group& other ) const
	{
		if ( initial_block_grid_x_coord 
			< other.initial_block_grid_x_coord)
		{
			return true;
		}
		else if ( initial_block_grid_x_coord
			> other.initial_block_grid_x_coord )
		{
			return false;
		}
		//else if ( initial_block_grid_x_coord 
		//	== other.initial_block_grid_x_coord )
		//{
		//	return ( initial_block_grid_y_coord 
		//		< other.initial_block_grid_y_coord );
		//}
		
		
		return ( initial_block_grid_y_coord 
			< other.initial_block_grid_y_coord );
	}
	
	
} __attribute__((aligned(4)));

// This is a sprite_init_param_group with size
class sprite_ipgws : public sprite_init_param_group
{
public:		// variables
	vec2_u32 size_2d;
	
public:		// functions
	inline sprite_ipgws() : sprite_init_param_group(),
		size_2d(vec2_u32())
	{
	}
	
	inline sprite_ipgws
		( const sprite_ipgws& to_copy )
		: sprite_init_param_group( to_copy.type,
		to_copy.initial_block_grid_x_coord,
		to_copy.initial_block_grid_y_coord, to_copy.facing_right,
		to_copy.extra_param_0, to_copy.extra_param_1,
		to_copy.extra_param_2, to_copy.extra_param_3, 
		to_copy.spawn_state ), size_2d(to_copy.size_2d)
	{
	}
	
	
	inline sprite_ipgws( const sprite_type& s_type, 
		u32 s_initial_block_grid_x_coord, u32 s_initial_block_grid_y_coord,
		bool s_facing_right, u32 s_extra_param_0, u32 s_extra_param_1,
		u32 s_extra_param_2, u32 s_extra_param_3, 
		const sprite_spawn_state& s_spawn_state, 
		const vec2_u32& s_size_2d )
		: sprite_init_param_group( s_type, s_initial_block_grid_x_coord, 
		s_initial_block_grid_y_coord, s_facing_right, s_extra_param_0, 
		s_extra_param_1, s_extra_param_2, s_extra_param_3, s_spawn_state ), 
		size_2d(s_size_2d)
	{
	}
	
	inline sprite_ipgws& operator = 
		( const sprite_ipgws& to_copy )
	{
		sprite_init_param_group::operator = ( sprite_init_param_group
			( to_copy.type, to_copy.initial_block_grid_x_coord, 
			to_copy.initial_block_grid_y_coord, to_copy.facing_right,
			to_copy.extra_param_0, to_copy.extra_param_1,
			to_copy.extra_param_2, to_copy.extra_param_3,
			to_copy.spawn_state ) );
		
		size_2d = to_copy.size_2d;
		
		return *this;
	}
	
	
} __attribute__((aligned(4)));

// needed for undo_and_redo_action's unordered_set<sprite_ipgws>'s
namespace std
{
	template <>
	struct hash<sprite_ipgws>
	{
		size_t operator () ( const sprite_ipgws& to_hash ) const
		{
			const size_t hash_block_grid_coord_x
				( hash<u32>()(to_hash.initial_block_grid_x_coord) );
			const size_t hash_block_grid_coord_y
				( hash<u32>()(to_hash.initial_block_grid_y_coord) );
			
			return hash_block_grid_coord_x 
				^ ( hash_block_grid_coord_y << 1);
		}
	};
}


// Note:  for st_door, extra_param_0 is used to designate the destination
// sublevel_entrance index, and extra_param_1 is used to designate the
// destination sublevel index.  Also, while this is probably not necessary
// to put into the game itself, extra_param_2 is used to designate to WHICH
// NUMBER door a sprite_init_param_group corresponds.



class sublevel;



#define list_of_16x16_sprite_pg_stuff(macro) \
	macro(up_left) macro(up) \
	macro(left) macro(origin)
#define list_of_16x16_sprite_pg_stuff_no_origin(macro) \
	macro(up_left) macro(up) \
	macro(left) 

#define list_of_16x32_sprite_pg_stuff(macro) \
	macro(up_left) macro(up) \
	macro(left) macro(origin) \
	macro(down_left) macro(down)
#define list_of_16x32_sprite_pg_stuff_no_origin(macro) \
	macro(up_left) macro(up) \
	macro(left) \
	macro(down_left) macro(down)

#define list_of_32x32_sprite_pg_stuff(macro) \
	macro(up_left) macro(up) macro(up_right) \
	macro(left) macro(origin) macro(right) \
	macro(down_left) macro(down) macro(down_right)
#define list_of_32x32_sprite_pg_stuff_no_origin(macro) \
	macro(up_left) macro(up) macro(up_right) \
	macro(left) macro(right) \
	macro(down_left) macro(down) macro(down_right)



class adj_sprite_ipgws_ptr_group_base
{
public:		// variables
	// The TOP LEFT block grid coordinate INTERSECTED BY THE SPRITE is
	// considered to be the origin.  In this case, the origin is the in the
	// CENTER of the checked block grid coordinates.
	sprite_ipgws * up_left_ptr, * up_ptr, * up_right_ptr,
		* left_ptr, * origin_ptr, * right_ptr, 
		* down_left_ptr, * down_ptr, * down_right_ptr;
	
public:		// functions
	inline adj_sprite_ipgws_ptr_group_base() 
		: up_left_ptr(NULL), up_ptr(NULL), up_right_ptr(NULL),
		left_ptr(NULL), origin_ptr(NULL), right_ptr(NULL), 
		down_left_ptr(NULL), down_ptr(NULL), down_right_ptr(NULL)
	{
	}
	
	virtual bool can_add_sprite() = 0;
	virtual void erase_overlapping_sprites() = 0;
	
	
	inline bool overlaps_up_left() const
	{
		if ( up_left_ptr != NULL )
		{
			if ( up_left_ptr->size_2d.x > 16 
				&& up_left_ptr->size_2d.y > 16 )
			{
				return true;
			}
		}
		
		return false;
	}
	inline bool overlaps_up() const
	{
		if ( up_ptr != NULL )
		{
			if ( up_ptr->size_2d.y > 16 )
			{
				return true;
			}
		}
		
		return false;
	}
	inline bool overlaps_up_right() const
	{
		if ( up_right_ptr != NULL )
		{
			if ( up_right_ptr->size_2d.y > 16 )
			{
				return true;
			}
		}
		
		return false;
	}
	
	
	inline bool overlaps_left() const
	{
		if ( left_ptr != NULL )
		{
			if ( left_ptr->size_2d.x > 16 )
			{
				return true;
			}
		}
		
		return false;
	}
	inline bool overlaps_origin() const
	{
		if ( origin_ptr != NULL )
		{
			if ( origin_ptr->type != st_default )
			{
				return true;
			}
		}
		
		return false;
	}
	inline bool overlaps_right() const
	{
		if ( right_ptr != NULL )
		{
			if ( right_ptr->type != st_default )
			{
				return true;
			}
		}
		
		return false;
	}
	
	
	inline bool overlaps_down_left() const
	{
		if ( down_left_ptr != NULL )
		{
			if ( down_left_ptr->size_2d.x > 16 )
			{
				return true;
			}
		}
		
		return false;
	}
	inline bool overlaps_down() const
	{
		if ( down_ptr != NULL )
		{
			if ( down_ptr->type != st_default )
			{
				return true;
			}
		}
		
		return false;
	}
	inline bool overlaps_down_right() const
	{
		if ( down_right_ptr != NULL )
		{
			if ( down_right_ptr->type != st_default )
			{
				return true;
			}
		}
		
		return false;
	}
	
	
};


// These go from top left to bottom right, and they are generated using
// block grid coordinates.  Only one sprite should be permitted per block
// grid coordinate.  These classes are intended to be used to help
// overwrite data in a sublevel's 2D vector of
// sprite_ipgws.

// "adj" is short for "adjacent"

// Also, it is not necessary for EVERY adjacent position to be checked,
// only the ones occupied by the sprite and the ones to the top and left of
// the sprite.

// PLEASE set pointers to NULL if they represent a block grid coordinate
// that is OUTSIDE THE SUBLEVEL.


// This is used when it is necessary to determine whether a 16x16 sprite
// can be placed, since only one sprite is allowed per block grid
// coordinate.

// Schematic, where X is the 16x16 sprite
// [ ][ ]
// [ ][X]


class adj_sprite_ipgws_ptr_group_16x16
	: public adj_sprite_ipgws_ptr_group_base
{
public:		// functions
	inline adj_sprite_ipgws_ptr_group_16x16()
		: adj_sprite_ipgws_ptr_group_base()
	{
	}
	
	adj_sprite_ipgws_ptr_group_16x16( sublevel& the_sublevel, 
		u32 origin_block_grid_x_coord, u32 origin_block_grid_y_coord );
	
	virtual bool can_add_sprite();
	virtual void erase_overlapping_sprites();
	
};



// This is used when it is necessary to determine whether a 16x32 sprite
// can be placed, since only one sprite is allowed per block grid
// coordinate.

// Schematic, where X is the 16x32 sprite
// [ ][ ]
// [ ][X]
// [ ][X]

class adj_sprite_ipgws_ptr_group_16x32 
	: public adj_sprite_ipgws_ptr_group_base
{
public:		// functions
	inline adj_sprite_ipgws_ptr_group_16x32()
		: adj_sprite_ipgws_ptr_group_base()
	{
	}
	
	adj_sprite_ipgws_ptr_group_16x32( sublevel& the_sublevel, 
		u32 origin_block_grid_x_coord, u32 origin_block_grid_y_coord );
	
	virtual bool can_add_sprite();
	virtual void erase_overlapping_sprites();
	
};


// This is used when it is necessary to determine whether a 32x32 sprite
// can be placed, since only one sprite is allowed per block grid
// coordinate.

// Schematic, where X is the 32x32 sprite
// [ ][ ][ ]
// [ ][X][X]
// [ ][X][X]

class adj_sprite_ipgws_ptr_group_32x32
	: public adj_sprite_ipgws_ptr_group_base
{
public:		// functions
	inline adj_sprite_ipgws_ptr_group_32x32() 
		: adj_sprite_ipgws_ptr_group_base()
	{
	}
	
	adj_sprite_ipgws_ptr_group_32x32( sublevel& the_sublevel, 
		u32 origin_block_grid_x_coord, u32 origin_block_grid_y_coord );
	
	virtual bool can_add_sprite();
	virtual void erase_overlapping_sprites();
	
	
};





// This is used when it is necessary to determine the block grid position
// of a sprite that was clicked.

// Schematic.  Any of these could have been the postion of the sprite that
// was pressed.  X is the block grid position that was actually clicked.
// [ ][ ]
// [ ][X]

class adj_sprite_ipgws_ptr_group_for_selecting_sprite
{
public:		// variables
	// origin_ptr is a pointer to whichever sprite was actually clicked
	sprite_ipgws * up_left_ptr, * up_right_ptr, 
		* down_left_ptr, * down_right_ptr, * origin_ptr;
	
	
public:		// functions
	inline adj_sprite_ipgws_ptr_group_for_selecting_sprite()
		: up_left_ptr(NULL), up_right_ptr(NULL),
		down_left_ptr(NULL), down_right_ptr(NULL),
		origin_ptr(NULL)
	{
	}
	
	adj_sprite_ipgws_ptr_group_for_selecting_sprite
		( sublevel& the_sublevel, u32 mouse_pos_block_grid_x_coord, 
		u32 mouse_pos_block_grid_y_coord );
	
};


#endif		// sprite_level_data_stuff_hpp
