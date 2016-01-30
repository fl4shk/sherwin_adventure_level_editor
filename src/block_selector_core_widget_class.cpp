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


#include "block_selector_core_widget_class.hpp"

const string block_selector_core_widget::block_gfx_file_name
	= "gfx/the_block_gfx.png";


block_selector_core_widget::block_selector_core_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	left_current_block_index(0), right_current_block_index(0),
	block_palette_modified_recently(false)
{
	block_gfx_raw_texture.loadFromFile(block_gfx_file_name);
	selected_block_sprite.setTexture(block_gfx_raw_texture);
	
	
	block_palette_render_texture.create( get_num_blocks_per_column() 
		* slot_outer_width, get_num_blocks_per_row() 
		* slot_outer_height );
	block_palette_render_texture.clear(sf::Color::White);
	block_palette_texture = block_palette_render_texture.getTexture();
	block_palette_sprite.setTexture(block_palette_texture);
	
	// These are needed to make it so that this widget actually shows up.
	full_resize( QSize( get_num_blocks_per_column() * slot_outer_width,
		get_num_blocks_per_row() * slot_outer_height ) );
	set_min_max_sizes( QSize( get_num_blocks_per_column() 
		* slot_outer_width, get_num_blocks_per_row() 
		* slot_outer_height ) );
	
	
	//slot_outer_usual_image.create( slot_outer_width, slot_outer_height, 
	//	sf::Color::Black );
	//slot_outer_left_selected_image.create( slot_outer_width, 
	//	slot_outer_height, sf::Color::White );
	//slot_outer_right_selected_image.create( slot_outer_width, 
	//	slot_outer_height, sf::Color::Cyan );
	
	slot_outer_usual_image.create( slot_outer_width, slot_outer_height, 
		sf::Color::Cyan );
	slot_outer_left_selected_image.create( slot_outer_width, 
		slot_outer_height, sf::Color::Black );
	slot_outer_right_selected_image.create( slot_outer_width, 
		slot_outer_height, sf::Color::Red );
	
	//slot_inner_texture.loadFromImage(slot_inner_image);
	
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



void block_selector_core_widget::mousePressEvent( QMouseEvent* event )
{
	sf::Vector2i event_pos_in_block_selection_coords
		( event->x() / slot_outer_width, 
		event->y() / slot_outer_height );
	
	if ( event->button() == Qt::LeftButton )
	{
		u32 n_left_current_block_index 
			= block_selection_coords_to_current_block_index
			(event_pos_in_block_selection_coords);
		
		if ( n_left_current_block_index < get_num_blocks_per_palette() )
		{
			set_left_current_block_index(n_left_current_block_index);
		}
	}
	else if ( event->button() == Qt::RightButton )
	{
		u32 n_right_current_block_index 
			= block_selection_coords_to_current_block_index
			(event_pos_in_block_selection_coords);
		
		if ( n_right_current_block_index < get_num_blocks_per_palette() )
		{
			set_right_current_block_index(n_right_current_block_index);
		}
	}
	
}


void block_selector_core_widget::generate_block_palette_render_texture()
{
	//cout << "block_palette_render_texture:  " 
	//	<< block_palette_render_texture.setActive(true) << endl;
	block_palette_render_texture.clear(sf::Color::Green);
	
	u32 num_blocks_per_column = get_num_blocks_per_column(),
		num_blocks_per_row = get_num_blocks_per_row();
	
	for ( u32 j=0; j<num_blocks_per_row; ++j )
	{
		for ( u32 i=0; i<num_blocks_per_column; ++i )
		{
			//slot_inner_image.setPixel( 0, 0, palette.at
			//	( j * num_colors_per_row + i ) );
			//slot_inner_texture.loadFromImage(slot_inner_image);
			//
			////slot_inner_sprite.setPosition( i * slot_outer_width + 1,
			////	block_palette_render_texture.getSize().y 
			////	- ( ( j + 1 ) * slot_outer_height - 1 ) );
			
			selected_block_sprite.setTextureRect( sf::IntRect
				( sf::Vector2i( i * slot_inner_width, 
				j * slot_inner_height ), sf::Vector2i( slot_inner_width,
				slot_inner_height ) ) );
			
			selected_block_sprite.setPosition( i * slot_outer_width + 1,
				j * slot_outer_height + 1 );
			
			//cout << "xpos == " << i * slot_outer_width + 1 << endl;
			//cout << "ypos == " << j * slot_outer_height + 1 << endl;
			
			// Highlight the SELECTED blocks with a border, drawing the
			// LEFT mouse button selected block one on top of the right
			// mouse button selected block.
			if ( left_current_block_index == j * num_blocks_per_column + i )
			{
				slot_outer_left_selected_sprite.setPosition
					( selected_block_sprite.getPosition().x - 1,
					selected_block_sprite.getPosition().y - 1 );
				
				block_palette_render_texture.draw
					(slot_outer_left_selected_sprite);
			}
			else if ( right_current_block_index 
				== j * num_blocks_per_column + i )
			{
				slot_outer_right_selected_sprite.setPosition
					( selected_block_sprite.getPosition().x - 1,
					selected_block_sprite.getPosition().y - 1 );
				
				block_palette_render_texture.draw
					(slot_outer_right_selected_sprite);
			}
			else
			{
				slot_outer_usual_sprite.setPosition
					( selected_block_sprite.getPosition().x - 1,
					selected_block_sprite.getPosition().y - 1 );
				
				block_palette_render_texture.draw(slot_outer_usual_sprite);
			}
			
			block_palette_render_texture.draw(selected_block_sprite);
		}
	}
	
	block_palette_render_texture.display();
	block_palette_texture = block_palette_render_texture.getTexture();
}

void block_selector_core_widget::on_update()
{
	if (block_palette_modified_recently)
	{
		block_palette_modified_recently = false;
		
		//generate_palette_render_texture();
		
		generate_block_palette_render_texture();
		block_palette_texture = block_palette_render_texture.getTexture();
		
		
	}
	
	
	clear(sf::Color::Cyan);
	
	draw(block_palette_sprite);
	
	//sf::Sprite test_sprite(block_gfx_raw_texture);
	//draw(test_sprite);
}



