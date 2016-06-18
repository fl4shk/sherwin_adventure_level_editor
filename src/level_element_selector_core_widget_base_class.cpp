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

#include "level_element_selector_core_widget_base_class.hpp"


// block_selector_core_widget's use this 
const string level_element_selector_core_widget_base
	::level_element_gfx_file_name = "gfx/the_block_gfx.png";


level_element_selector_core_widget_base
	::level_element_selector_core_widget_base
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	left_current_level_element_index(0), 
	right_current_level_element_index(0),
	level_element_palette_modified_recently(false)
{
	shared_constructor_code( get_level_element_gfx_file_name(), 
		get_slot_inner_width(), get_slot_inner_height(), 
		get_slot_outer_width(), get_slot_outer_height() );
}

level_element_selector_core_widget_base
	::level_element_selector_core_widget_base
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size,
	const string& other_level_element_gfx_file_name, 
	u32 other_slot_inner_width, u32 other_slot_inner_height,
	u32 other_slot_outer_width, u32 other_slot_outer_height ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	left_current_level_element_index(0), 
	right_current_level_element_index(0),
	level_element_palette_modified_recently(false)
{
	shared_constructor_code( other_level_element_gfx_file_name,
		other_slot_inner_width, other_slot_inner_height,
		other_slot_outer_width, other_slot_outer_height );
}

void level_element_selector_core_widget_base::shared_constructor_code
	( const string& other_level_element_gfx_file_name,
	u32 other_slot_inner_width, u32 other_slot_inner_height,
	u32 other_slot_outer_width, u32 other_slot_outer_height )
{
	
	level_element_gfx_raw_image.loadFromFile
		(other_level_element_gfx_file_name);
	level_element_gfx_raw_image.createMaskFromColor(sf::Color::Cyan);
	level_element_gfx_raw_texture.loadFromImage
		(level_element_gfx_raw_image);
	selected_level_element_sprite.setTexture
		(level_element_gfx_raw_texture);
	
	u32 other_num_level_elements_per_column 
		= get_num_level_elements_per_column(other_slot_inner_height);
	u32 other_num_level_elements_per_row 
		= get_num_level_elements_per_row(other_slot_inner_width);
	
	//level_element_palette_render_texture.create
	//	( other_num_level_elements_per_row * other_slot_outer_width, 
	//	other_num_level_elements_per_column * other_slot_outer_height );
	level_element_palette_render_texture.create
		( other_num_level_elements_per_row * other_slot_outer_width + 1, 
		other_num_level_elements_per_column * other_slot_outer_height 
		+ 1 );
	
	
	level_element_palette_render_texture.clear(sf::Color::White);
	level_element_palette_texture 
		= level_element_palette_render_texture.getTexture();
	level_element_palette_sprite.setTexture(level_element_palette_texture);
	
	
	// These are needed to make it so that this widget actually shows up.
	full_resize( QSize
		( level_element_palette_render_texture.getSize().x,
		level_element_palette_render_texture.getSize().y ) );
	set_min_max_sizes( QSize
		( level_element_palette_render_texture.getSize().x, 
		level_element_palette_render_texture.getSize().y ) );
	
	
	//slot_outer_usual_image.create( slot_outer_width, slot_outer_height, 
	//	sf::Color::Black );
	//slot_outer_left_selected_image.create( slot_outer_width, 
	//	slot_outer_height, sf::Color::White );
	//slot_outer_right_selected_image.create( slot_outer_width, 
	//	slot_outer_height, sf::Color::Cyan );
	
	slot_outer_usual_image.create( other_slot_outer_width, 
		other_slot_outer_height, sf::Color::Cyan );
	//slot_outer_left_selected_image.create( other_slot_outer_width, 
	//	other_slot_outer_height, sf::Color::Black );
	//slot_outer_right_selected_image.create( other_slot_outer_width, 
	//	other_slot_outer_height, sf::Color::Red );
	
	slot_outer_left_selected_image.create( other_slot_outer_width,
		other_slot_outer_height, sf::Color::Cyan );
	slot_outer_right_selected_image.create( other_slot_outer_width,
		other_slot_outer_height, sf::Color::Cyan );
	
	// Vertical lines
	for ( u32 j=0; j<other_slot_outer_height; ++j )
	{
		slot_outer_left_selected_image.setPixel( 0, j,
			sf::Color::Black );
		slot_outer_left_selected_image.setPixel( other_slot_outer_width 
			- 1, j, sf::Color::Black );
		
		slot_outer_right_selected_image.setPixel( 0, j,
			sf::Color::Red );
		slot_outer_right_selected_image.setPixel( other_slot_outer_width 
			- 1, j, sf::Color::Red );
	}
	
	// Horizontal lines
	for ( u32 i=0; i<other_slot_outer_width; ++i )
	{
		slot_outer_left_selected_image.setPixel( i, 0, sf::Color::Black );
		slot_outer_left_selected_image.setPixel( i,
			other_slot_outer_height - 1, sf::Color::Black );
		
		slot_outer_right_selected_image.setPixel( i, 0, sf::Color::Red );
		slot_outer_right_selected_image.setPixel( i,
			other_slot_outer_height - 1, sf::Color::Red );
	}
	
	
	
	slot_outer_usual_texture.loadFromImage(slot_outer_usual_image);
	slot_outer_left_selected_texture.loadFromImage
		(slot_outer_left_selected_image);
	slot_outer_right_selected_texture.loadFromImage
		(slot_outer_right_selected_image);
	
	
	slot_outer_usual_sprite.setTexture(slot_outer_usual_texture);
	slot_outer_left_selected_sprite.setTexture
		(slot_outer_left_selected_texture);
	slot_outer_right_selected_sprite.setTexture
		(slot_outer_right_selected_texture);
	
}

bool level_element_selector_core_widget_base::level_element_index_is_valid
	( u32 level_element_index ) const
{
	return ( level_element_index 
		< get_num_level_elements_per_palette() );
}

void level_element_selector_core_widget_base::mousePressEvent
	( QMouseEvent* event )
{
	sf::Vector2i event_pos_in_level_element_selection_coords
		( event->x() / get_slot_outer_width(), 
		event->y() / get_slot_outer_height() );
	
	//cout << event_pos_in_level_element_selection_coords.x
	//	<< ", " << event_pos_in_level_element_selection_coords.y << "\n";
	
	if ( event->button() == Qt::LeftButton )
	{
		u32 n_left_current_level_element_index 
			= level_element_selection_coords_to_current_level_element_index
			(event_pos_in_level_element_selection_coords);
		
		//cout << n_left_current_level_element_index << "\n\n";
		
		if ( level_element_index_is_valid
			(n_left_current_level_element_index) )
		{
			set_left_current_level_element_index
				(n_left_current_level_element_index);
		}
	}
	else if ( event->button() == Qt::RightButton )
	{
		u32 n_right_current_level_element_index 
			= level_element_selection_coords_to_current_level_element_index
			(event_pos_in_level_element_selection_coords);
		
		//cout << n_right_current_level_element_index << "\n\n";
		
		if ( level_element_index_is_valid
			(n_right_current_level_element_index) )
		{
			set_right_current_level_element_index
				(n_right_current_level_element_index);
		}
	}
	
}


void level_element_selector_core_widget_base
	::generate_level_element_palette_render_texture()
{
	//cout << "level_element_palette_render_texture:  " 
	//	<< level_element_palette_render_texture.setActive(true) << endl;
	//level_element_palette_render_texture.clear(sf::Color::Green);
	level_element_palette_render_texture.clear(sf::Color::Cyan);
	
	u32 num_level_elements_per_column 
		= get_num_level_elements_per_column(),
		num_level_elements_per_row = get_num_level_elements_per_row();
	
	for ( u32 j=0; j<num_level_elements_per_column; ++j )
	{
		for ( u32 i=0; i<num_level_elements_per_row; ++i )
		{
			//slot_inner_image.setPixel( 0, 0, palette.at
			//	( j * num_colors_per_row + i ) );
			//slot_inner_texture.loadFromImage(slot_inner_image);
			//
			////slot_inner_sprite.setPosition( i * get_slot_outer_width() + 1,
			////	level_element_palette_render_texture.getSize().y 
			////	- ( ( j + 1 ) * get_slot_outer_height() - 1 ) );
			
			selected_level_element_sprite.setTextureRect( sf::IntRect
				( sf::Vector2i( i * get_slot_inner_width(), 
				j * get_slot_inner_height() ), 
				sf::Vector2i( get_slot_inner_width(),
				get_slot_inner_height() ) ) );
			
			selected_level_element_sprite.setPosition
				( i * get_slot_outer_width() + 1,
				j * get_slot_outer_height() + 1 );
			
			//cout << "xpos == " << i * get_slot_outer_width() + 1 << endl;
			//cout << "ypos == " << j * get_slot_outer_height() + 1 << endl;
			
			// Highlight the SELECTED level_elements with a border, drawing
			// the LEFT mouse button selected level_element one on top of
			// the right mouse button selected level_element.
			if ( left_current_level_element_index 
				== j * num_level_elements_per_row + i )
			{
				slot_outer_left_selected_sprite.setPosition
					( selected_level_element_sprite.getPosition().x - 1,
					selected_level_element_sprite.getPosition().y - 1 );
				
				level_element_palette_render_texture.draw
					(slot_outer_left_selected_sprite);
			}
			else if ( right_current_level_element_index 
				== j * num_level_elements_per_row + i )
			{
				slot_outer_right_selected_sprite.setPosition
					( selected_level_element_sprite.getPosition().x - 1,
					selected_level_element_sprite.getPosition().y - 1 );
				
				level_element_palette_render_texture.draw
					(slot_outer_right_selected_sprite);
			}
			else
			{
				slot_outer_usual_sprite.setPosition
					( selected_level_element_sprite.getPosition().x - 1,
					selected_level_element_sprite.getPosition().y - 1 );
				
				level_element_palette_render_texture.draw
					(slot_outer_usual_sprite);
			}
			
			level_element_palette_render_texture.draw
				(selected_level_element_sprite);
		}
	}
	
	level_element_palette_render_texture.display();
	level_element_palette_texture 
		= level_element_palette_render_texture.getTexture();
}

void level_element_selector_core_widget_base::on_update()
{
	if (level_element_palette_modified_recently)
	{
		level_element_palette_modified_recently = false;
		
		//generate_palette_render_texture();
		
		generate_level_element_palette_render_texture();
		level_element_palette_texture 
			= level_element_palette_render_texture.getTexture();
		
	}
	
	clear(sf::Color::Cyan);
	
	draw(level_element_palette_sprite);
	
}




