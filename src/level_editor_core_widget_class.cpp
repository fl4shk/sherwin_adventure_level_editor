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
		->set_the_level_editor_core_widget(this);
	
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
	cout << "level_editor_core_widget's mouse position:  "
		<< event->x() << ", " << event->y() << endl;
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f event_pos_in_canvas_coords 
		= the_sfml_canvas_widget->mapPixelToCoords
		( sf::Vector2i( event->x(), event->y() ), 
		the_sfml_canvas_widget->get_apparent_view() );
	
	sf::Vector2i event_pos_in_canvas_pixel_coords
		= sf::Vector2i( (int)event_pos_in_canvas_coords.x,
		(int)event_pos_in_canvas_coords.y );
	
	cout << "mouse position in canvas coordinates:  "
		<< event_pos_in_canvas_pixel_coords.x << ", "
		<< event_pos_in_canvas_pixel_coords.y << endl;
	
	prev_mouse_pos = event->pos();
	
	// Check whether the mouse was clicked somewhere inside the image.
	if ( !the_sfml_canvas_widget->point_is_in_render_texture
		(event_pos_in_canvas_pixel_coords) )
	{
		return;
	}
	
	the_sfml_canvas_widget->modified_recently = true;
	
	if ( event->button() == Qt::LeftButton )
	{
		if ( level_element_selectors_tab_widget->currentWidget() 
			== the_block_selector_widget )
		{
			//cout << "the_block_selector_widget is enabled!\n";
			
			sf::Sprite test_sprite;
			
			test_sprite.setTexture( get_the_block_selector_core_widget()
				->get_level_element_gfx_raw_texture() );
			
			test_sprite.setTextureRect
				( get_the_block_selector_core_widget()
				->get_left_current_texture_rect() );
			
			sf::IntRect test_sprite_texture_rect 
				= test_sprite.getTextureRect();
			cout << test_sprite_texture_rect.left << ", "
				<< test_sprite_texture_rect.top << ", "
				<< test_sprite_texture_rect.width << ", "
				<< test_sprite_texture_rect.height << endl;
			
			test_sprite.setPosition
				( (u32)( event_pos_in_canvas_coords.x / 16 ) * 16,
				(u32)( ( ( ( the_sfml_canvas_widget->getSize().y 
				/ the_sfml_canvas_widget->scale_factor )
				-  event_pos_in_canvas_coords.y ) / 16 ) + 1 ) * 16 );
			
			test_sprite.setScale( 1.0f, -1.0f );
			
			the_sfml_canvas_widget->canvas_render_texture.draw
				(test_sprite);
		}
		else if ( level_element_selectors_tab_widget->currentWidget() 
			== the_sprite_16x16_selector_widget )
		{
			//cout << "the_sprite_16x16_selector_widget is enabled!\n";
			
			sf::Sprite test_sprite;
			
			test_sprite.setTexture
				( get_the_sprite_16x16_selector_core_widget()
				->get_level_element_gfx_raw_texture() );
			
			test_sprite.setTextureRect
				( get_the_sprite_16x16_selector_core_widget()
				->get_left_current_texture_rect() );
			
			sf::IntRect test_sprite_texture_rect 
				= test_sprite.getTextureRect();
			cout << test_sprite_texture_rect.left << ", "
				<< test_sprite_texture_rect.top << ", "
				<< test_sprite_texture_rect.width << ", "
				<< test_sprite_texture_rect.height << endl;
			
			test_sprite.setPosition
				( (u32)( event_pos_in_canvas_coords.x / 16 ) * 16,
				(u32)( ( ( ( the_sfml_canvas_widget->getSize().y 
				/ the_sfml_canvas_widget->scale_factor )
				-  event_pos_in_canvas_coords.y ) / 16 ) + 1 ) * 16 );
			
			test_sprite.setScale( 1.0f, -1.0f );
			
			the_sfml_canvas_widget->canvas_render_texture.draw
				(test_sprite);
		}
		else if ( level_element_selectors_tab_widget->currentWidget() 
			== the_sprite_16x32_selector_widget )
		{
			//cout << "the_sprite_16x32_selector_widget is enabled!\n";
			
			sf::Sprite test_sprite;
			
			test_sprite.setTexture
				( get_the_sprite_16x32_selector_core_widget()
				->get_level_element_gfx_raw_texture() );
			
			test_sprite.setTextureRect
				( get_the_sprite_16x32_selector_core_widget()
				->get_left_current_texture_rect() );
			
			sf::IntRect test_sprite_texture_rect 
				= test_sprite.getTextureRect();
			cout << test_sprite_texture_rect.left << ", "
				<< test_sprite_texture_rect.top << ", "
				<< test_sprite_texture_rect.width << ", "
				<< test_sprite_texture_rect.height << endl;
			
			test_sprite.setPosition
				( (u32)( event_pos_in_canvas_coords.x / 16 ) * 16,
				(u32)( ( ( ( the_sfml_canvas_widget->getSize().y 
				/ the_sfml_canvas_widget->scale_factor )
				-  event_pos_in_canvas_coords.y ) / 16 ) + 1 ) * 16 );
			
			test_sprite.setScale( 1.0f, -1.0f );
			
			the_sfml_canvas_widget->canvas_render_texture.draw
				(test_sprite);
		}
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


