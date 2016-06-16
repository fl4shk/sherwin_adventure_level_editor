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

void rect_selection_stuff::start_creating_rs
	( const vec2_s32& n_starting_block_grid_coords_of_mouse, 
	rect_selection_layer n_selection_layer )
{
	selection_still_being_created = true;
	
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

void rect_selection_stuff::continue_creating_rs
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

void rect_selection_stuff::stop_creating_rs()
{
	selection_still_being_created = false;
	
	//cout << "stop_rect_selection()\n";
	mouse_released = true;
	
	starting_block_grid_coords_before_moving = starting_block_grid_coords;
	ending_block_grid_coords_before_moving = ending_block_grid_coords;
	
	sf::IntRect&& selection_rect_before_moving 
		= get_selection_rect_before_moving();
	
	
	// The reason these copies happen is that the_sublevel can be resized
	// (in particular, made smaller in either horizontally or vertically)
	// while keeping the_rect_selection_stuff active.
	if ( selection_layer == rsl_blocks )
	{
		moving_block_vec_2d.clear();
		
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			moving_block_vec_2d.push_back(vector<block>());
			
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				if ( !the_sublevel->contains_block_grid_pos
					(original_block_grid_pos) )
				{
					//moving_block_vec_2d.at(j).push_back(block());
					moving_block_vec_2d.at(j).push_back(block());
					
					continue;
				}
				
				block the_block = the_sublevel
					->uncompressed_block_data_vec_2d
					.at((u32)original_block_grid_pos.y)
					.at((u32)original_block_grid_pos.x);
				
				moving_block_vec_2d.at(j).push_back(the_block);
			}
		}
	}
	else if ( selection_layer == rsl_sprites )
	{
		moving_sprite_ipgws_vec_2d.clear();
		
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			moving_sprite_ipgws_vec_2d.push_back(vector<sprite_ipgws>());
			
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				if ( !the_sublevel->contains_block_grid_pos
					(original_block_grid_pos) )
				{
					moving_sprite_ipgws_vec_2d.at(j)
						.push_back(sprite_ipgws());
					continue;
				}
				
				sprite_ipgws the_sprite_ipgws = the_sublevel
					->sprite_ipgws_vec_2d
					.at((u32)original_block_grid_pos.y)
					.at((u32)original_block_grid_pos.x);
				
				moving_sprite_ipgws_vec_2d.at(j).push_back
					(the_sprite_ipgws);
			}
		}
	}
	
}



void rect_selection_stuff::start_moving_rs_contents
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

void rect_selection_stuff::continue_moving_rs_contents
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
	if ( starting_block_grid_coords.x > ( (s32)the_sublevel->size_2d.x 
		- ending_block_grid_coords_offset.x ) )
	{
		starting_block_grid_coords.x = the_sublevel->size_2d.x 
			- ending_block_grid_coords_offset.x;
	}
	if ( starting_block_grid_coords.y > ( (s32)the_sublevel->size_2d.y 
		- ending_block_grid_coords_offset.y ) )
	{
		starting_block_grid_coords.y = the_sublevel->size_2d.y
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

void rect_selection_stuff::stop_moving_rs_contents()
{
	//cout << "stop_moving_rect_rs_contents()\n"; 
	mouse_released = true;
	moving = false;
	
}


void rect_selection_stuff::finalize_rs_movement()
{
	//cout << "finalize_rs_movement()\n";
	
	//disable_rect_selection();
	//disable_selection();
	
	////if ( get_rect_selection_single_sprite_selected() )
	//if ( get_single_sprite_selected() )
	//{
	//	return;
	//}
	//
	////const sf::IntRect& selection_rect 
	////	= selection_rect;
	
	sf::IntRect&& selection_rect_before_moving 
		= get_selection_rect_before_moving();
	
	//if ( selection_rect == selection_rect_before_moving 
	//	&& !get_selection_was_pasted() )
	//{
	//	return;
	//}
	
	
	// Blocks were moved, but not pasted
	if ( selection_layer == rsl_blocks && !get_selection_was_pasted() )
	{
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				if ( !the_sublevel->contains_block_grid_pos
					(original_block_grid_pos) )
				{
					continue;
				}
				
				block& the_block = the_sublevel
					->uncompressed_block_data_vec_2d
					.at((u32)original_block_grid_pos.y)
					.at((u32)original_block_grid_pos.x);
				
				// Delete the data of the_block.
				the_block = block();
			}
		}
		
		rs_movement_finalization_block_shared_code(moving_block_vec_2d);
	}
	// Blocks were pasted
	else if ( original_layer_of_pasted_selection == rsl_blocks 
		&& get_selection_was_pasted() )
	{
		rs_movement_finalization_block_shared_code(copied_block_vec_2d);
	}
	
	// Sprites were moved, but not pasted
	else if ( selection_layer == rsl_sprites 
		&& !get_selection_was_pasted() )
	{
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				if ( !the_sublevel->contains_block_grid_pos
					(original_block_grid_pos) )
				{
					continue;
				}
				
				sprite_ipgws& the_sprite_ipgws 
					= the_sublevel->sprite_ipgws_vec_2d
					.at((u32)original_block_grid_pos.y)
					.at((u32)original_block_grid_pos.x);
				
				// Delete the data of the_sprite_ipgws.
				the_sprite_ipgws = sprite_ipgws();
			}
		}
		
		rs_movement_finalization_sprite_shared_code
			(moving_sprite_ipgws_vec_2d);
	}
	// Sprites were pasted
	else if ( original_layer_of_pasted_selection == rsl_sprites 
		&& get_selection_was_pasted() )
	{
		rs_movement_finalization_sprite_shared_code
			(copied_sprite_ipgws_vec_2d);
	}
	
}

void rect_selection_stuff::rs_movement_finalization_block_shared_code
	( vector< vector<block> >& the_other_block_vec_2d )
{
	for ( s32 j=0; j<selection_rect.height; ++j )
	{
		for ( s32 i=0; i<selection_rect.width; ++i )
		{
			vec2_s32 block_grid_pos( selection_rect.left + i,
				selection_rect.top + j );
			
			if ( !the_sublevel->contains_block_grid_pos(block_grid_pos) )
			{
				//cout << "j, i == " << j << ", " << i << endl;
				continue;
			}
			
			the_sublevel->uncompressed_block_data_vec_2d
				.at(block_grid_pos.y).at(block_grid_pos.x)
				= the_other_block_vec_2d.at(j).at(i);
		}
	}
}

void rect_selection_stuff::rs_movement_finalization_sprite_shared_code
	( vector< vector<sprite_ipgws> >& the_other_sprite_ipgws_vec_2d )
{
	// These three lambda functions have, uh, weird names, which is an
	// interesting coincidence.
	auto say_that_size_2d_y_is_weird = []() -> void
	{
		cout << "rs_movement_finalization_sprite_shared_code():  weird "
			<< "issue where the_other_sprite_ipgws.size_2d.y is neither "
			<< "16 or 32.\n";
	};
	auto say_that_size_2d_x_is_weird = []() -> void
	{
		cout << "rs_movement_finalization_sprite_shared_code():  weird "
			<< "issue where the_other_sprite_ipgws.size_2d.x is neither "
			<< "16 or 32.\n";
	};
	auto say_that_size_2d_combo_is_weird = []() -> void
	{
		cout << "rs_movement_finalization_sprite_shared_code():  weird "
			<< "issue where the_other_sprite_ipgws.size_2d.x == 32 and "
			<< "the_other_sprite_ipgws.size_2d.y == 16.\n";
	};
	
	auto single_sprite_handler = [&]( const vec2_s32& block_grid_pos, 
		const sprite_ipgws& the_other_sprite_ipgws ) -> void
	{
		sprite_ipgws& the_new_sprite_ipgws 
			= the_sublevel->sprite_ipgws_vec_2d
			.at(block_grid_pos.y).at(block_grid_pos.x);
		
		if ( the_other_sprite_ipgws.type != st_default )
		{
			if ( the_other_sprite_ipgws.size_2d.x == 16 )
			{
				// 16x16
				if ( the_other_sprite_ipgws.size_2d.y == 16 )
				{
					adj_sprite_ipgws_ptr_group_16x16 ptr_group
						( *the_sublevel, block_grid_pos.x,
						block_grid_pos.y );
					
					// Erase sprites overlapped by the moved or pasted
					// sprite
					ptr_group.erase_overlapping_sprites();
					
				}
				
				// 16x32
				else if ( the_other_sprite_ipgws.size_2d.y == 32 )
				{
					adj_sprite_ipgws_ptr_group_16x32 ptr_group
						( *the_sublevel, block_grid_pos.x,
						block_grid_pos.y );
					
					// Erase sprites overlapped by the moved or pasted
					// sprite
					ptr_group.erase_overlapping_sprites();
				}
				
				// Invalid size_2d
				else
				{
					say_that_size_2d_y_is_weird();
				}
			}
			else if ( the_other_sprite_ipgws.size_2d.x == 32 )
			{
				// 32x16, which isn't allowed
				if ( the_other_sprite_ipgws.size_2d.y == 16 )
				{
					say_that_size_2d_combo_is_weird();
				}
				
				// 32x32
				else if ( the_other_sprite_ipgws.size_2d.y == 32 )
				{
					adj_sprite_ipgws_ptr_group_32x32 ptr_group
						( *the_sublevel, block_grid_pos.x,
						block_grid_pos.y );
					
					// Erase sprites overlapped by the moved or pasted
					// sprite
					ptr_group.erase_overlapping_sprites();
				}
				
				// Invalid size_2d
				else
				{
					say_that_size_2d_y_is_weird();
				}
			}
			else
			{
				say_that_size_2d_x_is_weird();
			}
			
			
			the_new_sprite_ipgws = the_other_sprite_ipgws;
			
			// Don't forget to update the intial block grid
			// coordinates, since the_new_sprite_ipgws is at a
			// different location from the_other_sprite_ipgws.
			the_new_sprite_ipgws.initial_block_grid_x_coord
				= block_grid_pos.x;
			the_new_sprite_ipgws.initial_block_grid_y_coord
				= block_grid_pos.y;
			
		}
		//// Erase sprites even when there's not one in the specific slot
		//else //if ( the_other_sprite_ipgws.type == st_default )
		//{
		//	the_new_sprite_ipgws = sprite_ipgws();
		//}
	};
	
	
	for ( s32 j=0; j<selection_rect.height; ++j )
	{
		for ( s32 i=0; i<selection_rect.width; ++i )
		{
			vec2_s32 block_grid_pos( selection_rect.left + i,
				selection_rect.top + j );
			
			if ( !the_sublevel->contains_block_grid_pos(block_grid_pos) )
			{
				continue;
			}
			
			const sprite_ipgws& the_other_sprite_ipgws 
				= the_other_sprite_ipgws_vec_2d.at(j).at(i);
			
			single_sprite_handler( block_grid_pos,
				the_other_sprite_ipgws );
		}
	}
}


// Copy/paste stuff
void rect_selection_stuff::copy_rs_contents()
{
	if ( !get_enabled() || get_single_sprite_selected() )
	{
		return;
	}
	
	original_layer_of_pasted_selection = selection_layer;
	
	
	sf::IntRect&& selection_rect_before_moving 
		= get_selection_rect_before_moving();
	
	auto shared_copying_code = [&]( bool copy_sprites ) -> void
	{
		if ( !copy_sprites )
		{
			copied_block_vec_2d.clear();
		}
		else //if ( copy_sprites )
		{
			copied_sprite_ipgws_vec_2d.clear();
		}
		
		for ( s32 j=0; j<selection_rect.height; ++j )
		{
			if ( !copy_sprites )
			{
				copied_block_vec_2d.push_back(vector<block>());
			}
			else //if ( copy_sprites )
			{
				copied_sprite_ipgws_vec_2d.push_back
					(vector<sprite_ipgws>());
			}
			
			for ( s32 i=0; i<selection_rect.width; ++i )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				if ( the_sublevel->contains_block_grid_pos
					(original_block_grid_pos) )
				{
					if ( !copy_sprites )
					{
						copied_block_vec_2d.at(j).push_back( the_sublevel
							->uncompressed_block_data_vec_2d
							.at((u32)original_block_grid_pos.y)
							.at((u32)original_block_grid_pos.x) );
					}
					else //if ( copy_sprites )
					{
						copied_sprite_ipgws_vec_2d.at(j).push_back
							( the_sublevel->sprite_ipgws_vec_2d
							.at((u32)original_block_grid_pos.y)
							.at((u32)original_block_grid_pos.x) );
					}
				}
				else //if ( !the_sublevel->contains_block_grid_pos
					//(original_block_grid_pos) )
				{
					if ( !copy_sprites )
					{
						copied_block_vec_2d.at(j).push_back(block());
					}
					else //if ( copy_sprites )
					{
						copied_sprite_ipgws_vec_2d.at(j).push_back
							(sprite_ipgws());
					}
				}
				
				//cout << i << ", " << j << ";  "
				//	<< the_sublevel->contains_block_grid_pos
				//	(original_block_grid_pos) << endl;
			}
		}
	};
	
	if ( original_layer_of_pasted_selection == rsl_blocks )
	{
		shared_copying_code(false);
	}
	else if ( original_layer_of_pasted_selection == rsl_sprites )
	{
		shared_copying_code(true);
	}
	
}


// This function doesn't actually affect the_sublevel directly
void rect_selection_stuff::paste_copied_rs_contents
	( const vec2_s32& n_starting_block_grid_coords )
{
	if ( original_layer_of_pasted_selection == rsl_blocks 
		&& copied_block_vec_2d.size() == 0 )
	{
		return;
	}
	else if ( original_layer_of_pasted_selection == rsl_sprites 
		&& copied_sprite_ipgws_vec_2d.size() == 0 )
	{
		return;
	}
	
	enabled = true;
	mouse_released = true;
	moving = false;
	single_sprite_selected = false;
	
	selection_was_pasted = true;
	
	starting_block_grid_coords = n_starting_block_grid_coords;
	
	selection_rect.left = starting_block_grid_coords.x;
	selection_rect.top = starting_block_grid_coords.y;
	
	if ( original_layer_of_pasted_selection == rsl_blocks )
	{
		//selection_rect.width = copied_block_vec_2d.at(0).size();
		selection_rect.width = copied_block_vec_2d.back().size();
		selection_rect.height = copied_block_vec_2d.size();
	}
	else if ( original_layer_of_pasted_selection == rsl_sprites )
	{
		//selection_rect.width = copied_sprite_ipgws_vec_2d.at(0).size();
		selection_rect.width = copied_sprite_ipgws_vec_2d.back().size();
		selection_rect.height = copied_sprite_ipgws_vec_2d.size();
	}
	
	ending_block_grid_coords = starting_block_grid_coords
		+ vec2_s32( selection_rect.width, selection_rect.height );
	
	
	
}


void rect_selection_stuff::enable_single_sprite_selection
	( sprite_ipgws* n_selected_sprite_ipgws )
{
	enabled = true;
	
	single_sprite_selected = true;
	selection_layer = rsl_sprites;
	selection_was_pasted = false;
	
	selection_rect = sf::IntRect
		( n_selected_sprite_ipgws->initial_block_grid_x_coord,
		n_selected_sprite_ipgws->initial_block_grid_y_coord,
		n_selected_sprite_ipgws->size_2d.x 
		/ level_editor_sfml_canvas_widget::num_pixels_per_block_row, 
		n_selected_sprite_ipgws->size_2d.y 
		/ level_editor_sfml_canvas_widget::num_pixels_per_block_column );
	
	//vec2_s32 block_grid_start_pos
	//	( (s32)n_selected_sprite_ipgws.initial_block_grid_x_coord,  
	//	(s32)n_selected_sprite_ipgws.initial_block_grid_y_coord );
	//
	//vec2_s32 block_grid_end_pos = block_grid_start_pos
	//	+ vec2_s32( (s32)n_selected_sprite_ipgws.real_size_2d.x 
	//		* (s32)level_editor_sfml_canvas_widget::num_pixels_per_block_column,
	//		(s32)n_selected_sprite_ipgws.real_size_2d.y
	//		* (s32)level_editor_sfml_canvas_widget::num_pixels_per_block_row );
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



