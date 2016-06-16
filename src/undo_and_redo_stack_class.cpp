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


#include "undo_and_redo_stack_class.hpp"

undo_and_redo_action::undo_and_redo_action
	( const undo_and_redo_action& to_copy )
	: the_action_type(to_copy.the_action_type),
	test_bool(to_copy.test_bool),
	replaced_block_umap(to_copy.replaced_block_umap),
	new_block_umap(to_copy.new_block_umap),
	old_sprite_ipgws(to_copy.old_sprite_ipgws),
	new_sprite_ipgws(to_copy.new_sprite_ipgws),
	selection_rect_before_moving(to_copy.selection_rect_before_moving),
	replaced_sprite_ipgws_uset_2d(to_copy.replaced_sprite_ipgws_uset_2d),
	new_sprite_ipgws_uset_2d(to_copy.new_sprite_ipgws_uset_2d)
{
	//cout << "in undo_and_redo_action( const undo_and_redo_action& "
	//	<< "to_copy )\n";
}

//undo_and_redo_action::undo_and_redo_action
//	( undo_and_redo_action&& to_move )
//	: prev_block_map
//{
//	
//}

undo_and_redo_action::~undo_and_redo_action()
{
	//cout << "in ~undo_and_redo_action()\n";
}

void undo_and_redo_action::debug_print()
{
	
}

undo_and_redo_stack::undo_and_redo_stack()
{
	action_vec.clear();
	next_action_index = 0;
}


u64 undo_and_redo_stack::get_next_action_index() const
{
	return next_action_index;
}

undo_and_redo_action& undo_and_redo_stack::get_curr_action()
{
	return action_vec.at(get_next_action_index());
}


void undo_and_redo_stack::add_action
	( const undo_and_redo_action& the_action )
{
	//cout << "add_action()\n";
	//cout << "next_action_index:  " << next_action_index << endl;
	//cout << "action_vec.size():  " << action_vec.size() << endl;
	
	// Check whether undo has been done recently.
	if ( next_action_index == action_vec.size() )
	{
		//cout << "if\n";
		action_vec.push_back(the_action);
	}
	
	// If not, then replace all the stuff after
	else //if ( next_action_index < action_vec.size() - 1 )
	{
		//cout << "else\n";
		action_vec.resize( next_action_index + 1 );
		action_vec.push_back(the_action);
	}
	
	++next_action_index;
}


//bool undo_and_redo_stack::undo()
//{
//	if ( next_action_index > 0 )
//	{
//		--next_action_index;
//	}
//}
//
//bool undo_and_redo_stack::redo()
//{
//	if ( next_action_index < action_vec.size() - 1 )
//	{
//		++next_action_index;
//	}
//}


