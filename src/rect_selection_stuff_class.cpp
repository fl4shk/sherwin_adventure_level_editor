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

#include "rect_selection_stuff_class.hpp"
#include "sublevel_class.hpp"
#include "sfml_canvas_widget_classes.hpp"


void rect_selection_stuff::start_creating_selection
	( const vec2_s32& n_starting_block_grid_coords_of_mouse, 
	rect_selection_layer n_selection_layer )
{
	//cout << "start_rect_selection()\n";
	
	starting_block_grid_coords_of_mouse
		= n_starting_block_grid_coords_of_mouse;
	starting_block_grid_coords = n_starting_block_grid_coords_of_mouse;
	
	selection_rect.left = n_starting_block_grid_coords_of_mouse.x;
	selection_rect.top = n_starting_block_grid_coords_of_mouse.y;
	
	single_sprite_selected = false;
	
	enabled = true;
	mouse_released = false;
	selection_layer = n_selection_layer;
	moving = false;
	
	selection_was_pasted = false;
	
	
	// Just one block
	selection_rect = sf::IntRect( n_starting_block_grid_coords_of_mouse.x,
		n_starting_block_grid_coords_of_mouse.y, 1, 1 );
	
	ending_block_grid_coords = n_starting_block_grid_coords_of_mouse;
	
	
	starting_block_grid_coords_before_moving = starting_block_grid_coords;
	ending_block_grid_coords_before_moving = ending_block_grid_coords;
}

void rect_selection_stuff::continue_creating_selection
	( const vec2_s32& curr_block_grid_coords_of_mouse )
{
	//cout << "continue_rect_selection()\n";
	
	
	if ( starting_block_grid_coords_of_mouse.x
		< curr_block_grid_coords_of_mouse.x )
	{
		starting_block_grid_coords.x
			= starting_block_grid_coords_of_mouse.x;
		ending_block_grid_coords.x = curr_block_grid_coords_of_mouse.x;
	}
	else
	{
		starting_block_grid_coords.x = curr_block_grid_coords_of_mouse.x;
		ending_block_grid_coords.x = starting_block_grid_coords_of_mouse.x;
	}
	
	if ( starting_block_grid_coords_of_mouse.y
		< curr_block_grid_coords_of_mouse.y )
	{
		starting_block_grid_coords.y
			= starting_block_grid_coords_of_mouse.y;
		ending_block_grid_coords.y = curr_block_grid_coords_of_mouse.y;
	}
	else
	{
		starting_block_grid_coords.y = curr_block_grid_coords_of_mouse.y;
		ending_block_grid_coords.y = starting_block_grid_coords_of_mouse.y;
	}
	
	
	// Correction things.
	if ( starting_block_grid_coords.x < 0 )
	{
		starting_block_grid_coords.x = 0;
	}
	if ( starting_block_grid_coords.y < 0 )
	{
		starting_block_grid_coords.y = 0;
	}
	if ( starting_block_grid_coords.x >= (s32)the_sublevel->size_2d.x )
	{
		starting_block_grid_coords.x = the_sublevel->size_2d.x - 1;
	}
	if ( starting_block_grid_coords.y >= (s32)the_sublevel->size_2d.y )
	{
		starting_block_grid_coords.y = the_sublevel->size_2d.y - 1;
	}
	
	
	if ( ending_block_grid_coords.x < 0 )
	{
		ending_block_grid_coords.x = 0;
	}
	if ( ending_block_grid_coords.y < 0 )
	{
		ending_block_grid_coords.y = 0;
	}
	if ( ending_block_grid_coords.x >= (s32)the_sublevel->size_2d.x )
	{
		ending_block_grid_coords.x = the_sublevel->size_2d.x - 1;
	}
	if ( ending_block_grid_coords.y >= (s32)the_sublevel->size_2d.y )
	{
		ending_block_grid_coords.y = the_sublevel->size_2d.y - 1;
	}
	
	selection_rect.left = starting_block_grid_coords.x;
	selection_rect.top = starting_block_grid_coords.y;
	
	selection_rect.width = ending_block_grid_coords.x 
		- starting_block_grid_coords.x + 1;
	selection_rect.height = ending_block_grid_coords.y 
		- starting_block_grid_coords.y + 1;
	
	starting_block_grid_coords_before_moving = starting_block_grid_coords;
	ending_block_grid_coords_before_moving = ending_block_grid_coords;
}

void rect_selection_stuff::stop_creating_selection()
{
	//cout << "stop_rect_selection()\n";
	mouse_released = true;
	
	starting_block_grid_coords_before_moving = starting_block_grid_coords;
	ending_block_grid_coords_before_moving = ending_block_grid_coords;
}



void rect_selection_stuff::start_moving_selection_contents
	( const vec2_s32 n_clicked_location_in_rect )
{
	//cout << "start_moving_rect_selection_contents()\n";
	
	single_sprite_selected = false;
	
	enabled = true;
	mouse_released = false;
	//selection_layer = n_selection_layer;
	moving = true;
	
	clicked_location_in_rect = n_clicked_location_in_rect;
	
	starting_block_grid_coords_of_mouse = starting_block_grid_coords
		+ n_clicked_location_in_rect;
	
}

void rect_selection_stuff::continue_moving_selection_contents
	( const vec2_s32 curr_block_grid_coords_of_mouse )
{
	//cout << "continue_moving_rect_selection_contents()\n";
	
	
	//vec2_s32 ending_block_grid_coords_offset
	//	= ending_block_grid_coords - starting_block_grid_coords;
	vec2_s32 ending_block_grid_coords_offset( selection_rect.width,
		selection_rect.height );
	
	starting_block_grid_coords = curr_block_grid_coords_of_mouse
		- clicked_location_in_rect;
	
	
	// Correction things.
	if ( starting_block_grid_coords.x < 0 )
	{
		starting_block_grid_coords.x = 0;
	}
	if ( starting_block_grid_coords.y < 0 )
	{
		starting_block_grid_coords.y = 0;
	}
	if ( starting_block_grid_coords.x >= ( (s32)the_sublevel->size_2d.x 
		- ending_block_grid_coords_offset.x ) )
	{
		starting_block_grid_coords.x = ( the_sublevel->size_2d.x - 1 )
			- ending_block_grid_coords_offset.x;
	}
	if ( starting_block_grid_coords.y >= ( (s32)the_sublevel->size_2d.y 
		- ending_block_grid_coords_offset.y ) )
	{
		starting_block_grid_coords.y = ( the_sublevel->size_2d.y - 1 )
			- ending_block_grid_coords_offset.y;
		
	}
	
	ending_block_grid_coords = starting_block_grid_coords
		+ ending_block_grid_coords_offset;
	
	if ( ending_block_grid_coords.x < ending_block_grid_coords_offset.x )
	{
		ending_block_grid_coords.x = ending_block_grid_coords_offset.x;
	}
	if ( ending_block_grid_coords.y < ending_block_grid_coords_offset.y )
	{
		ending_block_grid_coords.y = ending_block_grid_coords_offset.y;
	}
	if ( ending_block_grid_coords.x >= (s32)the_sublevel->size_2d.x )
	{
		ending_block_grid_coords.x = the_sublevel->size_2d.x - 1;
	}
	if ( ending_block_grid_coords.y >= (s32)the_sublevel->size_2d.y )
	{
		ending_block_grid_coords.y = the_sublevel->size_2d.y - 1;
	}
	
	selection_rect.left = starting_block_grid_coords.x;
	selection_rect.top = starting_block_grid_coords.y;
	
	//selection_rect.width = ending_block_grid_coords.x 
	//	- starting_block_grid_coords.x + 1;
	//selection_rect.height = ending_block_grid_coords.y 
	//	- starting_block_grid_coords.y + 1;
	
}

void rect_selection_stuff::stop_moving_selection_contents()
{
	//cout << "stop_moving_rect_selection_contents()\n"; 
	mouse_released = true;
	moving = false;
	
}


void rect_selection_stuff::finalize_movement_of_selection_contents()
{
	//cout << "finalize_movement_of_rect_selection_contents()\n";
	
	//disable_rect_selection();
	disable_selection();
	
	//if ( get_rect_selection_single_sprite_selected() )
	if ( get_single_sprite_selected() )
	{
		return;
	}
	
	//const sf::IntRect& selection_rect 
	//	= selection_rect;
	
	
	const vec2_s32& rs_starting_block_grid_coords_before_moving
		= starting_block_grid_coords_before_moving;
	const vec2_s32& rs_ending_block_grid_coords_before_moving
		= ending_block_grid_coords_before_moving;
	
	const sf::IntRect selection_rect_before_moving
		( rs_starting_block_grid_coords_before_moving.x,
		
		rs_starting_block_grid_coords_before_moving.y,
		
		( rs_ending_block_grid_coords_before_moving.x 
		- rs_starting_block_grid_coords_before_moving.x + 1 ),
		
		( rs_ending_block_grid_coords_before_moving.y
		- rs_starting_block_grid_coords_before_moving.y + 1 ) );
	
	if ( selection_rect == selection_rect_before_moving 
		&& !get_selection_was_pasted() )
	{
		return;
	}
	
	if ( selection_layer == rsl_blocks && !get_selection_was_pasted() )
	{
		vector< vector<block> > moved_blocks_in_selection_vec_2d;
		
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			moved_blocks_in_selection_vec_2d.push_back
				(vector<block>());
			
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				block& the_block = the_sublevel
					->uncompressed_block_data_vec_2d
					.at((u32)original_block_grid_pos.y)
					.at((u32)original_block_grid_pos.x);
				
				moved_blocks_in_selection_vec_2d.at(j).push_back
					(the_block);
				
				// Delete the data of the_block.
				the_block = block();
			}
		}
		
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 block_grid_pos( selection_rect.left + i,
					selection_rect.top + j );
				
				the_sublevel->uncompressed_block_data_vec_2d
					.at(block_grid_pos.y).at(block_grid_pos.x)
					= moved_blocks_in_selection_vec_2d.at(j).at(i);
			}
		}
	}
	else if ( original_layer_of_pasted_selection == rsl_blocks 
		&& get_selection_was_pasted() )
	{
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 block_grid_pos( selection_rect.left + i,
					selection_rect.top + j );
				
				the_sublevel->uncompressed_block_data_vec_2d
					.at(block_grid_pos.y).at(block_grid_pos.x)
					= copied_blocks_vec_2d.at(j).at(i);
			}
		}
	}
	else if ( selection_layer == rsl_sprites 
		&& !get_selection_was_pasted() )
	{
		vector< vector<sprite_init_param_group_with_size> > 
			moved_sprite_ipgws_in_selection_vec_2d;
		
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			moved_sprite_ipgws_in_selection_vec_2d.push_back
				(vector<sprite_init_param_group_with_size>());
			
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				sprite_init_param_group_with_size& the_sprite_ipgws 
					= the_sublevel->sprite_ipgws_vec_2d
					.at((u32)original_block_grid_pos.y)
					.at((u32)original_block_grid_pos.x);
				
				moved_sprite_ipgws_in_selection_vec_2d.at(j).push_back
					(the_sprite_ipgws);
				
				// Delete the data of the_sprite_ipgws.
				the_sprite_ipgws = sprite_init_param_group_with_size();
			}
		}
		
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 block_grid_pos( selection_rect.left + i,
					selection_rect.top + j );
				
				sprite_init_param_group_with_size& the_sprite_ipgws
					= moved_sprite_ipgws_in_selection_vec_2d.at(j).at(i);
				
				if ( the_sprite_ipgws.type != st_default )
				{
					sprite_init_param_group_with_size& the_new_sprite_ipgws 
						= the_sublevel->sprite_ipgws_vec_2d
						.at(block_grid_pos.y).at(block_grid_pos.x);
					
					the_new_sprite_ipgws = the_sprite_ipgws;
					
					the_new_sprite_ipgws.initial_block_grid_x_coord
						= block_grid_pos.x;
					the_new_sprite_ipgws.initial_block_grid_y_coord
						= block_grid_pos.y;
				}
			}
		}
	}
	else if ( original_layer_of_pasted_selection == rsl_sprites 
		&& get_selection_was_pasted() )
	{
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 block_grid_pos( selection_rect.left + i,
					selection_rect.top + j );
				
				sprite_init_param_group_with_size& the_sprite_ipgws
					= copied_sprite_ipgws_vec_2d.at(j).at(i);
				
				if ( the_sprite_ipgws.type != st_default )
				{
					sprite_init_param_group_with_size& the_new_sprite_ipgws 
						= the_sublevel->sprite_ipgws_vec_2d
						.at(block_grid_pos.y).at(block_grid_pos.x);
					
					the_new_sprite_ipgws = the_sprite_ipgws;
					
					the_new_sprite_ipgws.initial_block_grid_x_coord
						= block_grid_pos.x;
					the_new_sprite_ipgws.initial_block_grid_y_coord
						= block_grid_pos.y;
				}
			}
		}
	}
	
}


// Copy/paste stuff
void rect_selection_stuff::copy_selection_contents()
{
	if ( !get_enabled() )
	{
		return;
	}
	
	original_layer_of_pasted_selection = selection_layer;
}

void rect_selection_stuff::paste_copied_selection_contents
	( const vec2_s32& n_starting_block_grid_coords_of_mouse )
{
	selection_was_pasted = true;
	
	starting_block_grid_coords_of_mouse 
		= n_starting_block_grid_coords_of_mouse;
	
	starting_block_grid_coords = n_starting_block_grid_coords_of_mouse;
	//ending_block_grid_coords
	
}


void rect_selection_stuff::enable_single_sprite_selection
	( sprite_init_param_group_with_size* n_selected_sprite_ipgws )
{
	enabled = true;
	
	single_sprite_selected = true;
	selection_layer = rsl_sprites;
	selection_was_pasted = false;
	
	selection_rect = sf::IntRect
		( n_selected_sprite_ipgws->initial_block_grid_x_coord,
		n_selected_sprite_ipgws->initial_block_grid_y_coord,
		n_selected_sprite_ipgws->size_2d.x 
		/ sfml_canvas_widget::num_pixels_per_block_row, 
		n_selected_sprite_ipgws->size_2d.y 
		/ sfml_canvas_widget::num_pixels_per_block_column );
	
	//vec2_s32 block_grid_start_pos
	//	( (s32)n_selected_sprite_ipgws.initial_block_grid_x_coord,  
	//	(s32)n_selected_sprite_ipgws.initial_block_grid_y_coord );
	//
	//vec2_s32 block_grid_end_pos = block_grid_start_pos
	//	+ vec2_s32( (s32)n_selected_sprite_ipgws.real_size_2d.x 
	//		* (s32)sfml_canvas_widget::num_pixels_per_block_column,
	//		(s32)n_selected_sprite_ipgws.real_size_2d.y
	//		* (s32)sfml_canvas_widget::num_pixels_per_block_row );
	//
	//if ( block_grid_end_pos.x >= (s32)the_sublevel->size_2d.x )
	//{
	//	block_grid_end_pos.x = (s32)( the_sublevel->size_2d.x - 1 );
	//}
	//
	//if ( block_grid_end_pos.y >= the_sublevel->size_2d.y )
	//{
	//	block_grid_end_pos.y = (s32)( the_sublevel->size_2d.y - 1 );
	//}
}



