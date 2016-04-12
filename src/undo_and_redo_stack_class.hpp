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


#ifndef undo_and_redo_stack_class_hpp
#define undo_and_redo_stack_class_hpp


#include "misc_includes.hpp"
#include "block_stuff.hpp"
#include "sprite_level_data_stuff.hpp"
//#include "vec2_class.hpp"


class undo_and_redo_action
{
public:		// variables
	action_type the_action_type;
	
	
	// Stuff for drawing blocks.
	struct block_drawing_stuff
	{
		vector< pair< block, vec2_u32> > prev_block_vec, curr_block_vec;
	} the_block_drawing_stuff;
	
	// Stuff for drawing a sprite.
	struct sprite_drawing_stuff
	{
		sprite_init_param_group_with_size prev_sprite_ipgws, 
			curr_sprite_ipgws;
	} the_sprite_drawing_stuff;
	
	// Stuff for modifying a sprite.
	struct sprite_modifying_stuff
	{
		sprite_init_param_group_with_size prev_sprite_ipgws,
			curr_sprite_ipgws;
	} the_sprite_modifying_stuff;
	
	
	// Stuff for erasing sprites.
	struct sprite_erasing_stuff
	{
		vector<sprite_init_param_group_with_size> prev_sprite_ipgws_vec;
	} the_sprite_erasing_stuff;
	
	
	
};

class undo_and_redo_stack
{
public:		// variables
	vector<undo_and_redo_action> action_vec;
	
	u32 curr_action_index;
	
public:		// functions
	
	
};




#endif		// undo_and_redo_stack_class_hpp
