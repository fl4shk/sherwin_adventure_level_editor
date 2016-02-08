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


#ifndef level_element_selector_core_widget_base_class_hpp
#define level_element_selector_core_widget_base_class_hpp

#include "sfml_canvas_widget_classes.hpp"
//#include "block_selector_widget_class.hpp"

class level_editor_core_widget;

class level_element_selector_core_widget_base 
	: public sfml_canvas_widget_base
{
protected:		// constants
	static constexpr u32 slot_inner_width = 16, slot_inner_height = 16, 
		slot_outer_width = slot_inner_width + 2, 
		slot_outer_height = slot_inner_height + 2;
	
	static const string level_element_gfx_file_name;
	
protected:		// variables
	level_editor_core_widget* the_level_editor_core_widget;
	
	sf::Image level_element_gfx_raw_image;
	sf::Texture level_element_gfx_raw_texture;
	sf::Sprite selected_level_element_sprite;
	
	
	sf::RenderTexture level_element_palette_render_texture;
	sf::Texture level_element_palette_texture;
	sf::Sprite level_element_palette_sprite;
	
	// These variables are used for drawing the level_element images into
	// level_element_palette_render_texture.
	sf::Image slot_outer_usual_image, slot_outer_left_selected_image,
		slot_outer_right_selected_image;
	sf::Texture slot_outer_usual_texture, slot_outer_left_selected_texture,
		slot_outer_right_selected_texture;
	sf::Sprite slot_outer_usual_sprite, slot_outer_left_selected_sprite,
		slot_outer_right_selected_sprite;
	
	u32 left_current_level_element_index, right_current_level_element_index;
	bool level_element_palette_modified_recently;
	
public:		// variables
	
	
	
public:		// functions
	level_element_selector_core_widget_base( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size );
	
	// Some derived classes use this constructor
	level_element_selector_core_widget_base( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size, 
		const string& other_level_element_gfx_file_name,
		u32 other_slot_inner_width, u32 other_slot_inner_height,
		u32 other_slot_outer_width, u32 other_slot_outer_height );
	
	void shared_constructor_code
		( const string& other_level_element_gfx_file_name,
		u32 other_slot_inner_width, u32 other_slot_inner_height,
		u32 other_slot_outer_width, u32 other_slot_outer_height );
	
	
	//bool grab_palette_from_canvas_widget_image();
	
	virtual inline const string& get_level_element_gfx_file_name() const
	{
		return level_element_gfx_file_name;
	}
	
	inline sf::Texture& get_level_element_gfx_raw_texture()
	{
		return level_element_gfx_raw_texture;
	}
	inline sf::RenderTexture& get_level_element_palette_render_texture()
	{
		return level_element_palette_render_texture;
	}
	
	inline u32 get_left_current_level_element_index() const
	{
		return left_current_level_element_index;
	}
	inline void set_left_current_level_element_index
		( u32 n_left_current_level_element_index )
	{
		level_element_palette_modified_recently = true;
		u32 num_level_elements_per_palette 
			= get_num_level_elements_per_palette();
		
		left_current_level_element_index 
			= ( n_left_current_level_element_index 
			< num_level_elements_per_palette ) 
			? n_left_current_level_element_index 
			: ( num_level_elements_per_palette - 1 );
	}
	inline u32 get_right_current_level_element_index() const
	{
		return right_current_level_element_index;
	}
	
	inline u32 get_left_current_level_element_index_x_coord() const
	{
		return left_current_level_element_index 
			% get_num_level_elements_per_row();
	}
	inline u32 get_left_current_level_element_index_y_coord() const
	{
		return left_current_level_element_index 
			/ get_num_level_elements_per_row();
	}
	
	inline u32 get_right_current_level_element_index_x_coord() const
	{
		return right_current_level_element_index 
			% get_num_level_elements_per_row();
	}
	inline u32 get_right_current_level_element_index_y_coord() const
	{
		return right_current_level_element_index 
			/ get_num_level_elements_per_row();
	}
	
	
	inline u32 get_level_element_index_x_coord_of_other_index
		( u32 other_index ) const
	{
		return other_index % get_num_level_elements_per_row();
	}
	inline u32 get_level_element_index_y_coord_of_other_index
		( u32 other_index ) const
	{
		return other_index / get_num_level_elements_per_row();
	}
	
	
	inline void set_right_current_level_element_index
		( u32 n_right_current_level_element_index )
	{
		level_element_palette_modified_recently = true;
		u32 num_level_elements_per_palette 
			= get_num_level_elements_per_palette();
		
		right_current_level_element_index 
			= ( n_right_current_level_element_index 
			< num_level_elements_per_palette ) 
			? n_right_current_level_element_index 
			: ( num_level_elements_per_palette - 1 );
	}
	
	inline u32 
		level_element_selection_coords_to_current_level_element_index
		( const sf::Vector2i& pos_in_level_element_selection_coords )
	{
		//return (u32)( ( pos_in_level_element_selection_coords.y 
		//	* num_level_elements_per_row ) 
		//	+ pos_in_level_element_selection_coords.x );
		
		return (u32)( ( pos_in_level_element_selection_coords.y
			* get_num_level_elements_per_row() ) 
			+ pos_in_level_element_selection_coords.x );
	}
	
	inline u32 get_num_level_elements_per_row() const
	{
		return level_element_gfx_raw_texture.getSize().x 
			/ get_slot_inner_width();
	}
	inline u32 get_num_level_elements_per_column() const
	{
		return level_element_gfx_raw_texture.getSize().y 
			/ get_slot_inner_height();
	}
	
	inline u32 get_num_level_elements_per_row
		( u32 other_slot_inner_width ) const
	{
		return level_element_gfx_raw_texture.getSize().x 
			/ other_slot_inner_width;
	}
	inline u32 get_num_level_elements_per_column
		( u32 other_slot_inner_height ) const
	{
		return level_element_gfx_raw_texture.getSize().y 
			/ other_slot_inner_height;
	}
	
	
	
	inline u32 get_num_level_elements_per_palette() const
	{
		return get_num_level_elements_per_row() 
			* get_num_level_elements_per_column();
	}
	
	virtual inline u32 get_slot_inner_width() const
	{
		return slot_inner_width;
	}
	virtual inline u32 get_slot_inner_height() const
	{
		return slot_inner_height;
	}
	
	virtual inline u32 get_slot_outer_width() const
	{
		return slot_outer_width;
	}
	virtual inline u32 get_slot_outer_height() const
	{
		return slot_outer_height;
	}
	
	inline sf::IntRect get_left_current_texture_rect() const
	{
		return sf::IntRect
			( sf::Vector2i( get_left_current_level_element_index_x_coord()
			* get_slot_inner_width(),
			get_left_current_level_element_index_y_coord()
			* get_slot_inner_height() ),
			
			sf::Vector2i( get_slot_inner_width(), 
			get_slot_inner_height() ) );
	}
	
	inline sf::IntRect get_right_current_texture_rect() const
	{
		return sf::IntRect
			( sf::Vector2i( get_right_current_level_element_index_x_coord()
			* get_slot_inner_width(),
			get_right_current_level_element_index_y_coord()
			* get_slot_inner_height() ),
			
			sf::Vector2i( get_slot_inner_width(), 
			get_slot_inner_height() ) );
	}
	
	inline sf::IntRect get_texture_rect_of_other_index( u32 other_index ) 
		const
	{
		return sf::IntRect
			( sf::Vector2i
			( get_level_element_index_x_coord_of_other_index(other_index)
			* get_slot_inner_width(),
			get_level_element_index_y_coord_of_other_index(other_index)
			* get_slot_inner_height() ),
			
			sf::Vector2i( get_slot_inner_width(), 
			get_slot_inner_height() ) );
	}
	
	
	
	inline void set_the_level_editor_core_widget
		( level_editor_core_widget* n_the_level_editor_core_widget );
	
	
protected:		// functions
	virtual void mousePressEvent( QMouseEvent* event );
	
	//void generate_palette_render_texture();
	
	inline virtual void on_init()
	{
		cout << "level_element_selector_widget initialized!\n";
		
		generate_level_element_palette_render_texture();
	}
	
	virtual void generate_level_element_palette_render_texture();
	
	virtual void on_update();
	
};

#include "level_editor_core_widget_class.hpp"

inline void level_element_selector_core_widget_base
	::set_the_level_editor_core_widget
	( level_editor_core_widget* n_the_level_editor_core_widget )
{
	the_level_editor_core_widget = n_the_level_editor_core_widget;
}




#endif		// level_element_selector_core_widget_base_class_hpp
