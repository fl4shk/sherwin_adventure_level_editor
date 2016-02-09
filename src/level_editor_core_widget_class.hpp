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
#include "sublevel_class.hpp"


class block_selector_widget;
class sprite_16x16_selector_widget;
class sprite_16x32_selector_widget;

class block_selector_core_widget;
class sprite_16x16_selector_core_widget;
class sprite_16x32_selector_core_widget;

class level_editor_core_widget : public QWidget
{
public:		// enums
	
public:		// variables
	
	QWidget* parent;
	
	string level_file_name;
	
	sfml_canvas_widget* the_sfml_canvas_widget;
	
	
	// Level element selectors
	QTabWidget* level_element_selectors_tab_widget;
	
	block_selector_widget* the_block_selector_widget;
	sprite_16x16_selector_widget* the_sprite_16x16_selector_widget;
	sprite_16x32_selector_widget* the_sprite_16x32_selector_widget;
	
	#define list_of_level_element_widget_name_prefixes(macro) \
		macro(block) macro(sprite_16x16) macro(sprite_16x32)
	
	
	
	// Temporary sublevel instance until a "level" class is created that
	// contains multiple sublevels.  Perhaps a vector<level> would be of
	// interest to have, in case tabbed level editing is desired.
	sublevel the_sublevel;
	
	
	// This is for block line drawing and multiple level element
	// rectangular selections.
	//QPoint prev_mouse_pos;
	vec2_u32 block_grid_coords_of_prev_mouse_pos;
	
	
	mouse_mode the_mouse_mode;
	
	
public:		// functions
	level_editor_core_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, const string& s_level_file_name );
	
	
	// 
	void initialize_tab_stuff
		( QTabWidget* n_level_element_selectors_tab_widget,
		block_selector_widget* n_the_block_selector_widget,
		sprite_16x16_selector_widget* n_the_sprite_16x16_selector_widget,
		sprite_16x32_selector_widget* n_the_sprite_16x32_selector_widget );
	
	
	block_selector_core_widget* get_the_block_selector_core_widget();
	sprite_16x16_selector_core_widget* 
		get_the_sprite_16x16_selector_core_widget();
	sprite_16x32_selector_core_widget* 
		get_the_sprite_16x32_selector_core_widget();
	
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
};



#endif // level_editor_core_widget_class_hpp
