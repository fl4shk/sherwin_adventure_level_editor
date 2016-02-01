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

class block_selector_core_widget;
class sprite_16x16_selector_core_widget;
class sprite_16x32_selector_core_widget;

class level_editor_core_widget : public QWidget
{
public:		// variables
	
	QWidget* parent;
	
	string level_file_name;
	
	sfml_canvas_widget* the_sfml_canvas_widget;
	
	//
	block_selector_core_widget* the_block_selector_core_widget;
	
	sprite_16x16_selector_core_widget* 
		the_sprite_16x16_selector_core_widget;
	
	sprite_16x32_selector_core_widget* 
		the_sprite_16x32_selector_core_widget;
	
public:		// functions
	level_editor_core_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, const string& s_level_file_name );
	
	inline void set_the_block_selector_core_widget
		( block_selector_core_widget* n_the_block_selector_core_widget );
	inline void set_the_sprite_16x16_selector_core_widget
		( sprite_16x16_selector_core_widget* 
		n_the_sprite_16x16_selector_core_widget );
	inline void set_the_sprite_16x32_selector_core_widget
		( sprite_16x32_selector_core_widget* 
		n_the_sprite_16x32_selector_core_widget );
	
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	
};

#include "block_selector_core_widget_class.hpp"

inline void level_editor_core_widget::set_the_block_selector_core_widget
	( block_selector_core_widget* n_the_block_selector_core_widget )
{
	the_block_selector_core_widget = n_the_block_selector_core_widget;
}

inline void level_editor_core_widget
	::set_the_sprite_16x16_selector_core_widget
	( sprite_16x16_selector_core_widget* 
	n_the_sprite_16x16_selector_core_widget )
{
	the_sprite_16x16_selector_core_widget 
		= n_the_sprite_16x16_selector_core_widget;
}

inline void level_editor_core_widget
	::set_the_sprite_16x32_selector_core_widget
	( sprite_16x32_selector_core_widget* 
	n_the_sprite_16x32_selector_core_widget )
{
	the_sprite_16x32_selector_core_widget 
		= n_the_sprite_16x32_selector_core_widget;
}


#endif // level_editor_core_widget_class_hpp
