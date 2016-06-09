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


#define get_le_core_widget_stuff(the_core_widget)					\
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget			\
		= the_core_widget->the_sfml_canvas_widget.get();			\
	QTabWidget* level_element_selectors_tab_widget					\
		= the_core_widget->level_element_selectors_tab_widget;		\
																	\
	block_selector_widget* the_block_selector_widget 				\
		= the_core_widget->the_block_selector_widget;				\
	sprite_16x16_selector_widget* the_sprite_16x16_selector_widget	\
		= the_core_widget->the_sprite_16x16_selector_widget;		\
	sprite_16x32_selector_widget* the_sprite_16x32_selector_widget	\
		= the_core_widget->the_sprite_16x32_selector_widget;		\
																	\
	adj_sprite_ipgws_ptr_group_for_selecting_sprite&				\
		the_sprite_selection_ptr_group 								\
		= the_core_widget->the_sprite_selection_ptr_group;			\
																	\
	mouse_mode& the_mouse_mode = the_core_widget->the_mouse_mode;	\
	vec2_s32& block_grid_coords_of_prev_mouse_pos = the_core_widget	\
		->block_grid_coords_of_prev_mouse_pos;						\
																	\
	sf::Vector2i mouse_pos_in_canvas_widget_coords 					\
		= sf::Mouse::getPosition(*the_sfml_canvas_widget);			\
																	\
	/* This converts the clicked coordinate to pixel coordinates.*/	\
	sf::Vector2f mouse_pos_in_canvas_coords							\
		( (double)mouse_pos_in_canvas_widget_coords.x 				\
		/ (double)the_sfml_canvas_widget->scale_factor,				\
		(double)mouse_pos_in_canvas_widget_coords.y					\
		/ (double)the_sfml_canvas_widget->scale_factor );			\
																	\
	u32 scale_factor = the_sfml_canvas_widget->scale_factor;		\
																	\
	vec2_s32 block_grid_coords_of_mouse_pos							\
		= { (s32)( mouse_pos_in_canvas_coords.x						\
		/ ( level_editor_sfml_canvas_widget							\
		::num_pixels_per_block_row ) ),								\
																	\
		(s32)( ( the_core_widget->the_sublevel->real_size_2d.y 		\
		- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )	\
		- mouse_pos_in_canvas_coords.y )							\
		/ level_editor_sfml_canvas_widget							\
		::num_pixels_per_block_column ) ) };
	


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
	
	
protected:		// functions
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
	
	
	// Things to 
	void copy_selection_contents
		( rect_selection_stuff& the_rect_selection_stuff );
	void paste_copied_selection_contents
		( level_editor_sfml_canvas_widget* the_sfml_canvas_widget,
		rect_selection_stuff& the_rect_selection_stuff );
	
	
	friend class level_editor_widget;
	friend class level_editor_core_widget;
	friend class level_editor_sfml_canvas_widget;
	
};



#endif		// editing_manager_class_hpp
