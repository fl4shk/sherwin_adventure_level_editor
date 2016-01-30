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

block_selector_core_widget::block_selector_core_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	left_current_block_index(0), right_current_block_index(0),
	block_palette_modified_recently(false)
{
	test_texture.loadFromFile("icons/laugh_icon_32x32.png");
	
	// These are needed to make it so that this widget actually shows up.
	full_resize( QSize( test_texture.getSize().x, 
		test_texture.getSize().y ) );
	set_min_max_sizes( QSize( test_texture.getSize().x,
		test_texture.getSize().y ) );
	
}



void block_selector_core_widget::mousePressEvent( QMouseEvent* event )
{
	
}


void block_selector_core_widget::on_update()
{
	if (block_palette_modified_recently)
	{
		block_palette_modified_recently = false;
		
		//generate_palette_render_texture();
		//block_palette_texture = block_palette_render_texture.getTexture();
		
		
	}
	
	
	//clear(sf::Color::White);
	clear(sf::Color::Green);
	//draw(block_palette_sprite);
	
	sf::Sprite test_sprite(test_texture);
	draw(test_sprite);
}



