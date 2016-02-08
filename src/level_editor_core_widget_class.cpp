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
	
	the_sublevel.real_size_2d = vec2_u32( s_size.width() 
		/ sfml_canvas_widget::num_pixels_per_block_row, s_size.height() 
		/ sfml_canvas_widget::num_pixels_per_block_column );
	
	the_sfml_canvas_widget->set_the_sublevel(&the_sublevel);
}


void level_editor_core_widget::initialize_tab_stuff
	( QTabWidget* n_level_element_selectors_tab_widget, 
	block_selector_widget* n_the_block_selector_widget,
	sprite_16x16_selector_widget* n_the_sprite_16x16_selector_widget,
	sprite_16x32_selector_widget* n_the_sprite_16x32_selector_widget )
{
	level_element_selectors_tab_widget 
		= n_level_element_selectors_tab_widget;
	
	#define X(name) \
	the_##name##_selector_widget = n_the_##name##_selector_widget; \
	the_##name##_selector_widget->the_core_widget \
		->set_the_level_editor_core_widget(this); \
	the_sfml_canvas_widget->set_the_##name##_selector_core_widget \
		( get_the_##name##_selector_core_widget() );
	
	list_of_level_element_widget_name_prefixes(X)
	#undef X
}

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

void level_editor_core_widget::mousePressEvent( QMouseEvent* event )
{
	//cout << "level_editor_core_widget's mouse position:  "
	//	<< event->x() << ", " << event->y() << endl;
	
	cout << event->x() << ", " << event->y() << endl;
	
	//// This converts the clicked coordinate to pixel coordinates.
	//sf::Vector2f event_pos_in_canvas_coords 
	//	= the_sfml_canvas_widget->mapPixelToCoords
	//	( sf::Vector2i( event->x(), event->y() ), 
	//	the_sfml_canvas_widget->get_apparent_view() );
	
	//sf::Vector2i event_pos_in_canvas_pixel_coords
	//	= sf::Vector2i( (int)event_pos_in_canvas_coords.x,
	//	(int)event_pos_in_canvas_coords.y );
	
	
	sf::Vector2i event_pos_in_canvas_widget_coords 
		= sf::Mouse::getPosition(*the_sfml_canvas_widget);
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f event_pos_in_canvas_coords
		( (double)event_pos_in_canvas_widget_coords.x 
		/ (double)the_sfml_canvas_widget->scale_factor,
		(double)event_pos_in_canvas_widget_coords.y
		/ (double)the_sfml_canvas_widget->scale_factor );
	
	cout << event_pos_in_canvas_coords.x << ", "
		<< event_pos_in_canvas_coords.y << endl;
	
	
	prev_mouse_pos = event->pos();
	
	// Check whether the mouse was clicked somewhere inside the image.
	//if ( !the_sfml_canvas_widget->point_is_in_render_texture
	//	(event_pos_in_canvas_pixel_coords) )
	//if ( !the_sfml_canvas_widget->point_is_in_render_texture(mouse_pos) )
	//{
	//	cout << "out of bounds\n";
	//	return;
	//}
	
	sf::FloatRect visible_rect = the_sfml_canvas_widget
		->get_visible_rect();
	
	//cout << visible_rect.left << ", " << visible_rect.top << ", "
	//	<< visible_rect.width << ", " << visible_rect.height << endl;
	
	if ( !the_sfml_canvas_widget->point_is_in_visible_rect
		( sf::Vector2i( event->x(), event->y() ) ) )
	{
		cout << "out of bounds\n";
		return;
	}
	
	the_sfml_canvas_widget->modified_recently = true;
	
	if ( event->button() == Qt::LeftButton )
	{
		u32 scale_factor = the_sfml_canvas_widget->scale_factor;
		
		vec2_u32 block_grid_coords_of_event_pos
			= { (u32)( event_pos_in_canvas_coords.x
		/ ( sfml_canvas_widget::num_pixels_per_block_row ) ),
		
		(u32)( ( the_sublevel.real_size_2d.y 
		- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
		- event_pos_in_canvas_coords.y )
		/ sfml_canvas_widget::num_pixels_per_block_column ) ) };
		
		cout << block_grid_coords_of_event_pos.x << ", "
			<< block_grid_coords_of_event_pos.y << endl;
		
		
		block& the_block_at_event_pos 
			= the_sublevel.uncompressed_block_data_vec_2d
			[block_grid_coords_of_event_pos.y]
			[block_grid_coords_of_event_pos.x];
		
		
		sprite_init_param_group_with_size& the_sprite_ipgws_at_event_pos
			= the_sublevel.sprite_ipgws_vec_2d
			[block_grid_coords_of_event_pos.y]
			[block_grid_coords_of_event_pos.x];
		
		
		bool current_tabbed_widget_is_for_blocks 
			= ( level_element_selectors_tab_widget->currentWidget()
			== the_block_selector_widget );
		bool current_tabbed_widget_is_for_16x16_sprites
			= ( level_element_selectors_tab_widget->currentWidget()
			== the_sprite_16x16_selector_widget );
		bool current_tabbed_widget_is_for_16x32_sprites
			= ( level_element_selectors_tab_widget->currentWidget()
			== the_sprite_16x32_selector_widget );
		
		if (current_tabbed_widget_is_for_blocks)
		{
			//cout << "the_block_selector_widget_is_enabled!\n";
			
			the_block_at_event_pos.type 
				= get_the_block_selector_core_widget()
				->get_left_current_level_element_index();
			
			//cout << block_stuff::get_bt_name
			//	((block_type)the_block_at_event_pos.type) << endl;
		}
		
		else if (current_tabbed_widget_is_for_16x16_sprites)
		{
			//cout << "the_sprite_16x16_selector_widget_is_enabled!\n";
			
			the_sprite_ipgws_at_event_pos.type 
				= (sprite_type)(get_the_sprite_16x16_selector_core_widget()
				->get_left_current_level_element_index());
			
			the_sprite_ipgws_at_event_pos.initial_block_grid_x_coord
				= block_grid_coords_of_event_pos.x;
			the_sprite_ipgws_at_event_pos.initial_block_grid_y_coord
				= block_grid_coords_of_event_pos.y;
			
			the_sprite_ipgws_at_event_pos.size_2d = vec2_u32( 16, 16 );
			
			
		}
		
		
		cout << endl;
		
	}
	else if ( event->button() == Qt::RightButton )
	{
		//canvas_image->setPixel( (u32)event_pos_in_image_pixel_coords.x,
		//	(u32)event_pos_in_image_pixel_coords.y, 
		//	the_block_selector_core_widget->palette.at
		//	(the_block_selector_core_widget
		//	->right_current_color_index) );
		
	}
	
}


