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



#ifndef block_selector_core_widget_class_hpp
#define block_selector_core_widget_class_hpp

//#include "sfml_canvas_widget_classes.hpp"
#include "block_selector_widget_class.hpp"
#include "block_stuff.hpp"

class level_editor_core_widget;

class block_selector_core_widget : public sfml_canvas_widget_base
{
public:		// constants
	static constexpr u32 slot_inner_width = 16, slot_inner_height = 16, 
		slot_outer_width = slot_inner_width + 2, 
		slot_outer_height = slot_inner_height + 2;
	
	static const string block_gfx_file_name;
	
protected:		// variables
	level_editor_core_widget* the_level_editor_core_widget;
	
	sf::Texture block_gfx_raw_texture;
	sf::Sprite selected_block_sprite;
	
	
	//array< sf::Color, num_block_types > palette;
	sf::RenderTexture block_palette_render_texture;
	sf::Texture block_palette_texture;
	sf::Sprite block_palette_sprite;
	
	// These variables are used for drawing the block images into
	// block_palette_render_texture.
	sf::Image slot_outer_usual_image, slot_outer_left_selected_image,
		slot_outer_right_selected_image;
	sf::Texture slot_outer_usual_texture, slot_outer_left_selected_texture,
		slot_outer_right_selected_texture;
	sf::Sprite slot_outer_usual_sprite, slot_outer_left_selected_sprite,
		slot_outer_right_selected_sprite;

	u32 left_current_block_index, right_current_block_index;
	bool block_palette_modified_recently;
	
public:		// variables
	
	
	
public:		// functions
	block_selector_core_widget( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size );
	
	//bool grab_palette_from_canvas_widget_image();
	
	inline u32 get_left_current_block_index() const
	{
		return left_current_block_index;
	}
	inline void set_left_current_block_index
		( u32 n_left_current_block_index )
	{
		block_palette_modified_recently = true;
		u32 num_blocks_per_palette = get_num_blocks_per_palette();
		
		left_current_block_index = ( n_left_current_block_index 
			< num_blocks_per_palette ) ? n_left_current_block_index 
			: ( num_blocks_per_palette - 1 );
	}
	inline u32 get_right_current_block_index() const
	{
		return right_current_block_index;
	}
	
	inline void set_right_current_block_index
		( u32 n_right_current_block_index )
	{
		block_palette_modified_recently = true;
		u32 num_blocks_per_palette = get_num_blocks_per_palette();
		
		right_current_block_index = ( n_right_current_block_index 
			< num_blocks_per_palette ) ? n_right_current_block_index 
			: ( num_blocks_per_palette - 1 );
	}
	
	inline u32 block_selection_coords_to_current_block_index
		( const sf::Vector2i& pos_in_block_selection_coords )
	{
		//return (u32)( ( pos_in_block_selection_coords.y 
		//	* num_blocks_per_row ) + pos_in_block_selection_coords.x );
		return (u32)( ( pos_in_block_selection_coords.y
			* get_num_blocks_per_row() ) 
			+ pos_in_block_selection_coords.x );
	}
	
	inline u32 get_num_blocks_per_row() const
	{
		return block_gfx_raw_texture.getSize().x / slot_inner_width;
	}
	
	inline u32 get_num_blocks_per_column() const
	{
		return block_gfx_raw_texture.getSize().y / slot_inner_height;
	}
	
	inline u32 get_num_blocks_per_palette() const
	{
		return get_num_blocks_per_row() * get_num_blocks_per_column();
	}
	
	
	inline void set_the_level_editor_core_widget
		( level_editor_core_widget* n_the_level_editor_core_widget );
	
	
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	
	//void generate_palette_render_texture();
	
	inline void on_init()
	{
		cout << "block_selector_widget initalized!\n";
		
		generate_block_palette_render_texture();
	}
	
	void generate_block_palette_render_texture();
	
	void on_update();
	
	friend class block_selector_widget;
};

#include "level_editor_core_widget_class.hpp"

inline void block_selector_core_widget::set_the_level_editor_core_widget
	( level_editor_core_widget* n_the_level_editor_core_widget )
{
	the_level_editor_core_widget = n_the_level_editor_core_widget;
}

#endif		// block_selector_core_widget_class_hpp
