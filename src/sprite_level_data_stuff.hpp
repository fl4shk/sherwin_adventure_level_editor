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
	
	// The initial in-level x coordinate, multiplied by 16
	u32 initial_block_grid_x_coord;
	
	// The initial in-level y coordinate, multiplied by 16
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
	// information into the sprite_init_param_group class.
	sprite_spawn_state spawn_state;
	
public:		// functions
	
	// This function is included so that an std::forward_list of
	// sprite_init_param_group's can be sorted in ascending order by the
	// std::forward_list::sort() method.  The sorting is to be done based
	// on the initial_block_grid_x_coord parameter, followed by the
	// initial_block_grid_y_coord parameter.
	inline bool operator < ( const sprite_init_param_group& other ) const
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







#endif		// sprite_level_data_stuff_hpp
