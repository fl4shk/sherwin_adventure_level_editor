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

#ifndef editing_manager_class_hpp
#define editing_manager_class_hpp

#include "level_editor_widget_class.hpp"





// This class is supposed to help manage editing levels, including undo and
// redo stuff.  Editing actions are supposed to go through this class.
// Therefore, instances level_editor_widget and level_editor_core_widget
// will contain a pointer to an instance of this class.
class editing_manager
{
protected:		// variables
	
	// Pointers to the objects that use an instance of this.
	level_editor_widget* the_level_editor_widget;
	//vector< unique_ptr<level_editor_widget> >* the_core_widget_vec;
	
	struct undo_and_redo_stuff
	{
		undo_and_redo_stack ur_stack;
		bool ur_action_active;
		
		// I am not sure whether having only one ur_action_to_push will
		// work.  If it works, it should 
		undo_and_redo_action ur_action_to_push;
	};
	
	map< level_editor_core_widget*, undo_and_redo_stuff > ur_stuff_map;
	
	
	
public:		// functions
	inline editing_manager() : the_level_editor_widget(NULL)
	{
	}
	
	inline void set_the_level_editor_widget
		( level_editor_widget* n_the_level_editor_widget )
	{
		the_level_editor_widget = n_the_level_editor_widget;
	}
	inline level_editor_widget* get_the_level_editor_widget()
	{
		return the_level_editor_widget;
	}
	
	
	// Events from other classes
	
	// level_editor_widget
	void key_press_event( QKeyEvent* event );
	
	// level_editor_core_widget
	void mouse_press_event( level_editor_core_widget* the_core_widget, 
		QMouseEvent* event );
	void mouse_move_event( level_editor_core_widget* the_core_widget, 
		QMouseEvent* event );
	void mouse_release_event( level_editor_core_widget* the_core_widget, 
		QMouseEvent* event );
	
	
	// Editing actions that have been made into their own functions so
	// that the_editing_manager can handle them.
	
	void finalize_movement_of_rect_selection_contents
		( level_editor_core_widget* the_core_widget,
		rect_selection_stuff& the_rect_selection_stuff );
	
	
protected:		// functions
	// Mouse press handler functions.  These were once lambda functions,
	// but not any more.
	void handle_placing_le_during_mouse_press
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos,
		bool current_tabbed_widget_is_for_blocks,
		bool current_tabbed_widget_is_for_16x16_sprites,
		bool current_tabbed_widget_is_for_16x32_sprites );
	void handle_erasing_sprite_during_mouse_press
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos );
	void handle_selecting_single_sprite_during_mouse_press
		( level_editor_core_widget* the_core_widget );
	void handle_rect_selection_during_mouse_press
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos,
		bool current_tabbed_widget_is_for_blocks,
		bool current_tabbed_widget_is_for_16x16_sprites,
		bool current_tabbed_widget_is_for_16x32_sprites );
	
	// Mouse move handler functions.  These were once lambda functions, but
	// not any more.
	void handle_placing_le_during_mouse_move
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos,
		bool current_tabbed_widget_is_for_blocks );
	void handle_erasing_sprite_during_mouse_move
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos );
	void handle_selecting_single_sprite_during_mouse_move
		( level_editor_core_widget* the_core_widget );
	void handle_rect_selection_during_mouse_move
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos );
	
	
	// Mouse release handler functions.  These were once lambda functions,
	// but not any more.
	void handle_placing_le_during_mouse_release
		( level_editor_core_widget* the_core_widget );
	void handle_erasing_sprite_during_mouse_release
		( level_editor_core_widget* the_core_widget );
	void handle_selecting_single_sprite_during_mouse_release
		( level_editor_core_widget* the_core_widget );
	void handle_rect_selection_during_mouse_release
		( level_editor_core_widget* the_core_widget );
	
	
	
	inline vector< unique_ptr<level_editor_core_widget> >&
		get_the_core_widget_vec()
	{
		return the_level_editor_widget->the_core_widget_vec;
	}
	inline unique_ptr<level_editor_sfml_canvas_widget>&
		get_the_sfml_canvas_widget
		( unique_ptr<level_editor_core_widget>& the_core_widget )
	{
		return the_core_widget->the_sfml_canvas_widget;
	}
	
	
	// This is used in a few different places.
	void get_a_few_types_of_mouse_pos
		( level_editor_sfml_canvas_widget* the_sfml_canvas_widget,
		sf::Vector2i& ret_mouse_pos_in_canvas_widget_coords,
		sf::Vector2f& ret_mouse_pos_in_canvas_coords, 
		vec2_s32& ret_block_grid_coords_of_mouse_pos );
	
	
	// Editing functions to activate upon key press.  These are meant to
	// actually 
	void copy_selection_contents
		( rect_selection_stuff& the_rect_selection_stuff );
	void paste_copied_selection_contents
		( level_editor_core_widget* the_core_widget,
		rect_selection_stuff& the_rect_selection_stuff );
	
	
	
	
	
	friend class level_editor_widget;
	friend class level_editor_core_widget;
	friend class level_editor_sfml_canvas_widget;
	
};



#endif		// editing_manager_class_hpp
