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


#ifndef level_editor_core_widget_class_hpp
#define level_editor_core_widget_class_hpp


#include "misc_includes.hpp"
#include "sfml_canvas_widget_classes.hpp"
//#include "sublevel_class.hpp"
#include "level_class.hpp"


class block_selector_widget;
class sprite_16x16_selector_widget;
class sprite_16x32_selector_widget;

class block_selector_core_widget;
class sprite_16x16_selector_core_widget;
class sprite_16x32_selector_core_widget;

class editing_manager;

class level_editor_core_widget : public QWidget
{
	Q_OBJECT
	
public:		// enums
	
public:		// variables
	
	QWidget* parent;
	
	string level_file_name;
	
	//sfml_canvas_widget* the_sfml_canvas_widget;
	unique_ptr<level_editor_sfml_canvas_widget> the_sfml_canvas_widget;
	
	QPoint current_position;
	QSize current_size;
	
	
	// Level element selectors
	QTabWidget* level_element_selectors_tab_widget;
	
	block_selector_widget* the_block_selector_widget;
	sprite_16x16_selector_widget* the_sprite_16x16_selector_widget;
	sprite_16x32_selector_widget* the_sprite_16x32_selector_widget;
	
	#define list_of_level_element_widget_name_prefixes(macro) \
		macro(block) macro(sprite_16x16) macro(sprite_16x32)
	
	
	//level* the_level;
	sublevel* the_sublevel;
	
	
	// This is for block line drawing and multiple level element
	// rectangular selections.
	//QPoint prev_mouse_pos;
	//vec2_u32 block_grid_coords_of_prev_mouse_pos;
	vec2_s32 block_grid_coords_of_prev_mouse_pos;
	
	
	// The mouse mode
	mouse_mode the_mouse_mode;
	
	
	
	adj_sprite_ipgws_ptr_group_for_selecting_sprite
		the_sprite_selection_ptr_group;
	
	editing_manager* the_editing_manager;
	
	
public:		// functions
	level_editor_core_widget( QWidget* s_parent, const QPoint& s_position,
		const string& s_level_file_name, sublevel* s_the_sublevel,
		editing_manager* s_the_editing_manager);
	level_editor_core_widget( QWidget* s_parent, const QPoint& s_position,
		const string& s_level_file_name, sublevel* s_the_sublevel,
		const vec2_u32& s_sublevel_real_size_2d,
		editing_manager* s_the_editing_manager);
	
	void shared_constructor_code( const QPoint& s_position, 
		 const vec2_u32& s_sublevel_real_size_2d );
	
	
	// 
	void init_tab_stuff
		( QTabWidget* n_level_element_selectors_tab_widget,
		block_selector_widget* n_the_block_selector_widget,
		sprite_16x16_selector_widget* n_the_sprite_16x16_selector_widget,
		sprite_16x32_selector_widget* n_the_sprite_16x32_selector_widget );
	
	//void reinit( QScrollArea* n_scroll_area );
	
	
	block_selector_core_widget* get_the_block_selector_core_widget();
	sprite_16x16_selector_core_widget* 
		get_the_sprite_16x16_selector_core_widget();
	sprite_16x32_selector_core_widget* 
		get_the_sprite_16x32_selector_core_widget();
	
	bool zoom_in();
	bool zoom_out();
	
	//inline sublevel& get_curr_sublevel()
	//{
	//	return the_level->get_curr_sublevel();
	//}
	
	
	
protected:		// functions
	// Utility functions
	
	inline void do_emit_right_mouse_button_pressed()
	{
		emit right_mouse_button_pressed();
	}
	
	inline void do_emit_sprite_no_longer_selected()
	{
		emit sprite_no_longer_selected();
	}
	
	inline void do_emit_sprite_was_selected()
	{
		emit sprite_was_selected();
	}
	
	
	
	
	// Events
	//void keyPressEvent( QKeyEvent* event );
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );
	
	//void focusInEvent( QFocusEvent* event );
	//void focusOutEvent( QFocusEvent* event );
	
	
signals:		// signals
	void right_mouse_button_pressed();
	
	void sprite_was_selected();
	void sprite_no_longer_selected();
	
	
public:		// friend classes
	friend class editing_manager;
	
	
};



#endif // level_editor_core_widget_class_hpp
