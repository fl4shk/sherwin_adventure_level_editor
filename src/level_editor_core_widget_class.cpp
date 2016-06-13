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

#include "block_selector_core_widget_class.hpp"
#include "sprite_16x16_selector_core_widget_class.hpp"
#include "sprite_16x32_selector_core_widget_class.hpp"

#include "editing_manager_class.hpp"


level_editor_core_widget::level_editor_core_widget( QWidget* s_parent,
	const QPoint& s_position, const string& s_level_file_name, 
	sublevel* s_the_sublevel, editing_manager* s_the_editing_manager )
	: QWidget(s_parent), parent(s_parent),
	level_file_name(s_level_file_name), current_position(s_position), 
	the_sublevel(s_the_sublevel), the_mouse_mode(mm_place_level_elements),
	the_editing_manager(s_the_editing_manager)
{
	shared_constructor_code( s_position, the_sublevel->real_size_2d );
}


level_editor_core_widget::level_editor_core_widget( QWidget* s_parent,
	const QPoint& s_position, const string& s_level_file_name, 
	sublevel* s_the_sublevel, const vec2_u32& s_sublevel_real_size_2d,
	editing_manager* s_the_editing_manager )
	: QWidget(s_parent), parent(s_parent),
	level_file_name(s_level_file_name), current_position(s_position), 
	the_sublevel(s_the_sublevel), the_mouse_mode(mm_place_level_elements),
	the_editing_manager(s_the_editing_manager)
{
	shared_constructor_code( s_position, s_sublevel_real_size_2d );
}

void level_editor_core_widget::shared_constructor_code
	( const QPoint& s_position, const vec2_u32& s_sublevel_real_size_2d )
{
	// Set strong focus to enable keyboard events (and maybe mouse events?)
	// to be received.
	setFocusPolicy(Qt::StrongFocus);
	
	the_sublevel->real_size_2d = s_sublevel_real_size_2d;
	
	current_size = QSize( the_sublevel->real_size_2d.x
		* level_editor_sfml_canvas_widget::num_pixels_per_block_column,
		the_sublevel->real_size_2d.y
		* level_editor_sfml_canvas_widget::num_pixels_per_block_row );
	
	move(current_position);
	resize(current_size);
	
	//the_sfml_canvas_widget = new sfml_canvas_widget( this, s_position, 
	//	s_size );
	the_sfml_canvas_widget.reset(new level_editor_sfml_canvas_widget
		( this, current_position, current_size ));
	
	
	the_sfml_canvas_widget->set_the_sublevel(the_sublevel);
	
	
	//setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	//setMinimumSize( QSize( 512, 512 ) );
}


void level_editor_core_widget::init_tab_stuff
	( QTabWidget* n_level_element_selectors_tab_widget, 
	block_selector_widget* n_the_block_selector_widget,
	sprite_16x16_selector_widget* n_the_sprite_16x16_selector_widget,
	sprite_16x32_selector_widget* n_the_sprite_16x32_selector_widget )
{
	level_element_selectors_tab_widget 
		= n_level_element_selectors_tab_widget;
	
	#define X(name) \
	the_##name##_selector_widget = n_the_##name##_selector_widget; \
	/* the_##name##_selector_widget->the_core_widget \
		->set_the_level_editor_core_widget(this); */ \
	the_sfml_canvas_widget->set_the_##name##_selector_core_widget \
		( get_the_##name##_selector_core_widget() );
	
	list_of_level_element_widget_name_prefixes(X)
	#undef X
}

//void level_editor_core_widget::reinit( QScrollArea* n_scroll_area )
//{
//	current_size = QSize( the_sublevel->real_size_2d.x
//		* level_editor_sfml_canvas_widget::num_pixels_per_block_column,
//		the_sublevel->real_size_2d.y
//		* level_editor_sfml_canvas_widget::num_pixels_per_block_row );
//	
//	move(current_position);
//	resize(current_size);
//	
//	the_sfml_canvas_widget.reset(new sfml_canvas_widget( this, 
//		current_position, current_size ));
//	the_sfml_canvas_widget->set_the_sublevel(the_sublevel);
//	
//	#define X(name) \
//	the_sfml_canvas_widget->set_the_##name##_selector_core_widget \
//		(get_the_##name##_selector_core_widget());
//	
//	list_of_level_element_widget_name_prefixes(X)
//	#undef X
//	
//	the_sfml_canvas_widget->set_scroll_area(n_scroll_area);
//	
//}


block_selector_core_widget* 
	level_editor_core_widget::get_the_block_selector_core_widget()
{
	return the_block_selector_widget->the_core_widget;
}
sprite_16x16_selector_core_widget* 
	level_editor_core_widget::get_the_sprite_16x16_selector_core_widget()
{
	return the_sprite_16x16_selector_widget->the_core_widget;
}
sprite_16x32_selector_core_widget* 
	level_editor_core_widget::get_the_sprite_16x32_selector_core_widget()
{
	return the_sprite_16x32_selector_widget->the_core_widget;
}


bool level_editor_core_widget::zoom_in()
{
	//if ( the_sfml_canvas_widget->scale_factor == 16 )
	//{
	//	return false;
	//}
	//if ( the_sfml_canvas_widget->scale_factor == 4 )
	//{
	//	return false;
	//}
	if ( the_sfml_canvas_widget->scale_factor == 2 )
	{
		return false;
	}
	
	the_sfml_canvas_widget->scale_factor <<= 1;
	the_sfml_canvas_widget->zoomed_in_recently = true;
	
	return true;
}


bool level_editor_core_widget::zoom_out()
{
	if ( the_sfml_canvas_widget->scale_factor == 1 )
	{
		return false;
	}
	
	the_sfml_canvas_widget->scale_factor >>= 1;
	the_sfml_canvas_widget->zoomed_out_recently = true;
	
	return true;
}




// Events

void level_editor_core_widget::mousePressEvent( QMouseEvent* event )
{
	the_editing_manager->mouse_press_event( this, event );
}

void level_editor_core_widget::mouseMoveEvent( QMouseEvent* event )
{
	the_editing_manager->mouse_move_event( this, event );
}

void level_editor_core_widget::mouseReleaseEvent( QMouseEvent* event )
{
	the_editing_manager->mouse_release_event( this, event );
}



