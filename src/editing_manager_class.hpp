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
	
	
	unordered_map< level_editor_core_widget*, undo_and_redo_stuff >
		ur_stuff_umap;
	
	
	
	// These are used to deal with multiple mouse buttons possibly being
	// down or up during mouse events
	bool left_mouse_button_down, right_mouse_button_down;
	
	
public:		// functions
	inline editing_manager() : the_level_editor_widget(NULL),
		left_mouse_button_down(false), right_mouse_button_down(false)
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
	void finalize_movement_of_rs_contents
		( level_editor_core_widget* the_core_widget,
		rect_selection_stuff& the_rect_selection_stuff );
	
	void finalize_sprite_properties_modification
		( level_editor_core_widget* the_core_widget,
		const sprite_pw_extras& the_sprite_pw_extras,
		const sprite_ipgws& the_new_sprite_ipgws );
	
	void record_sublevel_properties_modification_ur_stuff
		( level_editor_core_widget* the_core_widget,
		const sublevel& the_sublevel_before_modification );
	
	
protected:		// functions
	undo_and_redo_stuff& get_or_create_ur_stuff
		( level_editor_core_widget* the_core_widget );
	
	
	// Mouse press handler functions.  These were once lambda functions,
	// but not any more.
	void handle_placing_le_during_mouse_press
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos,
		bool current_tabbed_widget_is_for_blocks,
		bool current_tabbed_widget_is_for_16x16_sprites,
		bool current_tabbed_widget_is_for_16x32_sprites );
	void handle_erasing_sprites_during_mouse_press
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos );
	void handle_selecting_single_sprite_during_mouse_press
		( level_editor_core_widget* the_core_widget );
	void handle_rs_during_mouse_press
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
	void handle_erasing_sprites_during_mouse_move
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos );
	void handle_selecting_single_sprite_during_mouse_move
		( level_editor_core_widget* the_core_widget );
	void handle_rs_during_mouse_move
		( level_editor_core_widget* the_core_widget,
		const vec2_s32& block_grid_coords_of_mouse_pos );
	
	
	// Mouse release handler functions.  These were once lambda functions,
	// but not any more.
	void handle_placing_le_during_mouse_release
		( level_editor_core_widget* the_core_widget );
	void handle_erasing_sprites_during_mouse_release
		( level_editor_core_widget* the_core_widget );
	void handle_selecting_single_sprite_during_mouse_release
		( level_editor_core_widget* the_core_widget );
	void handle_rs_during_mouse_release
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
	
	
	// Editing functions to activate upon mouse events
	
	inline block_type get_left_selected_block_type
		( level_editor_core_widget* the_core_widget ) const
	{
		return (block_type)the_core_widget
			->get_the_block_selector_core_widget()
			->get_left_current_level_element_index();
	}
	inline sprite_type get_left_selected_16x16_sprite_type
		( level_editor_core_widget* the_core_widget ) const
	{
		return (sprite_type)the_core_widget
			->get_the_sprite_16x16_selector_core_widget()
			->get_left_current_level_element_index();
	}
	inline sprite_type get_left_selected_16x32_sprite_type
		( level_editor_core_widget* the_core_widget ) const
	{
		return (sprite_type)the_core_widget
			->get_the_sprite_16x32_selector_core_widget()
			->get_left_current_level_element_index();
	}
	
	
	void place_single_block_and_record_ur_stuff
		( block& the_block_in_sublevel, const vec2_s32& block_grid_coord,
		const block_type& the_block_type,
		undo_and_redo_action& ur_action_to_push );
	
	// A wrapper
	inline void place_single_block_and_record_ur_stuff
		( sublevel* the_sublevel, const vec2_s32& block_grid_coord,
		const block_type& the_block_type,
		undo_and_redo_action& ur_action_to_push )
	{
		block& the_block_in_sublevel
			= the_sublevel->uncompressed_block_data_vec_2d
			.at((u32)block_grid_coord.y).at((u32)block_grid_coord.x);
		
		place_single_block_and_record_ur_stuff( the_block_in_sublevel,
			block_grid_coord, the_block_type, ur_action_to_push );
	}
	
	
	// This uses a purely integer-based block line placing algorithm.  The
	// algorithm was originally used for placing lines of pixels for a very
	// basic 3D software rasterizer I wrote for the GBA.  That code was
	// never published anywhere, since the software rasterizer kind of
	// sucked.  Perhaps I will put it up on GitHub at some point.
	void place_block_line( level_editor_core_widget* the_core_widget, 
		const sf::Vector2i& pos_0, const sf::Vector2i& pos_1, 
		block_type the_block_type, 
		undo_and_redo_action& ur_action_to_push );
	
	// A wrapper
	inline void place_block_line
		( level_editor_core_widget* the_core_widget, 
		const vec2_s32& pos_0, const vec2_s32& pos_1, 
		block_type the_block_type, 
		undo_and_redo_action& ur_action_to_push )
	{
		place_block_line( the_core_widget, 
			sf::Vector2i( pos_0.x, pos_0.y ),
			sf::Vector2i( pos_1.x, pos_1.y ), the_block_type,
			ur_action_to_push );
	}
	
	
	void place_single_16x16_sprite_and_record_ur_stuff
		( sublevel* the_sublevel, const vec2_s32& block_grid_coord, 
		const sprite_type& the_sprite_type,
		undo_and_redo_action& ur_action_to_push );
	
	void place_single_16x32_sprite_and_record_ur_stuff
		( sublevel* the_sublevel, const vec2_s32& block_grid_coord, 
		const sprite_type& the_sprite_type,
		undo_and_redo_action& ur_action_to_push );
	
	
	
	// Editing functions to activate upon key press.  These are meant to
	// actually 
	void copy_rs_contents
		( rect_selection_stuff& the_rect_selection_stuff );
	void paste_copied_rs_contents
		( level_editor_core_widget* the_core_widget,
		rect_selection_stuff& the_rect_selection_stuff );
	
	
	// Editing actions that have been made into their own functions so
	// that the_editing_manager can handle them.
	void finalize_movement_of_rs_contents_block_ur_stuff
		( level_editor_core_widget* the_core_widget, 
		rect_selection_stuff& the_rect_selection_stuff, 
		const sf::IntRect& selection_rect,
		const sf::IntRect& selection_rect_before_moving,
		bool rs_was_pasted );
	void finalize_movement_of_rs_contents_sprite_ur_stuff
		( level_editor_core_widget* the_core_widget, 
		rect_selection_stuff& the_rect_selection_stuff, 
		const sf::IntRect& selection_rect, 
		const sf::IntRect& selection_rect_before_moving,
		bool rs_was_pasted );
	
	
	// Sprite properties stuff
	
	
	
	friend class level_editor_widget;
	friend class level_editor_core_widget;
	friend class level_editor_sfml_canvas_widget;
	friend class sprite_properties_widget;
	
};



#endif		// editing_manager_class_hpp
