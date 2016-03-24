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


#ifndef mouse_mode_button_group_widget_class_hpp
#define mouse_mode_button_group_widget_class_hpp

#include "misc_includes.hpp"


// This class is intended to be used in the primary_widget class's toolbar.
class mouse_mode_button_group_widget : public QWidget
{
public:		// variables
	QWidget* parent;
	
	// A QHBoxLayout for the mouse_mode_button_group_widget itself.
	unique_ptr<QHBoxLayout> hbox_layout;
	
	
	// The QToolButton's.
	unique_ptr<QToolButton> draw_mode_tool_button,
		sprite_properties_mode_tool_button,
		rect_selection_mode_tool_button;
	
	
	// These are owned by the primary_widget.
	QAction* draw_mode_tool_button_action, 
		* sprite_properties_mode_tool_button_action,
		* rect_selection_mode_tool_button_action;
	
	
public:		// functions
	mouse_mode_button_group_widget( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size,
		QAction* s_draw_mode_tool_button_action,
		QAction* s_sprite_properties_mode_tool_button_action,
		QAction* s_rect_selection_mode_tool_button_action );
	
	
protected:		// functions
	void generate_tool_buttons();
	
};



#endif		// mouse_mode_button_group_widget_class_hpp
