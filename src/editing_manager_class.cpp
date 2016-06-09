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
//#include "sfml_canvas_widget_classes.hpp"
//#include "level_editor_core_widget_class.hpp"
//#include "sublevel_class.hpp"

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
		
		copy_selection_contents(the_rect_selection_stuff);
	}
	else if ( event->key() == Qt::Key_V 
		&& the_core_widget->the_mouse_mode == mm_rect_selection )
	{
		paste_copied_selection_contents( the_sfml_canvas_widget,
			the_rect_selection_stuff );
	}
}

void editing_manager::mouse_press_event
	( level_editor_core_widget* the_core_widget, QMouseEvent* event )
{
	get_le_core_widget_stuff(the_core_widget);
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	if ( !the_sfml_canvas_widget->block_grid_pos_is_in_sublevel
		(block_grid_coords_of_mouse_pos) )
	{
		return;
	}
	
	
	if ( !the_sfml_canvas_widget->point_is_in_visible_rect
		( sf::Vector2i( event->x(), event->y() ) ) )
	{
		//cout << "out of bounds\n";
		return;
	}
	
	
	block& the_block_at_mouse_pos 
		= the_core_widget->the_sublevel->uncompressed_block_data_vec_2d
		.at((u32)block_grid_coords_of_mouse_pos.y)
		.at((u32)block_grid_coords_of_mouse_pos.x);
	
	sprite_init_param_group_with_size& the_sprite_ipgws_at_mouse_pos
		= the_core_widget->the_sublevel->sprite_ipgws_vec_2d
		.at((u32)block_grid_coords_of_mouse_pos.y)
		.at((u32)block_grid_coords_of_mouse_pos.x);
	
	
	bool current_tabbed_widget_is_for_blocks 
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_block_selector_widget );
	bool current_tabbed_widget_is_for_16x16_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x16_selector_widget );
	bool current_tabbed_widget_is_for_16x32_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x32_selector_widget );
	
	
	//adj_sprite_ipgws_ptr_group_for_selecting_sprite
	//	the_sprite_selection_ptr_group( *the_core_widget->the_sublevel,
	//	block_grid_coords_of_mouse_pos.x, 
	//	block_grid_coords_of_mouse_pos.y );
	
	
	the_sprite_selection_ptr_group 
		= adj_sprite_ipgws_ptr_group_for_selecting_sprite
		( *the_core_widget->the_sublevel, 
		block_grid_coords_of_mouse_pos.x, 
		block_grid_coords_of_mouse_pos.y );
	
	
	if ( the_sprite_selection_ptr_group.origin_ptr != NULL )
	{
		if ( the_sprite_selection_ptr_group.origin_ptr->type 
			== st_default )
		{
			//if ( the_sfml_canvas_widget
			//	->get_rect_selection_single_sprite_selected() )
			//{
			//	the_sfml_canvas_widget->disable_rect_selection();
			//}
			
			//if ( the_sfml_canvas_widget
			//	->get_rect_selection_single_sprite_selected()
			//	&& the_sfml_canvas_widget->get_rect_selection_enabled()
			//	&& the_sfml_canvas_widget
			//	->get_rect_selection_mouse_released() )
			if ( the_sfml_canvas_widget->the_rect_selection_stuff
				.get_single_sprite_selected()
				&& the_sfml_canvas_widget->the_rect_selection_stuff
				.get_enabled()
				&& the_sfml_canvas_widget->the_rect_selection_stuff
				.get_mouse_released() )
			{
				//the_sfml_canvas_widget
				//	->finalize_movement_of_rect_selection_contents();
				the_sfml_canvas_widget->the_rect_selection_stuff
					.finalize_movement_of_selection_contents();
			}
			
			//cout << "st_default\n";
			//emit sprite_no_longer_selected();
			the_core_widget->do_emit_sprite_no_longer_selected();
		}
	}
	// I am not sure this will ever be the case.
	else 
	{
		//if ( the_sfml_canvas_widget
		//	->get_rect_selection_single_sprite_selected() )
		//{
		//	the_sfml_canvas_widget->disable_rect_selection();
		//}
		//if ( the_sfml_canvas_widget->get_rect_selection_enabled()
		//	&& the_sfml_canvas_widget
		//	->get_rect_selection_mouse_released() )
		//{
		//	the_sfml_canvas_widget
		//		->finalize_movement_of_rect_selection_contents();
		//}
		
		//if ( the_sfml_canvas_widget
		//	->get_rect_selection_single_sprite_selected()
		//	&& the_sfml_canvas_widget->get_rect_selection_enabled()
		//	&& the_sfml_canvas_widget
		//	->get_rect_selection_mouse_released() )
		if ( the_sfml_canvas_widget->the_rect_selection_stuff
			.get_single_sprite_selected()
			&& the_sfml_canvas_widget->the_rect_selection_stuff
			.get_enabled()
			&& the_sfml_canvas_widget->the_rect_selection_stuff
			.get_mouse_released() )
		{
			//the_sfml_canvas_widget
			//	->finalize_movement_of_rect_selection_contents();
			the_sfml_canvas_widget->the_rect_selection_stuff
				.finalize_movement_of_selection_contents();
		}
		//cout << "else\n";
		//emit sprite_no_longer_selected();
		the_core_widget->do_emit_sprite_no_longer_selected();
	}
	
	
	auto func_for_placing_level_elements = [&]() -> void
	{
		//if ( the_sfml_canvas_widget->get_rect_selection_enabled()
		//	&& the_sfml_canvas_widget
		//	->get_rect_selection_mouse_released() )
		if ( the_sfml_canvas_widget->the_rect_selection_stuff
			.get_enabled() 
			&& the_sfml_canvas_widget->the_rect_selection_stuff
			.get_mouse_released() )
		{
			//the_sfml_canvas_widget
			//	->finalize_movement_of_rect_selection_contents();
			the_sfml_canvas_widget->the_rect_selection_stuff
				.finalize_movement_of_selection_contents();
		}
		
		the_sfml_canvas_widget->modified_recently = true;
		
		if (current_tabbed_widget_is_for_blocks)
		{
			//cout << "the_block_selector_widget_is_enabled!\n";
			
			the_block_at_mouse_pos.type 
				= the_core_widget->get_the_block_selector_core_widget()
				->get_left_current_level_element_index();
			
			//cout << block_stuff::get_bt_name
			//	((block_type)the_block_at_mouse_pos.type) << endl;
		}
		
		else if (current_tabbed_widget_is_for_16x16_sprites)
		{
			//cout << "the_sprite_16x16_selector_widget_is_enabled!\n";
			
			auto add_sprite = [&]() -> void
			{
				//// Delete the data (if any)
				//the_sprite_ipgws_at_mouse_pos 
				//	= sprite_init_param_group_with_size();
				
				the_sprite_ipgws_at_mouse_pos.type 
					= (sprite_type)(the_core_widget
					->get_the_sprite_16x16_selector_core_widget()
					->get_left_current_level_element_index());
				
				the_sprite_ipgws_at_mouse_pos
					.initial_block_grid_x_coord
					= block_grid_coords_of_mouse_pos.x;
				the_sprite_ipgws_at_mouse_pos
					.initial_block_grid_y_coord
					= block_grid_coords_of_mouse_pos.y;
				
				the_sprite_ipgws_at_mouse_pos.size_2d 
					= vec2_u32( 16, 16 );
			};
			
			adj_sprite_ipgws_ptr_group_for_placing_sprite_16x16 
				the_ptr_group( *the_core_widget->the_sublevel, 
				block_grid_coords_of_mouse_pos.x,
				block_grid_coords_of_mouse_pos.y );
			
			if ( the_ptr_group.can_add_sprite() )
			{
				add_sprite();
			}
		}
		
		else if (current_tabbed_widget_is_for_16x32_sprites)
		{
			//cout << "the_sprite_16x32_selector_widget_is_enabled!\n";
			
			auto add_sprite = [&]() -> void
			{
				the_sprite_ipgws_at_mouse_pos.type 
					= (sprite_type)(the_core_widget
					->get_the_sprite_16x32_selector_core_widget()
					->get_left_current_level_element_index());
				
				the_sprite_ipgws_at_mouse_pos.initial_block_grid_x_coord
					= block_grid_coords_of_mouse_pos.x;
				the_sprite_ipgws_at_mouse_pos.initial_block_grid_y_coord
					= block_grid_coords_of_mouse_pos.y;
				
				the_sprite_ipgws_at_mouse_pos.size_2d = vec2_u32( 16, 32 );
			};
			
			adj_sprite_ipgws_ptr_group_for_placing_sprite_16x32 
				the_ptr_group( *the_core_widget->the_sublevel, 
				block_grid_coords_of_mouse_pos.x,
				block_grid_coords_of_mouse_pos.y );
			
			
			if ( the_ptr_group.can_add_sprite() )
			{
				add_sprite();
			}
			
		}
		
		//cout << endl;
	};
	
	auto func_for_erasing_sprites = [&]() -> void
	{
		
	};
	
	auto func_for_selecting_single_sprite = [&]() -> void
	{
		if ( the_sprite_selection_ptr_group.origin_ptr != NULL )
		{
			if ( the_sprite_selection_ptr_group.origin_ptr->type 
				== st_default )
			{
				//the_sfml_canvas_widget->disable_rect_selection();
				//if ( the_sfml_canvas_widget->get_rect_selection_enabled()
				//	&& the_sfml_canvas_widget
				//	->get_rect_selection_mouse_released() )
				if ( the_sfml_canvas_widget->the_rect_selection_stuff
					.get_enabled()
					&& the_sfml_canvas_widget->the_rect_selection_stuff
					.get_mouse_released() )
				{
					//the_sfml_canvas_widget
					//	->finalize_movement_of_rect_selection_contents();
					the_sfml_canvas_widget->the_rect_selection_stuff
						.finalize_movement_of_selection_contents();
				}
				
				//cout << "st_default\n";
				
				return;
			}
		}
		// I am not sure this will ever be the case.
		else 
		{
			//the_sfml_canvas_widget->disable_rect_selection();
			
			//if ( the_sfml_canvas_widget->get_rect_selection_enabled()
			//	&& the_sfml_canvas_widget
			//	->get_rect_selection_mouse_released() )
			if ( the_sfml_canvas_widget->the_rect_selection_stuff
				.get_enabled()
				&& the_sfml_canvas_widget->the_rect_selection_stuff
				.get_mouse_released() )
			{
				//the_sfml_canvas_widget
				//	->finalize_movement_of_rect_selection_contents();
				the_sfml_canvas_widget->the_rect_selection_stuff
					.finalize_movement_of_selection_contents();
			}
			
			
			//cout << "else\n";
			
			//emit sprite_no_longer_selected();
			the_core_widget->do_emit_sprite_no_longer_selected();
			return;
		}
		
		sprite_init_param_group_with_size* clicked_sprite_ipgws 
			= the_sprite_selection_ptr_group.origin_ptr;
		
		//the_sfml_canvas_widget->enable_single_sprite_rect_selection
		//	(clicked_sprite_ipgws);
		the_sfml_canvas_widget->the_rect_selection_stuff
			.enable_single_sprite_selection(clicked_sprite_ipgws);
		
		//emit sprite_was_selected();
		the_core_widget->do_emit_sprite_was_selected();
	};
	
	auto func_for_rect_selection = [&]() -> void
	{
		bool clicked_location_intersects_rect = the_sfml_canvas_widget
			->the_rect_selection_stuff.get_selection_rect().contains
			( block_grid_coords_of_mouse_pos.x, 
			block_grid_coords_of_mouse_pos.y );
		
		//cout << "clicked_location_intersects_rect:  "
		//	<< clicked_location_intersects_rect << endl;
		//
		//cout << "rect selection enabled:  " 
		//	<< the_sfml_canvas_widget->the_rect_selection_stuff
		//	.get_enabled() << endl;
		
		
		//if ( !the_sfml_canvas_widget->get_rect_selection_enabled() 
		//	|| ( the_sfml_canvas_widget->get_rect_selection_enabled() 
		//	&& !clicked_location_intersects_rect ) )
		if ( !the_sfml_canvas_widget->the_rect_selection_stuff
			.get_enabled() 
			|| ( the_sfml_canvas_widget->the_rect_selection_stuff
			.get_enabled() 
			&& !clicked_location_intersects_rect ) )
		{
			//if ( the_sfml_canvas_widget->get_rect_selection_enabled()
			//	&& the_sfml_canvas_widget
			//	->get_rect_selection_mouse_released() )
			if ( the_sfml_canvas_widget->the_rect_selection_stuff
				.get_enabled()
				&& the_sfml_canvas_widget->the_rect_selection_stuff
				.get_mouse_released() )
			{
				the_sfml_canvas_widget->the_rect_selection_stuff
					.finalize_movement_of_selection_contents();
			}
			
			if (current_tabbed_widget_is_for_blocks)
			{
				the_sfml_canvas_widget->the_rect_selection_stuff
					.start_creating_selection
					( block_grid_coords_of_mouse_pos, rsl_blocks );
			}
			else if ( current_tabbed_widget_is_for_16x16_sprites 
				|| current_tabbed_widget_is_for_16x32_sprites )
			{
				the_sfml_canvas_widget->the_rect_selection_stuff
					.start_creating_selection
					( block_grid_coords_of_mouse_pos, rsl_sprites );
			}
		}
		//else if ( the_sfml_canvas_widget->get_rect_selection_enabled()
		//	&& clicked_location_intersects_rect )
		else if ( the_sfml_canvas_widget->the_rect_selection_stuff
			.get_enabled() && clicked_location_intersects_rect )
		{
			//vec2_s32 clicked_location_in_rect
			//	= block_grid_coords_of_mouse_pos - the_sfml_canvas_widget
			//	->get_rect_selection_starting_block_grid_coords();
			vec2_s32 clicked_location_in_rect
				= block_grid_coords_of_mouse_pos - the_sfml_canvas_widget
				->the_rect_selection_stuff
				.get_starting_block_grid_coords();
			
			//the_sfml_canvas_widget->start_moving_rect_selection_contents
			//	(clicked_location_in_rect);
			the_sfml_canvas_widget->the_rect_selection_stuff
				.start_moving_selection_contents(clicked_location_in_rect);
		}
		
		//emit sprite_no_longer_selected();
		the_core_widget->do_emit_sprite_no_longer_selected();
	};
	
	if ( event->buttons() == Qt::LeftButton )
	{
		//cout << block_grid_coords_of_mouse_pos.x << ", "
		//	<< block_grid_coords_of_mouse_pos.y << endl;
		
		switch (the_mouse_mode)
		{
			case mm_place_level_elements:
				func_for_placing_level_elements();
				break;
			
			case mm_erase_sprites:
				func_for_erasing_sprites();
				break;
			
			case mm_select_single_sprite:
				func_for_selecting_single_sprite();
				break;
			
			case mm_rect_selection:
				func_for_rect_selection();
				break;
			
			default:
				cout << "Darn it.  I don't know what enum value "
					<< "the_mouse_mode is supposed to represent right "
					<< "now.  Note to the programmer:  there is a bug of "
					<< "some sort!\n";
				break;
		}
		
	}
	else if ( event->buttons() == Qt::RightButton )
	{
		
		////emit right_mouse_button_pressed();
		//the_core_widget->do_emit_right_mouse_button_pressed();
	}
	
	
	//prev_mouse_pos = event->pos();
	block_grid_coords_of_prev_mouse_pos = block_grid_coords_of_mouse_pos;
	
}

void editing_manager::mouse_move_event
	( level_editor_core_widget* the_core_widget, QMouseEvent* event )
{
	//cout << "mouseMoveEvent()\n";
	
	get_le_core_widget_stuff(the_core_widget);
	
	//level_editor_sfml_canvas_widget* the_sfml_canvas_widget 
	//	= the_core_widget->the_sfml_canvas_widget.get();
	//QTabWidget* level_element_selectors_tab_widget 
	//	= the_core_widget->level_element_selectors_tab_widget;
	//
	//block_selector_widget* the_block_selector_widget 
	//	= the_core_widget->the_block_selector_widget;
	//sprite_16x16_selector_widget* the_sprite_16x16_selector_widget
	//	= the_core_widget->the_sprite_16x16_selector_widget;
	//sprite_16x32_selector_widget* the_sprite_16x32_selector_widget
	//	= the_core_widget->the_sprite_16x32_selector_widget;
	//
	//adj_sprite_ipgws_ptr_group_for_selecting_sprite&
	//	the_sprite_selection_ptr_group 
	//	= the_core_widget->the_sprite_selection_ptr_group;
	//
	//mouse_mode& the_mouse_mode = the_core_widget->the_mouse_mode;
	//vec2_s32& block_grid_coords_of_prev_mouse_pos = the_core_widget
	//	->block_grid_coords_of_prev_mouse_pos;
	//
	//sf::Vector2i mouse_pos_in_canvas_widget_coords 
	//	= sf::Mouse::getPosition(*the_sfml_canvas_widget);
	//
	//// This converts the clicked coordinate to pixel coordinates.
	//sf::Vector2f mouse_pos_in_canvas_coords
	//	( (double)mouse_pos_in_canvas_widget_coords.x 
	//	/ (double)the_sfml_canvas_widget->scale_factor,
	//	(double)mouse_pos_in_canvas_widget_coords.y
	//	/ (double)the_sfml_canvas_widget->scale_factor );
	//
	//u32 scale_factor = the_sfml_canvas_widget->scale_factor;
	//
	//vec2_s32 block_grid_coords_of_mouse_pos
	//	= { (s32)( mouse_pos_in_canvas_coords.x
	//	/ ( level_editor_sfml_canvas_widget
	//	::num_pixels_per_block_row ) ),
	//	
	//	(s32)( ( the_core_widget->the_sublevel->real_size_2d.y 
	//	- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
	//	- mouse_pos_in_canvas_coords.y )
	//	/ level_editor_sfml_canvas_widget
	//	::num_pixels_per_block_column ) ) };
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	//if ( !the_sfml_canvas_widget->block_grid_pos_is_in_sublevel
	//	(block_grid_coords_of_mouse_pos) )
	//{
	//	return;
	//}
	//
	//if ( !the_sfml_canvas_widget->point_is_in_visible_rect
	//	( sf::Vector2i( event->x(), event->y() ) ) )
	//{
	//	//cout << "out of bounds\n";
	//	return;
	//}
	
	
	
	//sprite_init_param_group_with_size& the_sprite_ipgws_at_mouse_pos
	//	= the_core_widget->the_sublevel->sprite_ipgws_vec_2d
	//	[block_grid_coords_of_mouse_pos.y]
	//	[block_grid_coords_of_mouse_pos.x];
	
	
	bool current_tabbed_widget_is_for_blocks 
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_block_selector_widget );
	//bool current_tabbed_widget_is_for_16x16_sprites
	//	= ( level_element_selectors_tab_widget->currentWidget()
	//	== the_sprite_16x16_selector_widget );
	//bool current_tabbed_widget_is_for_16x32_sprites
	//	= ( level_element_selectors_tab_widget->currentWidget()
	//	== the_sprite_16x32_selector_widget );
	
	
	
	
	//adj_sprite_ipgws_ptr_group_for_selecting_sprite
	//	the_sprite_selection_ptr_group( *the_core_widget->the_sublevel,
	//	block_grid_coords_of_mouse_pos.x, 
	//	block_grid_coords_of_mouse_pos.y );
	
	the_sprite_selection_ptr_group 
		= adj_sprite_ipgws_ptr_group_for_selecting_sprite
		( *the_core_widget->the_sublevel, 
		block_grid_coords_of_mouse_pos.x, 
		block_grid_coords_of_mouse_pos.y );
	
	//if ( the_sprite_selection_ptr_group.origin_ptr != NULL )
	//{
	//	if ( the_sprite_selection_ptr_group.origin_ptr->type 
	//		== st_default )
	//	{
	//		the_sfml_canvas_widget->disable_rect_selection();
	//		
	//		//cout << "st_default\n";
	//		////emit sprite_no_longer_selected();
	//		//the_core_widget->do_emit_sprite_no_longer_selected();
	//	}
	//}
	//// I am not sure this will ever be the case.
	//else 
	//{
	//	the_sfml_canvas_widget->disable_rect_selection();
	//	
	//	//cout << "else\n";
	//	//emit sprite_no_longer_selected();
	//	the_core_widget->do_emit_sprite_no_longer_selected();
	//}
	
	
	auto func_for_placing_level_elements = [&]() -> void
	{
		the_sfml_canvas_widget->modified_recently = true;
		//cout << "placing level elements\n";
		
		if (current_tabbed_widget_is_for_blocks)
		{
			//cout << "the_block_selector_widget_is_enabled!\n";
			
			if ( block_grid_coords_of_mouse_pos.x < (s32)0
				|| block_grid_coords_of_mouse_pos.x 
					>= (s32)the_core_widget->the_sublevel->real_size_2d.x
				|| block_grid_coords_of_mouse_pos.y < (s32)0
				|| block_grid_coords_of_mouse_pos.y 
					>= (s32)the_core_widget->the_sublevel
				->real_size_2d.y )
			{
				//return;
			}
			else
			{
				block& the_block_at_mouse_pos 
					= the_core_widget->the_sublevel
					->uncompressed_block_data_vec_2d
					.at((u32)block_grid_coords_of_mouse_pos.y)
					.at((u32)block_grid_coords_of_mouse_pos.x);
				
				the_block_at_mouse_pos.type = the_core_widget
					->get_the_block_selector_core_widget()
					->get_left_current_level_element_index();
			}
			
			the_core_widget->draw_block_line
				( block_grid_coords_of_prev_mouse_pos,
				block_grid_coords_of_mouse_pos, 
				(block_type)the_core_widget
				->get_the_block_selector_core_widget()
				->get_left_current_level_element_index() );
		}
		
		//cout << endl;
	};
	
	auto func_for_erasing_sprites = [&]() -> void
	{
		
	};
	
	auto func_for_selecting_single_sprite = [&]() -> void
	{
	};
	
	auto func_for_rect_selection = [&]() -> void
	{
		if ( !the_sfml_canvas_widget->the_rect_selection_stuff
			.get_moving() )
		{
			the_sfml_canvas_widget->the_rect_selection_stuff
				.continue_creating_selection
				(block_grid_coords_of_mouse_pos);
		}
		else //if ( the_sfml_canvas_widget->the_rect_selection_stuff
			// .get_moving() )
		{
			the_sfml_canvas_widget->the_rect_selection_stuff
				.continue_moving_selection_contents
				(block_grid_coords_of_mouse_pos);
		}
		
		//emit sprite_no_longer_selected();
		the_core_widget->do_emit_sprite_no_longer_selected();
	};
	
	if ( event->buttons() & Qt::LeftButton )
	{
		//cout << block_grid_coords_of_mouse_pos.x << ", "
		//	<< block_grid_coords_of_mouse_pos.y << endl;
		
		switch (the_mouse_mode)
		{
			case mm_place_level_elements:
				func_for_placing_level_elements();
				break;
			
			case mm_erase_sprites:
				func_for_erasing_sprites();
				break;
			
			case mm_select_single_sprite:
				func_for_selecting_single_sprite();
				break;
			
			case mm_rect_selection:
				func_for_rect_selection();
				break;
			
			default:
				cout << "Darn it.  I don't know what enum value "
					<< "the_mouse_mode is supposed to represent right "
					<< "now.  Note to the programmer:  there is a bug of "
					<< "some sort!\n";
				break;
		}
		
	}
	else if ( event->buttons() & Qt::RightButton )
	{
		
		//emit right_mouse_button_pressed();
	}
	
	
	//prev_mouse_pos = event->pos();
	block_grid_coords_of_prev_mouse_pos = block_grid_coords_of_mouse_pos;
}

void editing_manager::mouse_release_event
	( level_editor_core_widget* the_core_widget, QMouseEvent* event )
{
	//cout << "mouseReleaseEvent()\n";
	
	get_le_core_widget_stuff(the_core_widget);
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	//if ( !the_sfml_canvas_widget->block_grid_pos_is_in_sublevel
	//	(block_grid_coords_of_mouse_pos) )
	//{
	//	return;
	//}
	
	//if ( !the_sfml_canvas_widget->point_is_in_visible_rect
	//	( sf::Vector2i( event->x(), event->y() ) ) )
	//{
	//	//cout << "out of bounds\n";
	//	return;
	//}
	
	
	//block& the_block_at_mouse_pos 
	//	= the_core_widget->the_sublevel->uncompressed_block_data_vec_2d
	//	.at((u32)block_grid_coords_of_mouse_pos.y)
	//	.at((u32)block_grid_coords_of_mouse_pos.x);
	
	//sprite_init_param_group_with_size& the_sprite_ipgws_at_mouse_pos
	//	= the_core_widget->the_sublevel->sprite_ipgws_vec_2d
	//	[block_grid_coords_of_mouse_pos.y]
	//	[block_grid_coords_of_mouse_pos.x];
	
	
	bool current_tabbed_widget_is_for_blocks 
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_block_selector_widget );
	bool current_tabbed_widget_is_for_16x16_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x16_selector_widget );
	bool current_tabbed_widget_is_for_16x32_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x32_selector_widget );
	
	
	
	//the_sprite_selection_ptr_group 
	//	= adj_sprite_ipgws_ptr_group_for_selecting_sprite
	//	( *the_core_widget->the_sublevel, 
	//	block_grid_coords_of_mouse_pos.x, 
	//	block_grid_coords_of_mouse_pos.y );
	
	
	
	auto func_for_placing_level_elements = [&]() -> void
	{
	};
	
	auto func_for_erasing_sprites = [&]() -> void
	{
		
	};
	
	auto func_for_selecting_single_sprite = [&]() -> void
	{
	};
	
	auto func_for_rect_selection = [&]() -> void
	{
		if ( !the_sfml_canvas_widget->the_rect_selection_stuff
			.get_moving() )
		{
			the_sfml_canvas_widget->the_rect_selection_stuff
				.stop_creating_selection();
		}
		else //if ( the_sfml_canvas_widget->the_rect_selection_stuff
			//.get_moving() )
		{
			the_sfml_canvas_widget->the_rect_selection_stuff
				.stop_moving_selection_contents();
		}
		
		//emit sprite_no_longer_selected();
		the_core_widget->do_emit_sprite_no_longer_selected();
	};
	
	//cout << block_grid_coords_of_mouse_pos.x << ", "
	//	<< block_grid_coords_of_mouse_pos.y << endl;
	
	switch (the_mouse_mode)
	{
		case mm_place_level_elements:
			func_for_placing_level_elements();
			break;
		
		case mm_erase_sprites:
			func_for_erasing_sprites();
			break;
		
		case mm_select_single_sprite:
			func_for_selecting_single_sprite();
			break;
		
		case mm_rect_selection:
			func_for_rect_selection();
			break;
		
		default:
			cout << "Darn it.  I don't know what enum value "
				<< "the_mouse_mode is supposed to represent right "
				<< "now.  Note to the programmer:  there is a bug of "
				<< "some sort!\n";
			break;
	}
	
	
	//prev_mouse_pos = event->pos();
	block_grid_coords_of_prev_mouse_pos = block_grid_coords_of_mouse_pos;
}

void editing_manager::copy_selection_contents
	( rect_selection_stuff& the_rect_selection_stuff )
{
	the_rect_selection_stuff.copy_selection_contents();
}


void editing_manager::paste_copied_selection_contents
	( level_editor_sfml_canvas_widget* the_sfml_canvas_widget,
	rect_selection_stuff& the_rect_selection_stuff )
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
	
	
	
	vec2<double> visible_block_grid_start_pos = the_sfml_canvas_widget
		->get_basic_visible_block_grid_start_pos(visible_rect);
	vec2<double> visible_block_grid_size_2d = the_sfml_canvas_widget
		->get_basic_visible_block_grid_size_2d(visible_rect);
	
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
		>= (s32)the_sfml_canvas_widget->the_sublevel->real_size_2d.x )
	{
		visible_block_grid_size_2d.x 
			= the_sfml_canvas_widget->the_sublevel->real_size_2d.x
			- visible_block_grid_start_pos.x;
	}
	if ( ( visible_block_grid_start_pos.y 
		+ visible_block_grid_size_2d.y )
		>= (s32)the_sfml_canvas_widget->the_sublevel->real_size_2d.y )
	{
		visible_block_grid_size_2d.y 
			= the_sfml_canvas_widget->the_sublevel->real_size_2d.y
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


