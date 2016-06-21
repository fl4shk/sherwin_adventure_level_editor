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
	else if ( event->key() == Qt::Key_S )
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
	//		finalize_movement_of_rs_contents( the_core_widget,
	//			the_rect_selection_stuff );
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
	//		finalize_movement_of_rs_contents( the_core_widget,
	//			the_rect_selection_stuff );
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
		
		copy_rs_contents(the_rect_selection_stuff);
	}
	else if ( event->key() == Qt::Key_V 
		&& the_core_widget->the_mouse_mode == mm_rect_selection )
	{
		paste_copied_rs_contents( the_core_widget,
			the_rect_selection_stuff );
	}
	else if ( event->key() == Qt::Key_Z )
	{
		bool undo_result = undo(the_core_widget);
		
		cout << "undo_result:  " << undo_result << endl;
	}
	else if ( event->key() == Qt::Key_Y )
	{
		bool redo_result = redo(the_core_widget);
		
		cout << "redo_result:  " << redo_result << endl;
	}
	
	
}


void editing_manager::mouse_press_event
	( level_editor_core_widget* the_core_widget, QMouseEvent* event )
{
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget 
		= the_core_widget->the_sfml_canvas_widget.get(); 
	QTabWidget* level_element_selectors_tab_widget 
		= the_core_widget->level_element_selectors_tab_widget; 
	
	block_selector_widget* the_block_selector_widget 
		= the_core_widget->the_block_selector_widget; 
	sprite_16x16_selector_widget* the_sprite_16x16_selector_widget 
		= the_core_widget->the_sprite_16x16_selector_widget; 
	sprite_16x32_selector_widget* the_sprite_16x32_selector_widget 
		= the_core_widget->the_sprite_16x32_selector_widget; 
	
	adj_sprite_ipgws_ptr_group_for_selecting_sprite& 
		the_sprite_selection_ptr_group 
		= the_core_widget->the_sprite_selection_ptr_group; 
	
	mouse_mode& the_mouse_mode = the_core_widget->the_mouse_mode; 
	vec2_s32& block_grid_coords_of_prev_mouse_pos = the_core_widget 
		->block_grid_coords_of_prev_mouse_pos; 
	
	
	sf::Vector2i mouse_pos_in_canvas_widget_coords; 
	sf::Vector2f mouse_pos_in_canvas_coords; 
	vec2_s32 block_grid_coords_of_mouse_pos; 
	
	get_a_few_types_of_mouse_pos( the_sfml_canvas_widget, 
		mouse_pos_in_canvas_widget_coords, mouse_pos_in_canvas_coords, 
		block_grid_coords_of_mouse_pos ); 
	
	rect_selection_stuff& the_rect_selection_stuff 
		= the_sfml_canvas_widget->the_rect_selection_stuff;
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	sublevel* the_sublevel = the_sfml_canvas_widget->the_sublevel;
	
	if ( !the_sublevel->contains_block_grid_pos
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
	
	
	//block& the_block_at_mouse_pos = the_sublevel
	//	->get_block_at_block_grid_pos(block_grid_coords_of_mouse_pos);
	
	//sprite_ipgws& the_sprite_ipgws_at_mouse_pos = the_sublevel
	//	->get_sprite_ipgws_at_block_grid_pos
	//	(block_grid_coords_of_mouse_pos);
	
	
	bool current_tabbed_widget_is_for_blocks 
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_block_selector_widget );
	bool current_tabbed_widget_is_for_16x16_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x16_selector_widget );
	bool current_tabbed_widget_is_for_16x32_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x32_selector_widget );
	
	
	
	if ( ( event->buttons() & Qt::LeftButton ) 
		&& !left_mouse_button_down )
	{
		left_mouse_button_down = true;
		
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
				if ( the_rect_selection_stuff.get_single_sprite_selected()
					&& the_rect_selection_stuff.get_enabled()
					&& the_rect_selection_stuff.get_mouse_released() )
				{
					finalize_movement_of_rs_contents
						( the_core_widget, the_rect_selection_stuff );
				}
				
				//cout << "st_default\n";
				the_core_widget->do_emit_sprite_no_longer_selected();
			}
		}
		// I am not sure this will ever be the case, mainly because mouse
		// pressing is I think always(?) inside the_core_widget.
		else 
		{
			if ( the_rect_selection_stuff.get_single_sprite_selected()
				&& the_rect_selection_stuff.get_enabled()
				&& the_rect_selection_stuff.get_mouse_released() )
			{
				finalize_movement_of_rs_contents( the_core_widget, 
					the_rect_selection_stuff );
			}
			//cout << "else\n";
			the_core_widget->do_emit_sprite_no_longer_selected();
		}
		
		//cout << block_grid_coords_of_mouse_pos.x << ", "
		//	<< block_grid_coords_of_mouse_pos.y << endl;
		
		switch (the_mouse_mode)
		{
			case mm_place_level_elements:
				handle_placing_le_during_mouse_press( the_core_widget,
					block_grid_coords_of_mouse_pos,
					current_tabbed_widget_is_for_blocks,
					current_tabbed_widget_is_for_16x16_sprites,
					current_tabbed_widget_is_for_16x32_sprites );
				break;
			
			case mm_erase_sprites:
				handle_erasing_sprites_during_mouse_press( the_core_widget,
					block_grid_coords_of_mouse_pos );
				break;
			
			case mm_select_single_sprite:
				handle_selecting_single_sprite_during_mouse_press
					(the_core_widget);
				break;
			
			case mm_rect_selection:
				handle_rs_during_mouse_press( the_core_widget,
					block_grid_coords_of_mouse_pos,
					current_tabbed_widget_is_for_blocks,
					current_tabbed_widget_is_for_16x16_sprites,
					current_tabbed_widget_is_for_16x32_sprites );
				break;
			
			default:
				cout << "Darn it.  I don't know what enum value "
					<< "the_mouse_mode is supposed to represent right "
					<< "now.  Note to the programmer:  there is a bug of "
					<< "some sort!\n";
				break;
		}
		
		//prev_mouse_pos = event->pos();
		block_grid_coords_of_prev_mouse_pos 
			= block_grid_coords_of_mouse_pos;
		
	}
	if ( ( event->buttons() & Qt::RightButton ) 
		&& !right_mouse_button_down )
	{
		right_mouse_button_down = true;
		
		////emit right_mouse_button_pressed();
		//the_core_widget->do_emit_right_mouse_button_pressed();
		//cout << "Right mouse button pressed\n";
	}
	
}



void editing_manager::mouse_move_event
	( level_editor_core_widget* the_core_widget, QMouseEvent* event )
{
	//cout << "mouseMoveEvent()\n";
	
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget 
		= the_core_widget->the_sfml_canvas_widget.get(); 
	QTabWidget* level_element_selectors_tab_widget 
		= the_core_widget->level_element_selectors_tab_widget; 
	
	block_selector_widget* the_block_selector_widget 
		= the_core_widget->the_block_selector_widget; 
	
	//adj_sprite_ipgws_ptr_group_for_selecting_sprite& 
	//	the_sprite_selection_ptr_group 
	//	= the_core_widget->the_sprite_selection_ptr_group; 
	
	mouse_mode& the_mouse_mode = the_core_widget->the_mouse_mode; 
	vec2_s32& block_grid_coords_of_prev_mouse_pos = the_core_widget 
		->block_grid_coords_of_prev_mouse_pos; 
	
	
	sf::Vector2i mouse_pos_in_canvas_widget_coords; 
	sf::Vector2f mouse_pos_in_canvas_coords; 
	vec2_s32 block_grid_coords_of_mouse_pos; 
	
	get_a_few_types_of_mouse_pos( the_sfml_canvas_widget, 
		mouse_pos_in_canvas_widget_coords, mouse_pos_in_canvas_coords, 
		block_grid_coords_of_mouse_pos ); 
	
	
	
	bool current_tabbed_widget_is_for_blocks 
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_block_selector_widget );
	
	
	
	//the_sprite_selection_ptr_group 
	//	= adj_sprite_ipgws_ptr_group_for_selecting_sprite
	//	( *the_core_widget->the_sublevel, 
	//	block_grid_coords_of_mouse_pos.x, 
	//	block_grid_coords_of_mouse_pos.y );
	
	
	//auto func_for_placing_level_elements = [&]() -> void
	//{
	//};
	//
	//auto func_for_erasing_spritess = [&]() -> void
	//{
	//};
	//
	//auto func_for_selecting_single_sprite = [&]() -> void
	//{
	//};
	//
	//auto func_for_rect_selection = [&]() -> void
	//{
	//};
	
	if ( left_mouse_button_down )
	{
		//cout << block_grid_coords_of_mouse_pos.x << ", "
		//	<< block_grid_coords_of_mouse_pos.y << endl;
		
		switch (the_mouse_mode)
		{
			case mm_place_level_elements:
				handle_placing_le_during_mouse_move( the_core_widget,
					block_grid_coords_of_mouse_pos,
					current_tabbed_widget_is_for_blocks );
				break;
			
			case mm_erase_sprites:
				handle_erasing_sprites_during_mouse_move( the_core_widget,
					block_grid_coords_of_mouse_pos );
				break;
			
			case mm_select_single_sprite:
				handle_selecting_single_sprite_during_mouse_move
					(the_core_widget);
				break;
			
			case mm_rect_selection:
				handle_rs_during_mouse_move( the_core_widget,
					block_grid_coords_of_mouse_pos );
				break;
			
			default:
				cout << "Darn it.  I don't know what enum value "
					<< "the_mouse_mode is supposed to represent right "
					<< "now.  Note to the programmer:  there is a bug of "
					<< "some sort!\n";
				break;
		}
		
		//prev_mouse_pos = event->pos();
		block_grid_coords_of_prev_mouse_pos 
			= block_grid_coords_of_mouse_pos;
	}
	if ( right_mouse_button_down )
	{
		//cout << "Right mouse button held while mouse moves\n";
		//emit right_mouse_button_pressed();
	}
	
	
}

void editing_manager::mouse_release_event
	( level_editor_core_widget* the_core_widget, QMouseEvent* event )
{
	//cout << "mouseReleaseEvent()\n";
	
	//get_le_core_widget_stuff(the_core_widget);
	
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget
		= the_core_widget->the_sfml_canvas_widget.get();
	
	mouse_mode& the_mouse_mode = the_core_widget->the_mouse_mode;
	vec2_s32& block_grid_coords_of_prev_mouse_pos = the_core_widget
		->block_grid_coords_of_prev_mouse_pos;
	
	sf::Vector2i mouse_pos_in_canvas_widget_coords;
	sf::Vector2f mouse_pos_in_canvas_coords;
	vec2_s32 block_grid_coords_of_mouse_pos;
	
	get_a_few_types_of_mouse_pos( the_sfml_canvas_widget,
		mouse_pos_in_canvas_widget_coords, mouse_pos_in_canvas_coords,
		block_grid_coords_of_mouse_pos );
	
	
	
	//auto func_for_placing_level_elements = [&]() -> void
	//{
	//};
	//
	//auto func_for_erasing_spritess = [&]() -> void
	//{
	//};
	//
	//auto func_for_selecting_single_sprite = [&]() -> void
	//{
	//};
	//
	//auto func_for_rect_selection = [&]() -> void
	//{
	//};
	
	//cout << block_grid_coords_of_mouse_pos.x << ", "
	//	<< block_grid_coords_of_mouse_pos.y << endl;
	
	bool do_update_ur_stuff = true;
	
	if ( left_mouse_button_down )
	{
		left_mouse_button_down = false;
		switch (the_mouse_mode)
		{
			case mm_place_level_elements:
				handle_placing_le_during_mouse_release
					(the_core_widget);
				break;
			
			case mm_erase_sprites:
				handle_erasing_sprites_during_mouse_release
					(the_core_widget);
				break;
			
			case mm_select_single_sprite:
				handle_selecting_single_sprite_during_mouse_release
					(the_core_widget);
				break;
			
			case mm_rect_selection:
				handle_rs_during_mouse_release
					(the_core_widget);
				break;
			
			default:
				do_update_ur_stuff = false;
				
				cout << "Darn it.  I don't know what enum value "
					<< "the_mouse_mode is supposed to represent right "
					<< "now.  Note to the programmer:  there is a bug of "
					<< "some sort!\n";
				break;
		}
		
		
		//prev_mouse_pos = event->pos();
		block_grid_coords_of_prev_mouse_pos 
			= block_grid_coords_of_mouse_pos;
		
		
		if ( do_update_ur_stuff )
		{
			undo_and_redo_stuff& ur_stuff = get_or_create_ur_stuff
				(the_core_widget);
			
			// Clear ur_action_to_push
			ur_stuff.ur_action_to_push = undo_and_redo_action();
		}
	}
	if ( right_mouse_button_down )
	{
		right_mouse_button_down = false;
		
		//cout << "Right mouse button released\n";
	}
}



void editing_manager::finalize_movement_of_rs_contents
	( level_editor_core_widget* the_core_widget,
	rect_selection_stuff& the_rect_selection_stuff )
{
	//sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	const sf::IntRect& selection_rect 
		= the_rect_selection_stuff.selection_rect;
	sf::IntRect&& selection_rect_before_moving 
		= the_rect_selection_stuff.get_selection_rect_before_moving();
	
	rect_selection_layer selection_layer 
		= the_rect_selection_stuff.get_selection_layer();
	rect_selection_layer original_layer_of_pasted_selection
		= the_rect_selection_stuff
		.get_original_layer_of_pasted_selection();
	bool single_sprite_selected 
		= the_rect_selection_stuff.get_single_sprite_selected();
	bool selection_was_pasted 
		= the_rect_selection_stuff.get_selection_was_pasted();
	
	
	the_rect_selection_stuff.disable_selection();
	
	
	if ( single_sprite_selected )
	{
		the_core_widget->do_emit_sprite_no_longer_selected();
		return;
	}
	
	// If a selection was not pasted, but not moved, don't update the undo
	// and redo stuff.
	if ( selection_rect == selection_rect_before_moving 
		&& !selection_was_pasted )
	{
		cout << "returning when single_sprite_selected == false\n";
		return;
	}
	
	bool do_update_ur_stuff = true;
	
	undo_and_redo_stuff& ur_stuff
		= get_or_create_ur_stuff(the_core_widget);
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	
	auto show_old_block_umap = [&]() -> void
	{
		for ( auto obum_iter : ur_action_to_push.old_block_umap )
		{
			cout << obum_iter.first.x << ", " << obum_iter.first.y 
				<< ";  " << block_stuff::get_bt_name_debug
				((block_type)obum_iter.second.type) << endl;
		}
	};
	
	auto show_replaced_block_umap = [&]() -> void
	{
		for ( auto rbum_iter : ur_action_to_push.replaced_block_umap )
		{
			cout << rbum_iter.first.x << ", " << rbum_iter.first.y 
				<< ";  " << block_stuff::get_bt_name_debug
				((block_type)rbum_iter.second.type) << endl;
		}
	};
	
	auto show_new_block_umap = [&]() -> void
	{
		for ( auto nbum_iter : ur_action_to_push.new_block_umap )
		{
			cout << nbum_iter.first.x << ", " << nbum_iter.first.y 
				<< ";  " << block_stuff::get_bt_name_debug
				((block_type)nbum_iter.second.type) << endl;
		}
	};
	
	
	
	auto show_old_sprite_ipgws_umap = [&]() -> void
	{
		for ( auto iter : ur_action_to_push.old_sprite_ipgws_umap )
		{
			cout << iter.first.x << ", " << iter.first.y << ";  " 
				<< sprite_type_helper::get_st_name_debug
				((sprite_type)iter.second.type) << endl;
		}
	};
	
	auto show_replaced_sprite_ipgws_umap = [&]() -> void
	{
		for ( auto iter : ur_action_to_push.replaced_sprite_ipgws_umap )
		{
			cout << iter.first.x << ", " << iter.first.y << ";  " 
				<< sprite_type_helper::get_st_name_debug 
				((sprite_type)iter.second.type) << endl;
		}
	};
	
	auto show_new_sprite_ipgws_umap = [&]() -> void
	{
		for ( auto iter : ur_action_to_push.new_sprite_ipgws_umap )
		{
			cout << iter.first.x << ", " << iter.first.y << ";  " 
				<< sprite_type_helper::get_st_name_debug
				((sprite_type)iter.second.type) << endl;
		}
	};
	
	
	// Blocks were moved, but not pasted
	if ( selection_layer == rsl_blocks && !selection_was_pasted )
	{
		finalize_movement_of_rs_contents_block_ur_stuff
			( the_core_widget, the_rect_selection_stuff, selection_rect, 
			selection_rect_before_moving, false );
		
		//cout << "Blocks in a not-pasted rect selection before moving:  "
		//	<< "\n";
		//show_old_block_umap();
		//
		//cout << "\nBlocks that were replaced by a moved but not pasted "
		//	<< "rect selection:  \n";
		//show_replaced_block_umap();
		//
		//cout << "\nBlocks in a rect selection that were moved but not "
		//	<< "pasted:  \n";
		//show_new_block_umap();
		//
		//cout << "\n\n";
	}
	// Blocks were pasted
	else if ( original_layer_of_pasted_selection == rsl_blocks 
		&& selection_was_pasted )
	{
		finalize_movement_of_rs_contents_block_ur_stuff
			( the_core_widget, the_rect_selection_stuff, selection_rect, 
			selection_rect_before_moving, true );
		
		
		//cout << "Blocks that were replaced by a pasted rect selection:  "
		//	<< endl;
		//show_replaced_block_umap();
		//
		//cout << "\nBlocks in a rect selection that were pasted:  \n";
		//show_new_block_umap();
		//
		//cout << "\n\n";
	}
	// Sprites were moved, but not pasted
	else if ( selection_layer == rsl_sprites && !selection_was_pasted )
	{
		finalize_movement_of_rs_contents_sprite_ur_stuff
			( the_core_widget, the_rect_selection_stuff, selection_rect, 
			selection_rect_before_moving, false );
		
		
		//cout << "Sprites in a not-pasted rect selection before moving:  "
		//	<< "\n";
		//show_old_sprite_ipgws_umap();
		//
		//cout << "\nSprites that were replaced by a moved but not pasted "
		//	<< "rect selection:  \n";
		//show_replaced_sprite_ipgws_umap();
		//
		//cout << "\nSprites in a rect selection that were moved but not "
		//	<< "pasted:  \n";
		//show_new_sprite_ipgws_umap();
		//
		//cout << "\n\n";
	}
	// Sprites were pasted
	else if ( original_layer_of_pasted_selection == rsl_sprites 
		&& selection_was_pasted )
	{
		finalize_movement_of_rs_contents_sprite_ur_stuff
			( the_core_widget, the_rect_selection_stuff, selection_rect, 
			selection_rect_before_moving, true );
		
		
		//cout << "Sprites that were replaced by a pasted rect selection:  "
		//	<< endl;
		//show_replaced_sprite_ipgws_umap();
		//
		//cout << "\nSprites in a rect selection that were pasted:  \n";
		//show_new_sprite_ipgws_umap();
		//
		//cout << "\n\n";
	}
	else
	{
		cout << "Strange issue in editing_manager"
			<< "::finalize_movement_of_rs_contents()\n";
		
		cout << "selection_layer:  " << selection_layer << ";  "
			<< "original_layer_of_pasted_selection:  " 
			<< original_layer_of_pasted_selection << ";  "
			<< "selection_was_pasted:  " << selection_was_pasted << endl;
		
		do_update_ur_stuff = false;
	}
	
	if ( do_update_ur_stuff )
	{
		ur_stuff.ur_stack.add_action(ur_action_to_push);
		
		ur_action_to_push = undo_and_redo_action();
	}
	
	the_rect_selection_stuff.finalize_rs_movement();
}

void editing_manager::finalize_sprite_properties_modification
	( level_editor_core_widget* the_core_widget,
	const sprite_pw_extras& the_sprite_pw_extras,
	const sprite_ipgws& the_new_sprite_ipgws )
{
	if ( !the_sprite_pw_extras.enabled )
	{
		return;
	}
	
	if ( the_sprite_pw_extras.backed_up_selected_sprite_ipgws
		== the_new_sprite_ipgws )
	{
		cout << "no change in the sprite_ipgws\n";
		return;
	}
	
	undo_and_redo_stuff& ur_stuff 
		= get_or_create_ur_stuff(the_core_widget);
	
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	
	ur_action_to_push.the_action_type = at_modify_sprite;
	ur_action_to_push.old_sprite_ipgws 
		= the_sprite_pw_extras.backed_up_selected_sprite_ipgws;
	ur_action_to_push.new_sprite_ipgws = the_new_sprite_ipgws;
	
	
	//auto show_sprite_stuff = [&]( const sprite_ipgws& the_sprite_ipgws )
	//	-> void
	//{
	//	cout << the_sprite_ipgws.facing_right << "; "
	//		<< the_sprite_ipgws.extra_param_0 << ", "
	//		<< the_sprite_ipgws.extra_param_1 << ", "
	//		<< the_sprite_ipgws.extra_param_2 << ", "
	//		<< the_sprite_ipgws.extra_param_3 << endl;
	//};
	//
	//cout << "ur_action_to_push.old_sprite_ipgws stuff:  \n";
	//show_sprite_stuff(ur_action_to_push.old_sprite_ipgws);
	//
	//cout << "ur_action_to_push.new_sprite_ipgws stuff:  \n";
	//show_sprite_stuff(ur_action_to_push.new_sprite_ipgws);
	//
	//cout << "\n\n";
	
	
	ur_stuff.ur_stack.add_action(ur_action_to_push);
	
	ur_action_to_push = undo_and_redo_action();
}

void editing_manager::record_sublevel_properties_modification_ur_stuff
	( level_editor_core_widget* the_core_widget,
	const sublevel& the_sublevel_before_modification )
{
	sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	undo_and_redo_stuff& ur_stuff 
		= get_or_create_ur_stuff(the_core_widget);
	
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	const vec2_u32& temp_old_real_size_2d 
		= the_sublevel_before_modification.real_size_2d;
	const vec2_u32& temp_new_real_size_2d = the_sublevel->real_size_2d;
	
	vec2_u32& old_real_size_2d = ur_action_to_push.old_real_size_2d;
	vec2_u32& new_real_size_2d = ur_action_to_push.new_real_size_2d;
	
	
	// Double protection against adding ur_stuff.undo_and_redo_action to
	// ur_stuff.ur_stack
	if ( temp_old_real_size_2d == temp_new_real_size_2d )
	{
		return;
	}
	
	
	ur_action_to_push.the_action_type = at_resize_sublevel;
	
	old_real_size_2d = temp_old_real_size_2d;
	new_real_size_2d = temp_new_real_size_2d;
	
	auto show_width_stuff = [&]() -> void
	{
		cout << "old width == " << old_real_size_2d.x << ";  "
			<< "new width == " << new_real_size_2d.x << ";  "
			<< "old width - new width == " 
			<< ( old_real_size_2d.x - new_real_size_2d.x )
			<< "\n";
	};
	
	auto show_height_stuff = [&]() -> void
	{
		cout << "old height == " << old_real_size_2d.y << ";  "
			<< "new height == " << new_real_size_2d.y << ";  "
			<< "old height - new height == " 
			<< ( old_real_size_2d.y - new_real_size_2d.y )
			<< "\n";
	};
	
	
	
	// 
	auto record_removed_les_shared_code = [&]( u32 i, u32 j ) -> void
	{
		//cout << "i, j == " << i << ", " << j << endl;
		
		vec2_s32 block_grid_pos( (s32)i, (s32)j );
		
		const block& the_old_block 
			= the_sublevel_before_modification
			.uncompressed_block_data_vec_2d.at(j).at(i);
		const sprite_ipgws& the_old_sprite_ipgws
			= the_sublevel_before_modification.sprite_ipgws_vec_2d
			.at(j).at(i);
		
		size_t num_copies_of_the_old_block = ur_action_to_push
			.old_block_umap.count(block_grid_pos);
		size_t num_copies_of_the_old_sprite_ipgws = ur_action_to_push
			.old_sprite_ipgws_umap.count(block_grid_pos);
		
		//cout << "num_copies_of_the_old_block == "
		//	<< num_copies_of_the_old_block << ";  "
		//	<< "num_copies_of_the_old_sprite_ipgws == "
		//	<< num_copies_of_the_old_sprite_ipgws << endl;
		
		// Block stuff
		if ( num_copies_of_the_old_block == 0 )
		{
			ur_action_to_push.old_block_umap[block_grid_pos]
				= the_old_block;
		}
		
		if ( num_copies_of_the_old_sprite_ipgws == 0 
			&& the_old_sprite_ipgws.type != st_default )
		{
			ur_action_to_push.old_sprite_ipgws_umap[block_grid_pos]
				= the_old_sprite_ipgws;
		}
		
		//cout << endl;
	};
	
	auto record_removed_les_width_stuff = [&]() -> void
	{
		for ( u32 i=new_real_size_2d.x; i<old_real_size_2d.x; ++i )
		{
			//cout << "i==" << i << endl;
			//cout << "j==";
			for ( u32 j=0; j<old_real_size_2d.x; ++j )
			{
				//ur_action_to_push.old_block_umap
				//cout << j << ", ";
				
				record_removed_les_shared_code( i, j );
			}
			//cout << endl;
		}
		//cout << "\n\n";
	};
	
	auto record_removed_les_height_stuff = [&]() -> void
	{
		for ( u32 j=new_real_size_2d.y; j<old_real_size_2d.y; ++j )
		{
			//cout << "j==" << j << endl;
			//cout << "i==";
			for ( u32 i=0; i<old_real_size_2d.x; ++i )
			{
				//ur_action_to_push.old_block_umap
				//cout << i << ", ";
				
				record_removed_les_shared_code( i, j );
			}
			//cout << endl;
		}
		//cout << "\n\n";
	};
	
	// Larger width
	if ( new_real_size_2d.x > old_real_size_2d.x )
	{
		// Larger height
		if ( new_real_size_2d.y > old_real_size_2d.y )
		{
			cout << "new width > old width and new height > old height\n";
		}
		// Smaller height
		else if ( new_real_size_2d.y < old_real_size_2d.y )
		{
			cout << "new width > old width and new height < old height\n";
			
			show_height_stuff();
			cout << endl;
			
			
			// Record the removed level elements
			record_removed_les_height_stuff();
		}
		// Same height
		else // if ( new_real_size_2d.y == old_real_size_2d.y )
		{
			cout << "new width > old width and new height == old height\n";
		}
	}
	// Smaller width
	else if ( new_real_size_2d.x < old_real_size_2d.x )
	{
		// Larger height
		if ( new_real_size_2d.y > old_real_size_2d.y )
		{
			cout << "new width < old width and new height > old height\n";
			
			show_width_stuff();
			cout << endl;
			
			
			// Record the removed level elements
			record_removed_les_width_stuff();
		}
		// Smaller height
		else if ( new_real_size_2d.y < old_real_size_2d.y )
		{
			cout << "new width < old width and new height < old height\n";
			
			show_width_stuff();
			show_height_stuff();
			cout << endl;
			
			
			// Record the removed level elements
			record_removed_les_width_stuff();
			record_removed_les_height_stuff();
		}
		// Same height
		else // if ( new_real_size_2d.y == old_real_size_2d.y )
		{
			cout << "new width < old width and new height == old height\n";
			
			show_width_stuff();
			cout << endl;
			
			
			// Record the removed level elements
			record_removed_les_width_stuff();
		}
	}
	// Same width
	else // if ( new_real_size_2d.x == old_real_size_2d.x )
	{
		// Larger height
		if ( new_real_size_2d.y > old_real_size_2d.y )
		{
			cout << "new width == old width and new height > old height\n";
		}
		// Smaller height
		else if ( new_real_size_2d.y < old_real_size_2d.y )
		{
			cout << "new width == old width and new height < old height\n";
			
			show_height_stuff();
			cout << endl;
			
			
			// Record the removed level elements
			record_removed_les_height_stuff();
		}
		// Same height
		else // if ( new_real_size_2d.y == old_real_size_2d.y )
		{
			cout << "Well, this shouldn't be happening.\n";
		}
	}
	
	
	ur_stuff.ur_stack.add_action(ur_action_to_push);
	ur_action_to_push = undo_and_redo_action();
}



undo_and_redo_stuff& editing_manager::get_or_create_ur_stuff
	( level_editor_core_widget* the_core_widget )
{
	return get_or_create_umap_value( ur_stuff_umap, the_core_widget );
}



void editing_manager::handle_placing_le_during_mouse_press
	( level_editor_core_widget* the_core_widget,
	const vec2_s32& block_grid_coords_of_mouse_pos,
	bool current_tabbed_widget_is_for_blocks,
	bool current_tabbed_widget_is_for_16x16_sprites,
	bool current_tabbed_widget_is_for_16x32_sprites )
{
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget 
		= the_core_widget->the_sfml_canvas_widget.get();
	rect_selection_stuff& the_rect_selection_stuff 
		= the_sfml_canvas_widget->the_rect_selection_stuff;
	
	sublevel* the_sublevel = the_sfml_canvas_widget->the_sublevel;
	
	block& the_block_at_mouse_pos = the_sublevel
		->get_block_at_block_grid_pos(block_grid_coords_of_mouse_pos);
	
	//sprite_ipgws& the_sprite_ipgws_at_mouse_pos = the_sublevel
	//	->get_sprite_ipgws_at_block_grid_pos
	//	(block_grid_coords_of_mouse_pos);
	
	if ( the_rect_selection_stuff.get_enabled() 
		&& the_rect_selection_stuff.get_mouse_released() )
	{
		finalize_movement_of_rs_contents( the_core_widget, 
			the_rect_selection_stuff );
	}
	
	the_sfml_canvas_widget->modified_recently = true;
	
	
	undo_and_redo_stuff& ur_stuff = get_or_create_ur_stuff
		(the_core_widget);
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	if (current_tabbed_widget_is_for_blocks)
	{
		//cout << "the_block_selector_widget_is_enabled!\n";
		
		//ur_action_to_push = undo_and_redo_action();
		ur_action_to_push.the_action_type = at_place_blocks;
		
		place_single_block_and_record_ur_stuff( the_block_at_mouse_pos, 
			block_grid_coords_of_mouse_pos, 
			get_left_selected_block_type(the_core_widget),
			ur_action_to_push );
	}
	
	else if (current_tabbed_widget_is_for_16x16_sprites)
	{
		ur_action_to_push.the_action_type = at_place_sprite;
		
		//cout << "the_sprite_16x16_selector_widget_is_enabled!\n";
		
		place_single_16x16_sprite_and_record_ur_stuff
			( the_sublevel, block_grid_coords_of_mouse_pos,
			get_left_selected_16x16_sprite_type(the_core_widget),
			ur_action_to_push );
	}
	
	else if (current_tabbed_widget_is_for_16x32_sprites)
	{
		ur_action_to_push.the_action_type = at_place_sprite;
		
		//cout << "the_sprite_16x32_selector_widget_is_enabled!\n";
		
		place_single_16x32_sprite_and_record_ur_stuff
			( the_sublevel, block_grid_coords_of_mouse_pos,
			get_left_selected_16x32_sprite_type(the_core_widget),
			ur_action_to_push );
	}
	
	//cout << endl;
	
}

void editing_manager::handle_erasing_sprites_during_mouse_press
	( level_editor_core_widget* the_core_widget,
	const vec2_s32& block_grid_coords_of_mouse_pos )
{
	
}

void editing_manager::handle_selecting_single_sprite_during_mouse_press
	( level_editor_core_widget* the_core_widget )
{
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget 
		= the_core_widget->the_sfml_canvas_widget.get();
	rect_selection_stuff& the_rect_selection_stuff 
		= the_sfml_canvas_widget->the_rect_selection_stuff;
	
	
	adj_sprite_ipgws_ptr_group_for_selecting_sprite&
		the_sprite_selection_ptr_group = the_core_widget
		->the_sprite_selection_ptr_group;
	
	the_core_widget->do_emit_sprite_no_longer_selected();
	
	if ( the_sprite_selection_ptr_group.origin_ptr != NULL )
	{
		if ( the_sprite_selection_ptr_group.origin_ptr->type 
			== st_default )
		{
			if ( the_rect_selection_stuff.get_enabled()
				&& the_rect_selection_stuff.get_mouse_released() )
			{
				finalize_movement_of_rs_contents
					( the_core_widget, the_rect_selection_stuff );
			}
			
			//cout << "st_default\n";
			
			return;
		}
	}
	// I am not sure this will ever be the case, mainly because the calling
	// function only activates when the mouse is clicked inside of the
	// level_editor_core_widget
	else 
	{
		//the_sfml_canvas_widget->disable_rect_selection();
		
		if ( the_rect_selection_stuff .get_enabled()
			&& the_rect_selection_stuff.get_mouse_released() )
		{
			finalize_movement_of_rs_contents( the_core_widget, 
				the_rect_selection_stuff );
		}
		
		
		//cout << "else\n";
		
		//emit sprite_no_longer_selected();
		//the_core_widget->do_emit_sprite_no_longer_selected();
		return;
	}
	
	sprite_ipgws* clicked_sprite_ipgws 
		= the_sprite_selection_ptr_group.origin_ptr;
	
	the_rect_selection_stuff.enable_single_sprite_selection
		(clicked_sprite_ipgws);
	
	//emit sprite_was_selected();
	the_core_widget->do_emit_sprite_was_selected();
	
}

void editing_manager::handle_rs_during_mouse_press
	( level_editor_core_widget* the_core_widget,
	const vec2_s32& block_grid_coords_of_mouse_pos,
	bool current_tabbed_widget_is_for_blocks,
	bool current_tabbed_widget_is_for_16x16_sprites,
	bool current_tabbed_widget_is_for_16x32_sprites )
{
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget 
		= the_core_widget->the_sfml_canvas_widget.get();
	rect_selection_stuff& the_rect_selection_stuff = the_sfml_canvas_widget
		->the_rect_selection_stuff;
	
	bool clicked_location_intersects_rect 
		= the_rect_selection_stuff.get_selection_rect().contains
		( block_grid_coords_of_mouse_pos.x, 
		block_grid_coords_of_mouse_pos.y );
	
	
	//adj_sprite_ipgws_ptr_group_for_selecting_sprite& 
	//	the_sprite_selection_ptr_group 
	//	= the_core_widget->the_sprite_selection_ptr_group; 
	
	//if ( the_sprite_selection_ptr_group.origin_ptr != NULL )
	//{
	//	if ( the_sprite_selection_ptr_group.origin_ptr->type 
	//		== st_default )
	//	{
	//		if ( the_rect_selection_stuff.get_single_sprite_selected()
	//			&& the_rect_selection_stuff.get_enabled()
	//			&& the_rect_selection_stuff.get_mouse_released() )
	//		{
	//			finalize_movement_of_rs_contents
	//				( the_core_widget, the_rect_selection_stuff );
	//		}
	//		
	//		//cout << "st_default\n";
	//		the_core_widget->do_emit_sprite_no_longer_selected();
	//	}
	//}
	
	
	// If the rect selection is currently not enabled, or if the rect
	// selection is enabled but the clicked position is outside the
	// rect selection
	if ( !the_rect_selection_stuff.get_enabled() 
		|| ( the_rect_selection_stuff.get_enabled() 
		&& !clicked_location_intersects_rect ) )
	{
		// If the rectangular selection is enabled and
		// the_rect_selection_stuff isn't aware of the mouse being
		// clicked (which it shouldn't be I think?)
		if ( the_rect_selection_stuff.get_enabled()
			&& the_rect_selection_stuff.get_mouse_released() )
		{
			finalize_movement_of_rs_contents
				( the_core_widget, the_rect_selection_stuff );
		}
		
		// Create a new rectangular selection
		if (current_tabbed_widget_is_for_blocks)
		{
			the_rect_selection_stuff.start_creating_rs
				( block_grid_coords_of_mouse_pos, rsl_blocks );
		}
		else if ( current_tabbed_widget_is_for_16x16_sprites 
			|| current_tabbed_widget_is_for_16x32_sprites )
		{
			the_rect_selection_stuff.start_creating_rs
				( block_grid_coords_of_mouse_pos, rsl_sprites );
		}
	}
	
	// If the rect selection is enabled and the clicked location is
	// inside the rect selection
	else if ( the_rect_selection_stuff.get_enabled() 
		&& clicked_location_intersects_rect )
	{
		vec2_s32 clicked_location_in_rect 
			= block_grid_coords_of_mouse_pos
			- the_rect_selection_stuff.get_starting_block_grid_coords();
		
		// Prepare for the mouse to be dragged such that the rect
		// selection and its contents can be dragged
		the_rect_selection_stuff.start_moving_rs_contents
			(clicked_location_in_rect);
	}
	
	
	// If the mouse is pressed and the rect selection tool is the
	// active tool, delete the_sprite_properties_widget if it exists.
	
	//emit sprite_no_longer_selected();
	the_core_widget->do_emit_sprite_no_longer_selected();
}


void editing_manager::handle_placing_le_during_mouse_move
	( level_editor_core_widget* the_core_widget,
	const vec2_s32& block_grid_coords_of_mouse_pos,
	bool current_tabbed_widget_is_for_blocks )
{
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget
		= the_core_widget->the_sfml_canvas_widget.get();
	
	sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	vec2_s32& block_grid_coords_of_prev_mouse_pos = the_core_widget
		->block_grid_coords_of_prev_mouse_pos;
	
	the_sfml_canvas_widget->modified_recently = true;
	//cout << "placing level elements\n";
	
	undo_and_redo_stuff& ur_stuff = get_or_create_ur_stuff
		(the_core_widget);
	
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	if (current_tabbed_widget_is_for_blocks)
	{
		//cout << "the_block_selector_widget_is_enabled!\n";
		
		block_type the_block_type = get_left_selected_block_type
			(the_core_widget);
		
		if ( !the_sublevel->contains_block_grid_pos
			(block_grid_coords_of_mouse_pos) )
		{
			//return;
		}
		else
		{
			place_single_block_and_record_ur_stuff( the_sublevel,
				block_grid_coords_of_mouse_pos, the_block_type,
				ur_action_to_push );
		}
		
		// This needs to be recorded for undo and redo stuff
		place_block_line( the_core_widget,
			block_grid_coords_of_prev_mouse_pos,
			block_grid_coords_of_mouse_pos, the_block_type,
			ur_action_to_push );
	}
	
	//cout << endl;
	
}

void editing_manager::handle_erasing_sprites_during_mouse_move
	( level_editor_core_widget* the_core_widget,
	const vec2_s32& block_grid_coords_of_mouse_pos )
{
	
}

void editing_manager::handle_selecting_single_sprite_during_mouse_move
	( level_editor_core_widget* the_core_widget )
{
	
}

void editing_manager::handle_rs_during_mouse_move
	( level_editor_core_widget* the_core_widget,
	const vec2_s32& block_grid_coords_of_mouse_pos )
{
	rect_selection_stuff& the_rect_selection_stuff = the_core_widget
		->the_sfml_canvas_widget->the_rect_selection_stuff;
	
	// If the selection isn't currently moving, i.e., its size is not
	// fixed yet, then continue changing its size.
	
	// I don't think either of these needs to be recorded for
	// undo/redo.
	if ( !the_rect_selection_stuff.get_moving() )
	{
		the_rect_selection_stuff.continue_creating_rs
			(block_grid_coords_of_mouse_pos);
	}
	else //if ( the_rect_selection_stuff.get_moving() )
	{
		the_rect_selection_stuff.continue_moving_rs_contents
			(block_grid_coords_of_mouse_pos);
	}
	
	//emit sprite_no_longer_selected();
	the_core_widget->do_emit_sprite_no_longer_selected();
}




void editing_manager::handle_placing_le_during_mouse_release
	( level_editor_core_widget* the_core_widget )
{
	undo_and_redo_stuff& ur_stuff = get_or_create_ur_stuff
		(the_core_widget);
	//undo_and_redo_stack& ur_stack = ur_stuff.ur_stack;
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	
	//cout << "Done placing level elements\n";
	//cout << "Here's the action_type:  " 
	//	<< ur_action_to_push.the_action_type << endl;
	
	
	auto func_for_placing_blocks = [&]() -> void
	{
		//cout << "func_for_placing_blocks()\n";
		
		//cout << "Here's replaced_block_umap:  \n";
		//
		//for ( auto rbum_iter : ur_action_to_push.replaced_block_umap )
		//{
		//	cout << "block coord:  ( " << rbum_iter.first.x << ", "
		//		<< rbum_iter.first.y << " ); block_type:  " 
		//		<< block_stuff::get_bt_name_debug
		//		((block_type)rbum_iter.second.type) << "\n";
		//}
		//
		//cout << endl;
		
		ur_stuff.ur_stack.add_action(ur_action_to_push);
	};
	
	auto func_for_placing_sprite = [&]() -> void
	{
		//cout << "func_for_placing_sprite()\n";
		
		sprite_ipgws& new_sprite_ipgws
			= ur_action_to_push.new_sprite_ipgws;
		
		if ( new_sprite_ipgws.type <= st_default 
			|| new_sprite_ipgws.type >= st_count )
		{
			return;
		}
		
		
		//cout << sprite_type_helper::get_st_name_debug
		//	((sprite_type)new_sprite_ipgws.type)
		//	<< ", " << new_sprite_ipgws.initial_block_grid_x_coord
		//	<< ", " << new_sprite_ipgws.initial_block_grid_y_coord
		//	<< ", " << new_sprite_ipgws.facing_right 
		//	<< ", " << new_sprite_ipgws.extra_param_0
		//	<< ", " << new_sprite_ipgws.extra_param_1
		//	<< ", " << new_sprite_ipgws.extra_param_2
		//	<< ", " << new_sprite_ipgws.extra_param_3 << endl;
		
		ur_stuff.ur_stack.add_action(ur_action_to_push);
	};
	
	switch ( ur_action_to_push.the_action_type )
	{
		case at_place_blocks:
			func_for_placing_blocks();
			break;
		
		case at_place_sprite:
			func_for_placing_sprite();
			break;
		
		default:
			cout << "Strange action_type.  Perhaps there's a bug?\n";
			break;
	}
}

void editing_manager::handle_erasing_sprites_during_mouse_release
	( level_editor_core_widget* the_core_widget )
{
	
}

void editing_manager::handle_selecting_single_sprite_during_mouse_release
	( level_editor_core_widget* the_core_widget )
{
	
}

void editing_manager::handle_rs_during_mouse_release
	( level_editor_core_widget* the_core_widget )
{
	rect_selection_stuff& the_rect_selection_stuff = the_core_widget
		->the_sfml_canvas_widget->the_rect_selection_stuff;
	
	if ( !the_rect_selection_stuff.get_moving() )
	{
		the_rect_selection_stuff.stop_creating_rs();
	}
	else //if ( the_rect_selection_stuff.get_moving() )
	{
		the_rect_selection_stuff.stop_moving_rs_contents();
	}
	
	// I can't remember why I did this.
	//emit sprite_no_longer_selected();
	the_core_widget->do_emit_sprite_no_longer_selected();
}


void editing_manager::get_a_few_types_of_mouse_pos
	( level_editor_sfml_canvas_widget* the_sfml_canvas_widget,
	sf::Vector2i& ret_mouse_pos_in_canvas_widget_coords,
	sf::Vector2f& ret_mouse_pos_in_canvas_coords, 
	vec2_s32& ret_block_grid_coords_of_mouse_pos )
{
	u32 scale_factor = the_sfml_canvas_widget->scale_factor;
	sublevel* the_sublevel = the_sfml_canvas_widget->the_sublevel;
	
	ret_mouse_pos_in_canvas_widget_coords  
		= sf::Mouse::getPosition(*the_sfml_canvas_widget); 
	
	// This converts the clicked coordinate to pixel coordinates.
	ret_mouse_pos_in_canvas_coords = sf::Vector2f
		( (double)ret_mouse_pos_in_canvas_widget_coords.x 
		/ (double)scale_factor, 
		(double)ret_mouse_pos_in_canvas_widget_coords.y 
		/ (double)scale_factor ); 
	
	ret_block_grid_coords_of_mouse_pos 
		= { (s32)( ret_mouse_pos_in_canvas_coords.x 
		/ ( level_editor_sfml_canvas_widget 
		::num_pixels_per_block_row ) ), 
		
		(s32)( ( the_sublevel->real_size_2d.y 
		- ( ( the_sfml_canvas_widget->getSize().y / scale_factor ) 
		- ret_mouse_pos_in_canvas_coords.y ) 
		/ level_editor_sfml_canvas_widget 
		::num_pixels_per_block_column ) ) }; 
}



// Editing functions to activate upon mouse events
void editing_manager::place_single_block_and_record_ur_stuff
	( block& the_block_in_sublevel, const vec2_s32& block_grid_coord,
	const block_type& the_block_type,
	undo_and_redo_action& ur_action_to_push )
{
	if ( !block_stuff::block_type_exists(the_block_type) )
	{
		return;
	}
	
	auto rbum_iter = ur_action_to_push.replaced_block_umap.find
		(block_grid_coord);
	auto nbum_iter = ur_action_to_push.new_block_umap.find
		(block_grid_coord);
	
	
	// Prevent overwriting the previous data within the same action
	if ( rbum_iter == ur_action_to_push.replaced_block_umap.end() )
	{
		ur_action_to_push.replaced_block_umap[block_grid_coord]
			= the_block_in_sublevel;
	}
	
	if ( nbum_iter == ur_action_to_push.new_block_umap.end() )
	{
		ur_action_to_push.new_block_umap[block_grid_coord] = block();
		ur_action_to_push.new_block_umap[block_grid_coord].type
			= the_block_type;
		
		// Start placing blocks, possibly just one
		the_block_in_sublevel.type = the_block_type;
	}
}


void editing_manager::place_block_line
	( level_editor_core_widget* the_core_widget, 
	const sf::Vector2i& pos_0, const sf::Vector2i& pos_1, 
	block_type the_block_type, undo_and_redo_action& ur_action_to_push )
{
	if ( !block_stuff::block_type_exists(the_block_type) )
	{
		return;
	}
	
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget
		= the_core_widget->the_sfml_canvas_widget.get();
	sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	the_sfml_canvas_widget->modified_recently = true;
	
	sf::Vector2i delta, block_coord, offset;
	
	delta = sf::Vector2i( pos_1.x - pos_0.x, pos_1.y - pos_0.y );
	
	if ( delta.x < 0 )
	{
		delta.x = -delta.x;
	}
	if ( delta.y < 0 )
	{
		delta.y = -delta.y;
	}
	
	block_coord = pos_0;
	
	if ( pos_0.x > pos_1.x )
	{
		offset.x = -1;
	}
	else
	{
		offset.x = 1;
	}
	
	if ( pos_0.y > pos_1.y )
	{
		offset.y = -1;
	}
	else
	{
		offset.y = 1;
	}
	
	//if ( point_is_in_image(pixel_coord) )
	//{
	//	canvas_image->setPixel( pixel_coord.x, pixel_coord.y, 
	//		color );
	//}
	
	if ( the_sublevel->contains_block_grid_pos(block_coord) )
	{
		//the_sublevel->uncompressed_block_data_vec_2d
		//	[block_coord.y][block_coord.x].type = the_block_type;
		place_single_block_and_record_ur_stuff( the_sublevel, 
			vec2_s32( block_coord.x, block_coord.y ), the_block_type, 
			ur_action_to_push );
	}
	
	if ( delta.x > delta.y )
	{
		s32 error = delta.x >> 1;
		
		while ( block_coord.x != pos_1.x )
		{
			error -= delta.y;
			
			if ( error < 0 )
			{
				block_coord.y += offset.y;
				error += delta.x;
			}
			
			block_coord.x += offset.x;
			
			//if ( point_is_in_image(pixel_coord) )
			//{
			//	canvas_image->setPixel( pixel_coord.x, 
			//		pixel_coord.y, color );
			//}
			
			if ( the_sublevel->contains_block_grid_pos(block_coord) )
			{
				//the_sublevel->uncompressed_block_data_vec_2d
				//	[block_coord.y][block_coord.x].type 
				//	= the_block_type;
				place_single_block_and_record_ur_stuff( the_sublevel, 
					vec2_s32( block_coord.x, block_coord.y ), 
					the_block_type, ur_action_to_push );
			}
		}
	}
	else
	{
		s32 error = delta.y >> 1;
		
		while ( block_coord.y != pos_1.y )
		{
			error -= delta.x;
			
			if ( error < 0 )
			{
				block_coord.x += offset.x;
				error += delta.y;
			}
			
			block_coord.y += offset.y;
			
			//if ( point_is_in_image(pixel_coord) )
			//{
			//	canvas_image->setPixel( pixel_coord.x, 
			//		pixel_coord.y, color );
			//}
			
			if ( the_sublevel->contains_block_grid_pos(block_coord) )
			{
				//the_sublevel->uncompressed_block_data_vec_2d
				//	[block_coord.y][block_coord.x].type 
				//	= the_block_type;
				place_single_block_and_record_ur_stuff( the_sublevel, 
					vec2_s32( block_coord.x, block_coord.y ), 
					the_block_type, ur_action_to_push );
			}
		}
	}
	
}

void editing_manager::place_single_16x16_sprite_and_record_ur_stuff
	( sublevel* the_sublevel, const vec2_s32& block_grid_coord, 
	const sprite_type& the_sprite_type,
	undo_and_redo_action& ur_action_to_push )
{
	if ( !sprite_type_helper::sprite_type_exists(the_sprite_type) )
	{
		return;
	}
	
	sprite_ipgws& the_sprite_ipgws_in_sublevel = the_sublevel
		->get_sprite_ipgws_at_block_grid_pos(block_grid_coord);
	
	auto add_sprite_and_record_ur_stuff = [&]() -> void
	{
		
		//the_sprite_ipgws_at_mouse_pos 
		//	= sprite_ipgws();
		
		// Create a new 16x16 sprite
		the_sprite_ipgws_in_sublevel.type = the_sprite_type;
		
		the_sprite_ipgws_in_sublevel.initial_block_grid_x_coord
			= block_grid_coord.x;
		the_sprite_ipgws_in_sublevel.initial_block_grid_y_coord
			= block_grid_coord.y;
		
		the_sprite_ipgws_in_sublevel.size_2d = vec2_u32( 16, 16 );
		
		ur_action_to_push.new_sprite_ipgws = the_sprite_ipgws_in_sublevel;
	};
	
	adj_sprite_ipgws_ptr_group_16x16 the_ptr_group( *the_sublevel, 
		block_grid_coord.x, block_grid_coord.y );
	
	if ( the_ptr_group.can_add_sprite() )
	{
		add_sprite_and_record_ur_stuff();
	}
}

void editing_manager::place_single_16x32_sprite_and_record_ur_stuff
	( sublevel* the_sublevel, const vec2_s32& block_grid_coord, 
	const sprite_type& the_sprite_type,
	undo_and_redo_action& ur_action_to_push )
{
	if ( !sprite_type_helper::sprite_type_exists(the_sprite_type) )
	{
		return;
	}
	
	sprite_ipgws& the_sprite_ipgws_in_sublevel = the_sublevel
		->get_sprite_ipgws_at_block_grid_pos(block_grid_coord);
	
	auto add_sprite_and_record_ur_stuff = [&]() -> void
	{
		the_sprite_ipgws_in_sublevel.type = the_sprite_type;
		
		the_sprite_ipgws_in_sublevel.initial_block_grid_x_coord
			= block_grid_coord.x;
		the_sprite_ipgws_in_sublevel.initial_block_grid_y_coord
			= block_grid_coord.y;
		
		the_sprite_ipgws_in_sublevel.size_2d = vec2_u32( 16, 32 );
		
		the_sprite_ipgws_in_sublevel.extra_param_0 = 0;
		the_sprite_ipgws_in_sublevel.extra_param_1 = 0;
		the_sprite_ipgws_in_sublevel.extra_param_2 = 0;
		the_sprite_ipgws_in_sublevel.extra_param_3 = 0;
		
		the_sprite_ipgws_in_sublevel.spawn_state = sss_not_active;
		
		ur_action_to_push.new_sprite_ipgws = the_sprite_ipgws_in_sublevel;
	};
	
	adj_sprite_ipgws_ptr_group_16x32 the_ptr_group( *the_sublevel, 
		block_grid_coord.x, block_grid_coord.y );
	
	
	if ( the_ptr_group.can_add_sprite() )
	{
		add_sprite_and_record_ur_stuff();
	}
	
}



// Editing functions to activate upon key press.  These are meant to
// actually 
void editing_manager::copy_rs_contents
	( rect_selection_stuff& the_rect_selection_stuff )
{
	the_rect_selection_stuff.copy_rs_contents();
}


void editing_manager::paste_copied_rs_contents
	( level_editor_core_widget* the_core_widget,
	rect_selection_stuff& the_rect_selection_stuff )
{
	level_editor_sfml_canvas_widget* the_sfml_canvas_widget
		= the_core_widget->the_sfml_canvas_widget.get();
	
	sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	if ( the_rect_selection_stuff.get_enabled() )
	{
		finalize_movement_of_rs_contents( the_core_widget,
			the_rect_selection_stuff );
	}
	
	//cout << "Pasting the copied selection contents\n";
	
	//the_rect_selection_stuff.paste_copied_selection_contents
	//	( vec2_s32( 0, 0 ) );
	
	sf::FloatRect visible_rect 
		= the_sfml_canvas_widget->get_visible_rect();
	
	//u32 scale_factor = the_sfml_canvas_widget->scale_factor;
	
	
	
	vec2<double> visible_block_grid_start_pos = the_sfml_canvas_widget
		->get_basic_visible_block_grid_start_pos(visible_rect);
	vec2<double> visible_block_grid_size_2d = the_sfml_canvas_widget
		->get_basic_visible_block_grid_size_2d(visible_rect);
	
	// This is so that sprites larger than 16x16 pixels will be placed
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
	
	if ( ( visible_block_grid_start_pos.x + visible_block_grid_size_2d.x )
		>= (s32)the_sublevel->real_size_2d.x )
	{
		visible_block_grid_size_2d.x = the_sublevel->real_size_2d.x
			- visible_block_grid_start_pos.x;
	}
	if ( ( visible_block_grid_start_pos.y + visible_block_grid_size_2d.y )
		>= (s32)the_sublevel->real_size_2d.y )
	{
		visible_block_grid_size_2d.y = the_sublevel->real_size_2d.y
			- visible_block_grid_start_pos.y;
	}
	
	
	
	// Paste at the location of the mouse.
	sf::FloatRect visible_block_grid_rect
		( visible_block_grid_start_pos.x,
		visible_block_grid_start_pos.y,
		visible_block_grid_size_2d.x, visible_block_grid_size_2d.y );
	
	
	sf::Vector2i mouse_pos_in_canvas_widget_coords;
	sf::Vector2f mouse_pos_in_canvas_coords;
	vec2_s32 block_grid_coords_of_mouse_pos;
	
	get_a_few_types_of_mouse_pos( the_sfml_canvas_widget,
		mouse_pos_in_canvas_widget_coords, mouse_pos_in_canvas_coords,
		block_grid_coords_of_mouse_pos );
	
	
	if ( visible_block_grid_rect.contains
		( block_grid_coords_of_mouse_pos.x, 
		block_grid_coords_of_mouse_pos.y ) )
	{
		the_rect_selection_stuff.paste_copied_rs_contents
			(block_grid_coords_of_mouse_pos);
	}
	//else
	//{
	//	the_rect_selection_stuff.paste_copied_rs_contents( vec2_s32
	//		( visible_block_grid_start_pos.x, 
	//		visible_block_grid_start_pos.y ) );
	//}
}


bool editing_manager::undo( level_editor_core_widget* the_core_widget )
{
	undo_and_redo_stuff& ur_stuff 
		= get_or_create_ur_stuff(the_core_widget);
	
	undo_and_redo_stack& ur_stack = ur_stuff.ur_stack;
	
	if ( !ur_stack.can_undo() )
	{
		return false;
	}
	
	undo_and_redo_action& curr_ur_action = ur_stack.get_curr_action();
	
	const action_type& the_action_type = curr_ur_action.the_action_type;
	
	sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	rect_selection_stuff& the_rect_selection_stuff = the_core_widget
		->the_sfml_canvas_widget->the_rect_selection_stuff;
	
	if ( the_rect_selection_stuff.get_enabled() 
		&& !the_rect_selection_stuff.get_single_sprite_selected() )
	{
		finalize_movement_of_rs_contents( the_core_widget,
			the_rect_selection_stuff );
		return false;
	}
	
	
	cout << "Hey, don't forget about finalize_undo()!\n";
	
	if ( the_action_type == at_place_blocks )
	{
		// Since placing blocks only changes the same blocks that are
		// replaced, it is only necessary to loop through the
		// replaced_block_umap's contents
		
		for ( auto iter : curr_ur_action.replaced_block_umap )
		{
			const vec2_s32& block_grid_pos = iter.first;
			const block& the_replaced_block = iter.second;
			
			
			the_sublevel->get_block_at_block_grid_pos(block_grid_pos)
				= the_replaced_block;
		}
	}
	else if ( the_action_type == at_place_sprite )
	{
		const sprite_ipgws& new_sprite_ipgws
			= curr_ur_action.new_sprite_ipgws;
		
		const vec2_s32 block_grid_pos
			( new_sprite_ipgws.initial_block_grid_x_coord, 
			new_sprite_ipgws.initial_block_grid_y_coord );
			
		
		the_sublevel->get_sprite_ipgws_at_block_grid_pos
			(block_grid_pos) = sprite_ipgws();
	}
	else if ( the_action_type == at_modify_sprite )
	{
		const sprite_ipgws& old_sprite_ipgws 
			= curr_ur_action.old_sprite_ipgws;
		const vec2_s32 block_grid_pos
			( old_sprite_ipgws.initial_block_grid_x_coord, 
			old_sprite_ipgws.initial_block_grid_y_coord );
		
		sprite_ipgws* the_sprite_ipgws = &(the_sublevel
			->get_sprite_ipgws_at_block_grid_pos(block_grid_pos));
		
		*the_sprite_ipgws = old_sprite_ipgws;
		
		the_rect_selection_stuff.enable_single_sprite_selection
			(the_sprite_ipgws);
		
		the_core_widget->do_emit_sprite_was_selected();
	}
	else if ( the_action_type == at_finish_moving_non_pasted_blocks )
	{
		for ( auto rbum_iter : curr_ur_action.replaced_block_umap )
		{
			const vec2_s32& block_grid_pos = rbum_iter.first;
			const block& the_block = rbum_iter.second;
			
			the_sublevel->get_block_at_block_grid_pos(block_grid_pos)
				= the_block;
		}
		
		for ( auto obum_iter : curr_ur_action.old_block_umap )
		{
			const vec2_s32& block_grid_pos = obum_iter.first;
			const block& the_block = obum_iter.second;
			
			the_sublevel->get_block_at_block_grid_pos(block_grid_pos)
				= the_block;
		}
	}
	else if ( the_action_type == at_finish_moving_non_pasted_sprites )
	{
		for ( auto iter : curr_ur_action.replaced_sprite_ipgws_umap )
		{
			const vec2_s32& block_grid_pos = iter.first;
			const sprite_ipgws& the_sprite_ipgws = iter.second;
			
			the_sublevel
				->get_sprite_ipgws_at_block_grid_pos(block_grid_pos)
				= the_sprite_ipgws;
		}
		
		for ( auto iter : curr_ur_action.old_sprite_ipgws_umap )
		{
			const vec2_s32& block_grid_pos = iter.first;
			const sprite_ipgws& the_sprite_ipgws = iter.second;
			
			the_sublevel
				->get_sprite_ipgws_at_block_grid_pos(block_grid_pos)
				= the_sprite_ipgws;
		}
	}
	else if ( the_action_type == at_finish_moving_pasted_blocks )
	{
		for ( auto rbum_iter : curr_ur_action.replaced_block_umap )
		{
			const vec2_s32& block_grid_pos = rbum_iter.first;
			const block& the_block = rbum_iter.second;
			
			the_sublevel->get_block_at_block_grid_pos(block_grid_pos)
				= the_block;
		}
	}
	else if ( the_action_type == at_finish_moving_pasted_sprites )
	{
		for ( auto iter : curr_ur_action.replaced_sprite_ipgws_umap )
		{
			const vec2_s32& block_grid_pos = iter.first;
			const sprite_ipgws& the_sprite_ipgws = iter.second;
			
			the_sublevel
				->get_sprite_ipgws_at_block_grid_pos(block_grid_pos)
				= the_sprite_ipgws;
		}
	}
	else if ( the_action_type == at_resize_sublevel )
	{
		
	}
	
	
	ur_stack.finalize_undo();
	
	return true;
}

bool editing_manager::redo( level_editor_core_widget* the_core_widget )
{
	undo_and_redo_stuff& ur_stuff 
		= get_or_create_ur_stuff(the_core_widget);
	
	undo_and_redo_stack& ur_stack = ur_stuff.ur_stack;
	
	if ( !ur_stack.can_redo() )
	{
		return false;
	}
	
	
	undo_and_redo_action& next_ur_action = ur_stack.get_next_action();
	
	const action_type& the_action_type = next_ur_action.the_action_type;
	
	cout << "Hey, don't forget about finalize_redo()!\n";
	
	if ( the_action_type == at_place_blocks )
	{
		
	}
	else if ( the_action_type == at_place_sprite )
	{
		
	}
	else if ( the_action_type == at_modify_sprite )
	{
		
	}
	else if ( the_action_type == at_finish_moving_non_pasted_blocks )
	{
		
	}
	else if ( the_action_type == at_finish_moving_non_pasted_sprites )
	{
		
	}
	else if ( the_action_type == at_finish_moving_pasted_blocks )
	{
		
	}
	else if ( the_action_type == at_finish_moving_pasted_sprites )
	{
		
	}
	else if ( the_action_type == at_resize_sublevel )
	{
		
	}
	
	
	return true;
}


void editing_manager::finalize_movement_of_rs_contents_block_ur_stuff
	( level_editor_core_widget* the_core_widget, 
	rect_selection_stuff& the_rect_selection_stuff, 
	const sf::IntRect& selection_rect, 
	const sf::IntRect& selection_rect_before_moving, bool rs_was_pasted )
{
	//if ( the_core_widget->the_mouse_mode == mm_select_single_sprite )
	//{
	//	return;
	//}
	
	sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	undo_and_redo_stuff& ur_stuff 
		= get_or_create_ur_stuff(the_core_widget);
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	if ( !rs_was_pasted )
	{
		ur_action_to_push.the_action_type 
			= at_finish_moving_non_pasted_blocks;
		
		
		ur_action_to_push.selection_rect_before_moving 
			= selection_rect_before_moving;
	}
	else //if ( rs_was_pasted )
	{
		ur_action_to_push.the_action_type 
			= at_finish_moving_pasted_blocks;
	}
	
	for ( s32 j=0; j<selection_rect.height; ++j )
	{
		for ( s32 i=0; i<selection_rect.width; ++i )
		{
			vec2_s32 block_grid_pos( selection_rect.left + i,
				selection_rect.top +j );
			
			if ( !the_sublevel->contains_block_grid_pos(block_grid_pos) )
			{
				ur_action_to_push.replaced_block_umap[block_grid_pos]
					= block();
			}
			else
			{
				ur_action_to_push.replaced_block_umap[block_grid_pos]
					= the_sublevel
					->get_block_at_block_grid_pos(block_grid_pos);
			}
			
			if ( !rs_was_pasted )
			{
				vec2_s32 original_block_grid_pos
					( selection_rect_before_moving.left + i,
					selection_rect_before_moving.top + j );
				
				const block& the_block = the_rect_selection_stuff
					.moving_block_vec_2d.at(j).at(i);
				
				ur_action_to_push.old_block_umap[original_block_grid_pos]
					= the_block;
				
				ur_action_to_push.new_block_umap[block_grid_pos]
					= the_block;
			}
			else //if ( rs_was_pasted )
			{
				ur_action_to_push.new_block_umap[block_grid_pos]
					= the_rect_selection_stuff.copied_block_vec_2d
					.at(j).at(i);
			}
		}
	}
	
}


void editing_manager::finalize_movement_of_rs_contents_sprite_ur_stuff
	( level_editor_core_widget* the_core_widget, 
	rect_selection_stuff& the_rect_selection_stuff, 
	const sf::IntRect& selection_rect, 
	const sf::IntRect& selection_rect_before_moving, bool rs_was_pasted )
{
	//if ( the_core_widget->the_mouse_mode == mm_select_single_sprite )
	//{
	//	return;
	//}
	
	sublevel* the_sublevel = the_core_widget->the_sublevel;
	
	undo_and_redo_stuff& ur_stuff 
		= get_or_create_ur_stuff(the_core_widget);
	undo_and_redo_action& ur_action_to_push = ur_stuff.ur_action_to_push;
	
	if ( !rs_was_pasted )
	{
		ur_action_to_push.the_action_type 
			= at_finish_moving_non_pasted_sprites;
		
		
		ur_action_to_push.selection_rect_before_moving 
			= selection_rect_before_moving;
	}
	else //if ( rs_was_pasted )
	{
		ur_action_to_push.the_action_type 
			= at_finish_moving_pasted_sprites;
	}
	
	
	// These three lambda functions have, uh, weird names, which is an
	// interesting coincidence.
	auto say_that_size_2d_y_is_weird = []() -> void
	{
		cout << "finalize_movement_of_rs_contents_sprite_ur_stuff():  "
			<< "weird issue where the_sprite_ipgws.size_2d.y is neither "
			<< "16 or 32.\n";
	};
	auto say_that_size_2d_x_is_weird = []() -> void
	{
		cout << "finalize_movement_of_rs_contents_sprite_ur_stuff():  "
			<< "weird issue where the_sprite_ipgws.size_2d.x is neither "
			<< "16 or 32.\n";
	};
	auto say_that_size_2d_combo_is_weird = []() -> void
	{
		cout << "finalize_movement_of_rs_contents_sprite_ur_stuff():  "
			<< "weird issue where the_sprite_ipgws.size_2d.x == 32 and "
			<< "the_sprite_ipgws->size_2d.y == 16.\n";
	};
	
	auto single_sprite_handler = [&]( const vec2_s32& block_grid_pos,
		s32 j, s32 i ) -> void
	{
		sprite_ipgws the_sprite_ipgws;
		
		if ( !rs_was_pasted )
		{
			the_sprite_ipgws = the_rect_selection_stuff
				.moving_sprite_ipgws_vec_2d.at(j).at(i);
		}
		else //if ( rs_was_pasted )
		{
			the_sprite_ipgws = the_rect_selection_stuff
				.copied_sprite_ipgws_vec_2d.at(j).at(i);
		}
		
		if ( !rs_was_pasted )
		{
			//vec2_s32 original_block_grid_pos
			//	( the_sprite_ipgws.initial_block_grid_x_coord, 
			//	the_sprite_ipgws.initial_block_grid_y_coord );
			vec2_s32 original_block_grid_pos
				( selection_rect_before_moving.left + i, 
				selection_rect_before_moving.top + j );
			
			ur_action_to_push
				.old_sprite_ipgws_umap[original_block_grid_pos] 
				= the_sprite_ipgws;
		}
		
		//if ( the_sprite_ipgws.type == st_default )
		//{
		//	//cout << "wow\n";
		//	return;
		//}
		
		if ( !the_sublevel->contains_block_grid_pos(block_grid_pos) )
		{
			//ur_action_to_push.replaced_sprite_ipgws_uset
			//	.insert(sprite_ipgws());
			return;
		}
		
		the_sprite_ipgws.initial_block_grid_x_coord = block_grid_pos.x;
		the_sprite_ipgws.initial_block_grid_y_coord = block_grid_pos.y;
		
		//ur_action_to_push.new_sprite_ipgws_uset.insert(the_sprite_ipgws);
		//ur_action_to_push.new_sprite_ipgws_uset.insert(the_sprite_ipgws);
		
		ur_action_to_push.new_sprite_ipgws_umap[block_grid_pos] 
			= the_sprite_ipgws;
		
		const sprite_ipgws& the_replaced_sprite_ipgws = the_sublevel
			->get_sprite_ipgws_at_block_grid_pos(block_grid_pos);
		
		if ( the_replaced_sprite_ipgws.type == st_default )
		{
			ur_action_to_push.replaced_sprite_ipgws_umap[block_grid_pos]
				= the_replaced_sprite_ipgws;
			
			return;
		}
		
		if ( the_sprite_ipgws.size_2d.x == 16 )
		{
			// 16x16
			if ( the_sprite_ipgws.size_2d.y == 16 )
			{
				adj_sprite_ipgws_ptr_group_16x16 ptr_group( *the_sublevel, 
					block_grid_pos.x, block_grid_pos.y );
				
				#define save_replaced_sprites(rel_pos) \
					if ( ptr_group.overlaps_##rel_pos() ) \
					{ \
						ur_action_to_push \
							.replaced_sprite_ipgws_umap[block_grid_pos] \
							= *ptr_group.rel_pos##_ptr; \
					}
				
				list_of_16x16_sprite_pg_stuff(save_replaced_sprites);
				
				#undef save_replaced_sprites
			}
			
			// 16x32
			else if ( the_sprite_ipgws.size_2d.y == 32 )
			{
				adj_sprite_ipgws_ptr_group_16x32 ptr_group( *the_sublevel, 
					block_grid_pos.x, block_grid_pos.y );
				
				#define save_replaced_sprites(rel_pos) \
					if ( ptr_group.overlaps_##rel_pos() ) \
					{ \
						ur_action_to_push \
							.replaced_sprite_ipgws_umap[block_grid_pos] \
							= *ptr_group.rel_pos##_ptr; \
					}
				
				list_of_16x32_sprite_pg_stuff(save_replaced_sprites);
				
				#undef save_replaced_sprites
			}
			
			// Invalid size_2d
			else
			{
				say_that_size_2d_y_is_weird();
			}
		}
		else if ( the_sprite_ipgws.size_2d.x == 32 )
		{
			// 32x16, which isn't allowed
			if ( the_sprite_ipgws.size_2d.y == 16 )
			{
				say_that_size_2d_combo_is_weird();
			}
			
			// 32x32
			else if ( the_sprite_ipgws.size_2d.y == 32 )
			{
				adj_sprite_ipgws_ptr_group_32x32 ptr_group( *the_sublevel, 
					block_grid_pos.x, block_grid_pos.y );
				
				#define save_replaced_sprites(rel_pos) \
					if ( ptr_group.overlaps_##rel_pos() ) \
					{ \
						ur_action_to_push \
							.replaced_sprite_ipgws_umap[block_grid_pos] \
							= *ptr_group.rel_pos##_ptr; \
					}
				
				list_of_32x32_sprite_pg_stuff(save_replaced_sprites);
				
				#undef save_replaced_sprites
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
		
	};
	
	for ( s32 j=0; j<selection_rect.height; ++j )
	{
		for ( s32 i=0; i<selection_rect.width; ++i )
		{
			vec2_s32 block_grid_pos( selection_rect.left + i,
				selection_rect.top +j );
			
			
			single_sprite_handler( block_grid_pos, j, i );
		}
	}
	
}


