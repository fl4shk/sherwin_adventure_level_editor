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
	
	// Door sprites
	
	// st_door is used for warping around different parts of either
	// a SINGLE sublevel or MULTIPLE sublevels.  These get connected to
	// sublevel_entrance's during the whole level conversion process.
	st_door,
	
	
	// Enemy sprites
	st_snow_golem,
	
	// st_count is the amount of sprite types.  It is automatically updated
	// by the compiler.
	st_count,
	
} __attribute__((aligned(4)));


class sprite_type_helper
{
public:		// variables
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
	
	static inline bool sprite_type_exists( sprite_type the_sprite_type )
	{
		return ( ( the_sprite_type >= 0 )
			&& ( the_sprite_type < st_count ) );
	}
	
};


#endif		// sprite_type_stuff_hpp
