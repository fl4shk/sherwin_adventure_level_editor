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


#ifndef rect_selection_stuff_class_hpp
#define rect_selection_stuff_class_hpp


#include "misc_includes.hpp"
#include "misc_types.hpp"
#include "vec2_class.hpp"
#include "sprite_level_data_stuff.hpp"
#include "sublevel_class.hpp"

//class sublevel;

// Rectangular selection drawing stuffs.  This is intended to be used
// for both individual level elements (like when a sprite is selected
// so that its properties can be modified) and groups of level
// elements.
class rect_selection_stuff
{
public:		// variables
	// Use shared_ptr's so that the_sfml_canvas_widget
	// ->the_rect_selection_stuff can be copied.
	shared_ptr<sf::Image> selection_image;
	shared_ptr<sf::Texture> selection_texture;
	shared_ptr<sf::Sprite> selection_sprite;
	
	
	// The actual rectangle representing the selected area.  It is
	// intended to have a position and size that are in block grid
	// coordinates.
	sf::IntRect selection_rect;
	
	
	
	// These are not intended to be used for the mouse_mode where
	// sprite properties can be changed.
	
	// This is the initially clicked block grid coordinate.
	vec2_s32 starting_block_grid_coords_of_mouse;
	
	// This represents the left and top values of selection_rect.
	vec2_s32 starting_block_grid_coords;
	
	// This represents the right and bottom values of selection_rect.
	vec2_s32 ending_block_grid_coords;
	
	
	
	
	
	// mouse_released is set to true when the mouse is released and the
	// selection mode is NOT for changing sprite properties.
	bool enabled, mouse_released;
	
	// Whether the rectangular selection is in the block layer or the
	// sprite layer.
	rect_selection_layer selection_layer;
	
	// This corresponds to when the level_editor_widget's
	// sprite_properties_widget should be generated.  I should perhaps have
	// a better name for this, though, since you can select a single sprite
	// using the type of rect selection that can be moved, copied, and
	// pasted.
	bool single_sprite_selected;
	
	
	
	// Stuff for moving around the selection's contents.
	bool moving;
	vec2_s32 clicked_location_in_rect;
	
	
	// This represents the left and top values of selection_rect.
	vec2_s32 starting_block_grid_coords_before_moving;
	
	// This represents the right and bottom values of selection_rect.
	vec2_s32 ending_block_grid_coords_before_moving;
	
	
	// A pointer to the current sublevel.  At some point, a pointer to an
	// instance of a class called "level" should be used instead.
	sublevel* the_sublevel;
	
	
	
	// Whether the selection was pasted.
	bool selection_was_pasted;
	// Which layer the pasted selection is from.
	rect_selection_layer original_layer_of_pasted_selection;
	
	// Whether the selection was finished being created.  This is not
	// related to pasting.
	bool selection_still_being_created;
	
	
	// Stuff to move
	vector< vector<block> > moving_block_vec_2d;
	vector< vector<sprite_init_param_group_with_size> >
		moving_sprite_ipgws_vec_2d;
	
	// Stuff to paste
	vector< vector<block> > copied_block_vec_2d;
	vector< vector<sprite_init_param_group_with_size> > 
		copied_sprite_ipgws_vec_2d;
	
	
public:		// functions
	inline rect_selection_stuff()
	{
	}
	
	inline rect_selection_stuff
		( const rect_selection_stuff& s_rect_selection_stuff )
	{
		*this = s_rect_selection_stuff;
	}
	
	inline bool get_enabled() const
	{
		return enabled;
	}
	inline bool get_mouse_released() const
	{
		return mouse_released;
	}
	inline rect_selection_layer get_selection_layer() const
	{
		return selection_layer;
	}
	inline bool get_single_sprite_selected() const
	{
		return single_sprite_selected;
	}
	inline sprite_init_param_group_with_size*
		get_single_selected_sprite_ipgws()
	{
		return &( the_sublevel->sprite_ipgws_vec_2d.at(selection_rect.top)
			.at(selection_rect.left) );
	}
	inline const sf::IntRect& get_selection_rect() const
	{
		return selection_rect;
	}
	
	inline const vec2_s32& get_starting_block_grid_coords()
		const
	{
		return starting_block_grid_coords;
	}
	
	inline bool get_moving() const
	{
		return moving;
	}
	
	inline bool get_selection_was_pasted() const
	{
		return selection_was_pasted;
	}
	inline rect_selection_layer get_original_layer_of_pasted_selection()
		const
	{
		return original_layer_of_pasted_selection;
	}
	
	
	//inline void enable_generic_rect_selection
	//	( const sf::IntRect& n_selection_rect )
	//{
	//	enabled = true;
	//	
	//	single_sprite_selected = false;
	//	
	//	selection_rect = n_selection_rect;
	//}
	
	void start_creating_selection
		( const vec2_s32& n_starting_block_grid_coords_of_mouse, 
		rect_selection_layer n_selection_layer );
	void continue_creating_selection
		( const vec2_s32& curr_block_grid_coords_of_mouse );
	void stop_creating_selection();
	
	// Stuff for moving the rectangular selection's contents.
	void start_moving_selection_contents
		( const vec2_s32 n_clicked_location_in_rect );
	void continue_moving_selection_contents
		( const vec2_s32 curr_block_grid_coords_of_mouse );
	void stop_moving_selection_contents();
	
	// 
	inline sf::IntRect get_selection_rect_before_moving() const
	{
		const vec2_s32& rs_starting_block_grid_coords_before_moving
			= starting_block_grid_coords_before_moving;
		const vec2_s32& rs_ending_block_grid_coords_before_moving
			= ending_block_grid_coords_before_moving;
		
		sf::IntRect selection_rect_before_moving
			( rs_starting_block_grid_coords_before_moving.x,
			rs_starting_block_grid_coords_before_moving.y,
			
			( rs_ending_block_grid_coords_before_moving.x 
			- rs_starting_block_grid_coords_before_moving.x + 1 ),
			
			( rs_ending_block_grid_coords_before_moving.y
			- rs_starting_block_grid_coords_before_moving.y + 1 ) );
		
		return selection_rect_before_moving;
	}
	
	void finalize_movement_of_selection_contents();
	
	
	// Copy/paste stuff
	void copy_selection_contents();
	void paste_copied_selection_contents
		( const vec2_s32& n_starting_block_grid_coords );
	
	
	void enable_single_sprite_selection
		( sprite_init_param_group_with_size* n_selected_sprite_ipgws );
	
	inline void disable_selection()
	{
		enabled = false;
	}
	
};


#endif		// rect_selection_stuff_class_hpp
