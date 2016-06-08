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

#include "editing_manager_class.hpp"


void editing_manager::key_press_event( QKeyEvent* event )
{
	// These probably ought to be in a toolbar of some sort, as well.
	
	level_editor_core_widget* the_core_widget = NULL;
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget = NULL;
	
	
	s32 curr_tab_index = the_level_editor_widget
		->get_curr_level_editor_core_widget_index();
	
	if ( curr_tab_index != -1 )
	{
		the_core_widget = get_the_core_widget_vec()
			.at(curr_tab_index).get();
		the_sfml_canvas_widget = the_core_widget
			->the_sfml_canvas_widget.get();
	}
	
	if ( the_core_widget == NULL )
	{
		cout << "Weird bug in key_press_event():  the_core_widget == "
			<< "NULL.\nExpect a segfault....";
	}
	
	
	rect_selection_stuff& the_rect_selection_stuff
		= the_sfml_canvas_widget->the_rect_selection_stuff;
	
	if ( event->key() == Qt::Key_A )
	{
		the_core_widget->zoom_in();
	}
	else if ( event->key() == Qt::Key_Z )
	{
		the_core_widget->zoom_out();
	}
	else if ( event->key() == Qt::Key_T )
	{
		// Temporary until a toggle button is created for this purpose.
		if ( !the_sfml_canvas_widget->get_block_grid_enabled() )
		{
			the_sfml_canvas_widget->enable_block_grid();
			//cout << "Tile grid now enabled.\n";
		}
		else
		{
			the_sfml_canvas_widget->disable_block_grid();
			//cout << "Tile grid now disabled.\n";
		}
	}
	//else if ( event->key() == Qt::Key_Q )
	//{
	//	if ( the_rect_selection_stuff.get_enabled() )
	//	{
	//		the_rect_selection_stuff
	//			.finalize_movement_of_selection_contents();
	//	}
	//	
	//	the_core_widget->the_mouse_mode = mm_place_level_elements;
	//	
	//	cout << "Current mouse mode:  place_level_elements\n";
	//}
	//else if ( event->key() == Qt::Key_W )
	//{
	//	if ( the_rect_selection_stuff.get_enabled() )
	//	{
	//		the_rect_selection_stuff
	//			.finalize_movement_of_selection_contents();
	//	}
	//	
	//	the_core_widget->the_mouse_mode = mm_select_single_sprite;
	//	
	//	cout << "Current mouse mode:  select_sprites\n";
	//}
	//else if ( event->key() == Qt::Key_E )
	//{
	//	the_core_widget->the_mouse_mode = mm_rect_selection;
	//	
	//	cout << "Current mouse mode:  rect_selection\n";
	//}
	else if ( event->key() == Qt::Key_C 
		&& the_core_widget->the_mouse_mode == mm_rect_selection 
		&& the_rect_selection_stuff.get_enabled() )
	{
		//cout << "Copying selection contents\n";
		
		the_rect_selection_stuff.copy_selection_contents();
	}
	else if ( event->key() == Qt::Key_V 
		&& the_core_widget->the_mouse_mode == mm_rect_selection )
	{
		if ( the_rect_selection_stuff.get_enabled() )
		{
			the_rect_selection_stuff
				.finalize_movement_of_selection_contents();
		}
		
		//cout << "Pasting the copied selection contents\n";
		
		//the_sfml_canvas_widget->the_rect_selection_stuff
		//	.paste_copied_selection_contents( vec2_s32( 0, 0 ) );
		
		sf::FloatRect visible_rect 
			= the_sfml_canvas_widget->get_visible_rect();
		
		u32 num_pixels_per_block_row 
			= the_sfml_canvas_widget->num_pixels_per_block_row;
		u32 num_pixels_per_block_column 
			= the_sfml_canvas_widget->num_pixels_per_block_column;
		u32 scale_factor = the_sfml_canvas_widget->scale_factor;
		
		vec2<double> visible_block_grid_start_pos
			( (double)visible_rect.left 
			/ (double)( num_pixels_per_block_column * scale_factor ), 
			(double)visible_rect.top / (double)( num_pixels_per_block_row
			* scale_factor ) );
		vec2<double> visible_block_grid_size_2d
			( (double)visible_rect.width 
			/ (double)( num_pixels_per_block_column * scale_factor ), 
			(double)visible_rect.height 
			/ (double)( num_pixels_per_block_row * scale_factor ) );
		
		// this is so that sprites larger than 16x16 pixels will be drawn
		// if their starting position is offscreen but part of them still
		// is on screen.
		--visible_block_grid_start_pos.x;
		--visible_block_grid_start_pos.y;
		
		//cout << visible_block_grid_start_pos.x << ", "
		//	<< visible_block_grid_start_pos.y << ", "
		//	<< visible_block_grid_size_2d.x << ", "
		//	<< visible_block_grid_size_2d.y << endl;
		
		++visible_block_grid_size_2d.x;
		++visible_block_grid_size_2d.y;
		
		++visible_block_grid_size_2d.x;
		++visible_block_grid_size_2d.y;
		
		if ( visible_block_grid_start_pos.x < 0 )
		{
			visible_block_grid_start_pos.x = 0;
		}
		if ( visible_block_grid_start_pos.y < 0 )
		{
			visible_block_grid_start_pos.y = 0;
		}
		
		if ( ( visible_block_grid_start_pos.x 
			+ visible_block_grid_size_2d.x )
			>= (s32)the_core_widget->the_sublevel->real_size_2d.x )
		{
			visible_block_grid_size_2d.x 
				= the_core_widget->the_sublevel->real_size_2d.x
				- visible_block_grid_start_pos.x;
		}
		if ( ( visible_block_grid_start_pos.y 
			+ visible_block_grid_size_2d.y )
			>= (s32)the_core_widget->the_sublevel->real_size_2d.y )
		{
			visible_block_grid_size_2d.y 
				= the_core_widget->the_sublevel->real_size_2d.y
				- visible_block_grid_start_pos.y;
		}
		
		
		
		// Paste at the location of the mouse.
		sf::FloatRect visible_block_grid_rect
			( visible_block_grid_start_pos.x,
			visible_block_grid_start_pos.y,
			visible_block_grid_size_2d.x, visible_block_grid_size_2d.y );
		
		
		
		sf::Vector2i mouse_pos_in_canvas_widget_coords 
			= sf::Mouse::getPosition(*the_sfml_canvas_widget);
		
		// This converts the clicked coordinate to pixel coordinates.
		sf::Vector2f mouse_pos_in_canvas_coords
			( (double)mouse_pos_in_canvas_widget_coords.x 
			/ (double)the_sfml_canvas_widget->scale_factor,
			(double)mouse_pos_in_canvas_widget_coords.y
			/ (double)the_sfml_canvas_widget->scale_factor );
		
		vec2_s32 block_grid_coords_of_mouse_pos
			= { (s32)( mouse_pos_in_canvas_coords.x
			/ ( level_editor_sfml_canvas_widget
			::num_pixels_per_block_row ) ),
			
			(s32)( ( the_sfml_canvas_widget->the_sublevel->real_size_2d.y 
			- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
			- mouse_pos_in_canvas_coords.y )
			/ level_editor_sfml_canvas_widget
			::num_pixels_per_block_column ) ) };
		
		
		if ( visible_block_grid_rect.contains
			( block_grid_coords_of_mouse_pos.x, 
			block_grid_coords_of_mouse_pos.y ) )
		{
			the_sfml_canvas_widget->the_rect_selection_stuff
				.paste_copied_selection_contents
				(block_grid_coords_of_mouse_pos);
		}
		//else
		//{
		//	the_sfml_canvas_widget->the_rect_selection_stuff
		//		.paste_copied_selection_contents( vec2_s32
		//		( visible_block_grid_start_pos.x, 
		//		visible_block_grid_start_pos.y ) );
		//}
	}
}
