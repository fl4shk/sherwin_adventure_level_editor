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
	
	// How this works is that when blocks are overwritten, keep track of
	// what the blocks were before, and what they were replaced with.  This
	// should work with plain old drawing blocks, plain old erasing blocks,
	// moving a rectangular selection of blocks, cutting a rectangular
	// selection of blocks, and pasting a copied rectangular selection of
	// blocks.
	
	// use std::map so that there are no duplicates (there really don't
	// need to be any).
	map< vec2_s32, block > prev_block_map, curr_block_map;
	
	
public:		// functions
	inline undo_and_redo_action()
	{
	}
	inline undo_and_redo_action( const undo_and_redo_action& to_copy )
	{
		*this = to_copy;
	}
	
	void debug_print();
	
};


class undo_and_redo_stack
{
//public:		// variables
protected:		// variables
	vector<undo_and_redo_action> action_vec;
	
	// A u64 can only represent so many different actions, so over time
	// perhaps this could be a source of an error :/
	u64 curr_action_index;
	
public:		// functions
	undo_and_redo_stack();
	
	
	// This is just for good measure.
	u64 get_curr_action_index() const;
	
	undo_and_redo_action& get_curr_action();
	
	
	// If not at the end of action_vec, this chops off everything after the
	// current position.
	void add_action( const undo_and_redo_action& the_action );
	
	
	void debug_print();
	
	// Only one undo or redo at a time.  Returns whether the undo or redo
	// was successful, which is probably only useful for debugging
	// purposes.
	bool undo(  );
	bool redo(  );
	
	
};


#endif		// undo_and_redo_stack_class_hpp
