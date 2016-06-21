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
#include "vec2_class.hpp"

class sprite_ipgws;

class undo_and_redo_action
{
public:		// variables
	action_type the_action_type;
	
	
	//bool test_bool;
	
	// Stuff for drawing blocks, cutting blocks, moving blocks without
	// pasting them, and pasting blocks.
	
	// How this works is that when blocks are overwritten, keep track of
	// what the blocks were before, and what they were replaced with.  This
	// should work with plain old drawing blocks, plain old erasing blocks,
	// moving a rectangular selection of blocks, cutting a rectangular
	// selection of blocks, and pasting a copied rectangular selection of
	// blocks.
	
	// use std::unordered_map so that there are no duplicates (there really
	// don't need to be any).
	
	// old_block_umap consists of blocks that were moved but not pasted,
	// with their original locations
	
	// Also note that old_block_umap is used to keep track of removed
	// blocks when a sublevel is resized and shrunk horizontally,
	// vertically, or both
	unordered_map< vec2_s32, block > old_block_umap, replaced_block_umap, 
		new_block_umap;
	
	
	// Stuff for drawing or modifying a single sprite.
	// old_sprite_ipgws is only used for when a single sprite is modified.
	sprite_ipgws old_sprite_ipgws, new_sprite_ipgws;
	
	
	
	// Stuff for moving blocks or sprites
	
	// The selection rect before it was moved
	sf::IntRect selection_rect_before_moving;
	
	
	// The sprites that were moved (with their original
	// initial_block_grid_x_coord and initial_block_grid_y_coord), the
	// sprites that were replaced, and the sprites that were moved or
	// pasted.  
	// Also note that old_sprite_ipgws_uset is used to keep track of
	// removed sprite_ipgws's when a sublevel is resized and shrunk
	// horizontally, vertically, or both
	unordered_set<sprite_ipgws> old_sprite_ipgws_uset, 
		replaced_sprite_ipgws_uset, new_sprite_ipgws_uset;
	
	//unordered_set<sprite_ipgws> test_sprite_ipgws_uset;
	
	// The old size of a resized sublevel, and the new size of a resized
	// sublevel
	vec2_u32 old_real_size_2d, new_real_size_2d;
	
	
	
public:		// functions
	undo_and_redo_action();
	
	// copy constructor
	undo_and_redo_action( const undo_and_redo_action& to_copy );
	
	//// move constructor
	//undo_and_redo_action( undo_and_redo_action&& to_move );
	
	~undo_and_redo_action();
	
	void debug_print();
	
};


class undo_and_redo_stack
{
//public:		// variables
protected:		// variables
	vector<undo_and_redo_action> action_vec;
	
	// A s64 can only represent so many different actions, so over time
	// perhaps this could be a source of an error :/
	s64 next_action_index;
	
public:		// functions
	undo_and_redo_stack();
	
	
	bool can_undo() const;
	bool can_redo() const;
	
	
	
	
	// get_curr_action() is used by undo-related code (including code
	// outside this class)
	undo_and_redo_action& get_curr_action();
	
	// get_curr_action() is used by redo-related code (including code
	// outside this class)
	undo_and_redo_action& get_next_action();
	
	
	// If not at the end of action_vec, this chops off everything after the
	// current position.
	void add_action( const undo_and_redo_action& the_action );
	
	
	void debug_print();
	
	//// Only one undo or redo at a time.  Returns whether the undo or redo
	//// was successful, which is probably only useful for debugging
	//// purposes.
	
	bool finalize_undo();
	bool finalize_redo();
	
	
	
protected:		// functions
	s64 get_curr_action_index() const;
	void set_curr_action_index( s64 n_curr_action_index );
	
	s64 get_next_action_index() const;
	void set_next_action_index( s64 n_next_action_index );
	
};


struct undo_and_redo_stuff
{
	undo_and_redo_stack ur_stack;
	
	undo_and_redo_action ur_action_to_push;
};



#endif		// undo_and_redo_stack_class_hpp
