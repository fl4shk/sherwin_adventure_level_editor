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


#include "level_editor_core_widget_class.hpp"

#include "block_selector_widget_class.hpp"
#include "sprite_16x16_selector_widget_class.hpp"
#include "sprite_16x32_selector_widget_class.hpp"

level_editor_core_widget::level_editor_core_widget( QWidget* s_parent,
	const QPoint& s_position, const QSize& s_size, 
	const string& s_level_file_name )
	: QWidget(s_parent), parent(s_parent),
	level_file_name(s_level_file_name)
{
	// Set strong focus to enable keyboard events (and maybe mouse events?)
	// to be received.
	setFocusPolicy(Qt::StrongFocus);
	
	move(s_position);
	resize(s_size);
	
	the_sfml_canvas_widget = new sfml_canvas_widget( this, s_position, 
		s_size );
}


void level_editor_core_widget::initialize_tab_stuff
	( QTabWidget* n_tab_widget, 
	block_selector_widget* n_the_block_selector_widget,
	sprite_16x16_selector_widget* n_the_sprite_16x16_selector_widget,
	sprite_16x32_selector_widget* n_the_sprite_16x32_selector_widget )
{
	tab_widget = n_tab_widget;
	
	#define X(name) \
	the_##name##_selector_widget = n_the_##name##_selector_widget; \
	the_##name##_selector_widget->the_core_widget \
		->set_the_level_editor_core_widget(this);
	
	list_of_widget_name_prefixes_in_tab_widget(X)
	#undef X
}


void level_editor_core_widget::mousePressEvent( QMouseEvent* event )
{
	cout << "level_editor_core_widget's mouse position:  "
		<< event->x() << ", " << event->y() << endl;
	
	
}


