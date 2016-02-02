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



#ifndef sprite_type_stuff_hpp
#define sprite_type_stuff_hpp


#include "misc_types.hpp"
#include "vec2_class.hpp"

#include <vector>
#include <string>
using namespace std;


// Adding, removing, or changing sprite types
enum sprite_type
{
	// The Default Sprite (also used for when there is no sprite in the
	// slot).
	st_default,
	
	// The Player
	st_player,
	
	// Powerup Sprites
	st_waffle,
	
	st_muffin,
	st_fire_muffin,
	st_ice_muffin,
	st_chocolate_muffin,
	
	// Block-like Sprites
	
	// st_door_# is used for warping around different parts of either
	// a SINGLE sublevel or MULTIPLE sublevels.  These get connected to
	// sublevel_entrance's during the whole level conversion process.
	st_door_0, st_door_1, st_door_2, st_door_3,
	st_door_4, st_door_5, st_door_6, st_door_7,
	st_door_8, st_door_9, st_door_10, st_door_11,
	st_door_12, st_door_13, st_door_14, st_door_15,
	st_door_16, st_door_17, st_door_18, st_door_19,
	st_door_20, st_door_21, st_door_22, st_door_23,
	st_door_24, st_door_25, st_door_26, st_door_27,
	st_door_28, st_door_29, st_door_30, st_door_31,
	
	// Enemy sprites
	st_snow_golem,
	
	// st_count is the amount of sprite types.  It is automatically updated
	// by the compiler.
	st_count,
	
} __attribute__((aligned(4)));


class sprite_type_helper
{
public:		// variables
	static constexpr sprite_type lowest_warp_id_st = st_door_0,
		highest_warp_id_st = st_door_31;
	
	static vector<string> st_name_vec;
	static const string st_unknown_str;
	
	//static vector<u32> st_height_vec;
	
public:		// functions
	
	static inline const string& get_st_name( sprite_type the_sprite_type )
	{
		if ( the_sprite_type < 0 || the_sprite_type >= st_count )
		{
			return st_name_vec.at(st_default);
		}
		return st_name_vec.at(the_sprite_type);
	}
	
	static inline const string& get_st_name_debug
		( sprite_type the_sprite_type )
	{
		if ( the_sprite_type < 0 || the_sprite_type >= st_count )
		{
			return st_unknown_str;
		}
		return st_name_vec.at(the_sprite_type);
	}
	
};


#endif		// sprite_type_stuff_hpp
