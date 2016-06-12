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

void undo_and_redo_action::debug_print()
{
	
}

undo_and_redo_stack::undo_and_redo_stack()
{
	action_vec.clear();
	curr_action_index = 0;
}


u64 undo_and_redo_stack::get_curr_action_index() const
{
	return curr_action_index;
}

undo_and_redo_action& undo_and_redo_stack::get_curr_action()
{
	return action_vec.at(get_curr_action_index());
}


void undo_and_redo_stack::add_action
	( const undo_and_redo_action& the_action )
{
	// Check whether undo has been done recently.
	if ( curr_action_index == action_vec.size() - 1 )
	{
		action_vec.push_back(the_action);
	}
	
	// If not, then replace all the stuff after
	else //if ( curr_action_index < action_vec.size() - 1 )
	{
		action_vec.resize( curr_action_index + 1 );
		action_vec.push_back(the_action);
	}
	
	++curr_action_index;
}


//bool undo_and_redo_stack::undo()
//{
//	if ( curr_action_index > 0 )
//	{
//		--curr_action_index;
//	}
//}
//
//bool undo_and_redo_stack::redo()
//{
//	if ( curr_action_index < action_vec.size() - 1 )
//	{
//		++curr_action_index;
//	}
//}


