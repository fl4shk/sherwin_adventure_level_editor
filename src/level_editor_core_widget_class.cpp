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


#include "level_editor_core_widget_class.hpp"

#include "block_selector_widget_class.hpp"
#include "sprite_16x16_selector_widget_class.hpp"
#include "sprite_16x32_selector_widget_class.hpp"

#include "block_selector_core_widget_class.hpp"
#include "sprite_16x16_selector_core_widget_class.hpp"
#include "sprite_16x32_selector_core_widget_class.hpp"



level_editor_core_widget::level_editor_core_widget( QWidget* s_parent,
	const QPoint& s_position, const string& s_level_file_name, 
	sublevel* s_the_sublevel )
	: QWidget(s_parent), parent(s_parent),
	level_file_name(s_level_file_name), current_position(s_position), 
	the_sublevel(s_the_sublevel), the_mouse_mode(mm_place_level_elements)
{
	shared_constructor_code( s_position, the_sublevel->real_size_2d );
}


level_editor_core_widget::level_editor_core_widget( QWidget* s_parent,
	const QPoint& s_position, const string& s_level_file_name, 
	sublevel* s_the_sublevel, const vec2_u32& s_sublevel_real_size_2d )
	: QWidget(s_parent), parent(s_parent),
	level_file_name(s_level_file_name), current_position(s_position), 
	the_sublevel(s_the_sublevel), the_mouse_mode(mm_place_level_elements)
{
	shared_constructor_code( s_position, s_sublevel_real_size_2d );
}

void level_editor_core_widget::shared_constructor_code
	( const QPoint& s_position, const vec2_u32& s_sublevel_real_size_2d )
{
	// Set strong focus to enable keyboard events (and maybe mouse events?)
	// to be received.
	setFocusPolicy(Qt::StrongFocus);
	
	the_sublevel->real_size_2d = s_sublevel_real_size_2d;
	
	current_size = QSize( the_sublevel->real_size_2d.x
		* sfml_canvas_widget::num_pixels_per_block_column,
		the_sublevel->real_size_2d.y
		* sfml_canvas_widget::num_pixels_per_block_row );
	
	move(current_position);
	resize(current_size);
	
	//the_sfml_canvas_widget = new sfml_canvas_widget( this, s_position, 
	//	s_size );
	the_sfml_canvas_widget.reset(new sfml_canvas_widget( this, 
		current_position, current_size ));
	
	//the_sublevel.real_size_2d = vec2_u32( s_size.width() 
	//	/ sfml_canvas_widget::num_pixels_per_block_column, s_size.height() 
	//	/ sfml_canvas_widget::num_pixels_per_block_row );
	//
	//the_sfml_canvas_widget->set_the_sublevel(&the_sublevel);
	
	//*the_level = level(s_sublevel_size_2d_vec);
	//curr_sublevel_index = 0;
	
	the_sfml_canvas_widget->set_the_sublevel(the_sublevel);
	
	
	//setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	//setMinimumSize( QSize( 512, 512 ) );
}


void level_editor_core_widget::initialize_tab_stuff
	( QTabWidget* n_level_element_selectors_tab_widget, 
	block_selector_widget* n_the_block_selector_widget,
	sprite_16x16_selector_widget* n_the_sprite_16x16_selector_widget,
	sprite_16x32_selector_widget* n_the_sprite_16x32_selector_widget )
{
	level_element_selectors_tab_widget 
		= n_level_element_selectors_tab_widget;
	
	#define X(name) \
	the_##name##_selector_widget = n_the_##name##_selector_widget; \
	the_##name##_selector_widget->the_core_widget \
		->set_the_level_editor_core_widget(this); \
	the_sfml_canvas_widget->set_the_##name##_selector_core_widget \
		( get_the_##name##_selector_core_widget() );
	
	list_of_level_element_widget_name_prefixes(X)
	#undef X
}

void level_editor_core_widget::reinitialize( const QSize& n_size,
	QScrollArea* n_scroll_area )
{
	current_size = n_size;
	
	move(current_position);
	resize(current_size);
	
	the_sfml_canvas_widget.reset(new sfml_canvas_widget( this, 
		current_position, current_size ));
	the_sfml_canvas_widget->set_the_sublevel(the_sublevel);
	
	#define X(name) \
	the_sfml_canvas_widget->set_the_##name##_selector_core_widget \
		(get_the_##name##_selector_core_widget());
	
	list_of_level_element_widget_name_prefixes(X)
	#undef X
	
	the_sfml_canvas_widget->set_scroll_area(n_scroll_area);
	
}


block_selector_core_widget* 
	level_editor_core_widget::get_the_block_selector_core_widget()
{
	return the_block_selector_widget->the_core_widget;
}
sprite_16x16_selector_core_widget* 
	level_editor_core_widget::get_the_sprite_16x16_selector_core_widget()
{
	return the_sprite_16x16_selector_widget->the_core_widget;
}
sprite_16x32_selector_core_widget* 
	level_editor_core_widget::get_the_sprite_16x32_selector_core_widget()
{
	return the_sprite_16x32_selector_widget->the_core_widget;
}


bool level_editor_core_widget::zoom_in()
{
	//if ( the_sfml_canvas_widget->scale_factor == 16 )
	//{
	//	return false;
	//}
	if ( the_sfml_canvas_widget->scale_factor == 4 )
	{
		return false;
	}
	
	the_sfml_canvas_widget->scale_factor <<= 1;
	the_sfml_canvas_widget->zoomed_in_recently = true;
	
	return true;
}


bool level_editor_core_widget::zoom_out()
{
	if ( the_sfml_canvas_widget->scale_factor == 1 )
	{
		return false;
	}
	
	the_sfml_canvas_widget->scale_factor >>= 1;
	the_sfml_canvas_widget->zoomed_out_recently = true;
	
	return true;
}




// Utility functions

void level_editor_core_widget::draw_block_line( const sf::Vector2i& pos_0, 
	const sf::Vector2i& pos_1, block_type the_block_type )
{
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
	//	canvas_image->setPixel( (u32)pixel_coord.x, (u32)pixel_coord.y, 
	//		color );
	//}
	
	if ( block_coord.x >= (s32)0 
		&& block_coord.x < (s32)the_sublevel->real_size_2d.x 
		&& block_coord.y >= (s32)0 
		&& block_coord.y < (s32)the_sublevel->real_size_2d.y )
	{
		the_sublevel->uncompressed_block_data_vec_2d
			[(u32)block_coord.y][(u32)block_coord.x].type = the_block_type;
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
			//	canvas_image->setPixel( (u32)pixel_coord.x, 
			//		(u32)pixel_coord.y, color );
			//}
			
			if ( block_coord.x >= (s32)0 
				&& ( block_coord.x 
				< (s32)the_sublevel->real_size_2d.x )
				&& block_coord.y >= (s32)0 
				&& ( block_coord.y 
				< (s32)the_sublevel->real_size_2d.y ) )
			{
				the_sublevel->uncompressed_block_data_vec_2d
					[(u32)block_coord.y][(u32)block_coord.x].type 
					= the_block_type;
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
			//	canvas_image->setPixel( (u32)pixel_coord.x, 
			//		(u32)pixel_coord.y, color );
			//}
			
			if ( block_coord.x >= (s32)0 
				&& ( block_coord.x 
				< (s32)the_sublevel->real_size_2d.x )
				&& block_coord.y >= (s32)0 
				&& ( block_coord.y 
				< (s32)the_sublevel->real_size_2d.y ) )
			{
				the_sublevel->uncompressed_block_data_vec_2d
					[(u32)block_coord.y][(u32)block_coord.x].type 
					= the_block_type;
			}
		}
	}
	
}



// Events

void level_editor_core_widget::mousePressEvent( QMouseEvent* event )
{
	sf::Vector2i mouse_pos_in_canvas_widget_coords 
		= sf::Mouse::getPosition(*the_sfml_canvas_widget);
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f mouse_pos_in_canvas_coords
		( (double)mouse_pos_in_canvas_widget_coords.x 
		/ (double)the_sfml_canvas_widget->scale_factor,
		(double)mouse_pos_in_canvas_widget_coords.y
		/ (double)the_sfml_canvas_widget->scale_factor );
	
	u32 scale_factor = the_sfml_canvas_widget->scale_factor;
	
	vec2_s32 block_grid_coords_of_mouse_pos
		= { (s32)( mouse_pos_in_canvas_coords.x
		/ ( sfml_canvas_widget::num_pixels_per_block_row ) ),
		
		(s32)( ( the_sublevel->real_size_2d.y 
		- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
		- mouse_pos_in_canvas_coords.y )
		/ sfml_canvas_widget::num_pixels_per_block_column ) ) };
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	if ( block_grid_coords_of_mouse_pos.x < (s32)0
		|| block_grid_coords_of_mouse_pos.x 
			>= (s32)the_sublevel->real_size_2d.x
		|| block_grid_coords_of_mouse_pos.y < (s32)0
		|| block_grid_coords_of_mouse_pos.y 
			>= (s32)the_sublevel->real_size_2d.y )
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
		= the_sublevel->uncompressed_block_data_vec_2d
		.at((u32)block_grid_coords_of_mouse_pos.y)
		.at((u32)block_grid_coords_of_mouse_pos.x);
	
	sprite_init_param_group_with_size& the_sprite_ipgws_at_mouse_pos
		= the_sublevel->sprite_ipgws_vec_2d
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
	//	the_sprite_selection_ptr_group( *the_sublevel,
	//	block_grid_coords_of_mouse_pos.x, 
	//	block_grid_coords_of_mouse_pos.y );
	
	
	the_sprite_selection_ptr_group 
		= adj_sprite_ipgws_ptr_group_for_selecting_sprite
		( *the_sublevel, block_grid_coords_of_mouse_pos.x, 
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
			emit sprite_no_longer_selected();
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
		emit sprite_no_longer_selected();
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
				= get_the_block_selector_core_widget()
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
					= (sprite_type)
					(get_the_sprite_16x16_selector_core_widget()
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
				the_ptr_group( *the_sublevel, 
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
					= (sprite_type)
					(get_the_sprite_16x32_selector_core_widget()
					->get_left_current_level_element_index());
				
				the_sprite_ipgws_at_mouse_pos.initial_block_grid_x_coord
					= block_grid_coords_of_mouse_pos.x;
				the_sprite_ipgws_at_mouse_pos.initial_block_grid_y_coord
					= block_grid_coords_of_mouse_pos.y;
				
				the_sprite_ipgws_at_mouse_pos.size_2d = vec2_u32( 16, 32 );
			};
			
			adj_sprite_ipgws_ptr_group_for_placing_sprite_16x32 
				the_ptr_group( *the_sublevel, 
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
			
			emit sprite_no_longer_selected();
			return;
		}
		
		sprite_init_param_group_with_size* clicked_sprite_ipgws 
			= the_sprite_selection_ptr_group.origin_ptr;
		
		//the_sfml_canvas_widget->enable_single_sprite_rect_selection
		//	(clicked_sprite_ipgws);
		the_sfml_canvas_widget->the_rect_selection_stuff
			.enable_single_sprite_selection(clicked_sprite_ipgws);
		
		emit sprite_was_selected();
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
		
		emit sprite_no_longer_selected();
	};
	
	if ( event->button() == Qt::LeftButton )
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
	else if ( event->button() == Qt::RightButton )
	{
		
		//emit right_mouse_button_pressed();
	}
	
	
	//prev_mouse_pos = event->pos();
	block_grid_coords_of_prev_mouse_pos = block_grid_coords_of_mouse_pos;
}

void level_editor_core_widget::mouseMoveEvent( QMouseEvent* event )
{
	//cout << "mouseMoveEvent()\n";
	
	sf::Vector2i mouse_pos_in_canvas_widget_coords 
		= sf::Mouse::getPosition(*the_sfml_canvas_widget);
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f mouse_pos_in_canvas_coords
		( (double)mouse_pos_in_canvas_widget_coords.x 
		/ (double)the_sfml_canvas_widget->scale_factor,
		(double)mouse_pos_in_canvas_widget_coords.y
		/ (double)the_sfml_canvas_widget->scale_factor );
	
	u32 scale_factor = the_sfml_canvas_widget->scale_factor;
	
	vec2_s32 block_grid_coords_of_mouse_pos
		= { (s32)( mouse_pos_in_canvas_coords.x
		/ ( sfml_canvas_widget::num_pixels_per_block_row ) ),
		
		(s32)( ( the_sublevel->real_size_2d.y 
		- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
		- mouse_pos_in_canvas_coords.y )
		/ sfml_canvas_widget::num_pixels_per_block_column ) ) };
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	//if ( block_grid_coords_of_mouse_pos.x < (s32)0
	//	|| block_grid_coords_of_mouse_pos.x 
	//		>= (s32)the_sublevel->size_2d.x
	//	|| block_grid_coords_of_mouse_pos.y < (s32)0
	//	|| block_grid_coords_of_mouse_pos.y 
	//		>= (s32)the_sublevel->size_2d.y )
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
	//	= the_sublevel->sprite_ipgws_vec_2d
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
	//	the_sprite_selection_ptr_group( *the_sublevel,
	//	block_grid_coords_of_mouse_pos.x, 
	//	block_grid_coords_of_mouse_pos.y );
	
	the_sprite_selection_ptr_group 
		= adj_sprite_ipgws_ptr_group_for_selecting_sprite
		( *the_sublevel, block_grid_coords_of_mouse_pos.x, 
		block_grid_coords_of_mouse_pos.y );
	
	//if ( the_sprite_selection_ptr_group.origin_ptr != NULL )
	//{
	//	if ( the_sprite_selection_ptr_group.origin_ptr->type 
	//		== st_default )
	//	{
	//		the_sfml_canvas_widget->disable_rect_selection();
	//		
	//		//cout << "st_default\n";
	//		emit sprite_no_longer_selected();
	//	}
	//}
	//// I am not sure this will ever be the case.
	//else 
	//{
	//	the_sfml_canvas_widget->disable_rect_selection();
	//	
	//	//cout << "else\n";
	//	emit sprite_no_longer_selected();
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
					>= (s32)the_sublevel->real_size_2d.x
				|| block_grid_coords_of_mouse_pos.y < (s32)0
				|| block_grid_coords_of_mouse_pos.y 
					>= (s32)the_sublevel->real_size_2d.y )
			{
				//return;
			}
			else
			{
				block& the_block_at_mouse_pos 
					= the_sublevel->uncompressed_block_data_vec_2d
					.at((u32)block_grid_coords_of_mouse_pos.y)
					.at((u32)block_grid_coords_of_mouse_pos.x);
				
				the_block_at_mouse_pos.type 
					= get_the_block_selector_core_widget()
					->get_left_current_level_element_index();
			}
			
			draw_block_line( block_grid_coords_of_prev_mouse_pos,
				block_grid_coords_of_mouse_pos, 
				(block_type)get_the_block_selector_core_widget()
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
		
		emit sprite_no_longer_selected();
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


void level_editor_core_widget::mouseReleaseEvent( QMouseEvent* event )
{
	//cout << "mouseReleaseEvent()\n";
	
	sf::Vector2i mouse_pos_in_canvas_widget_coords 
		= sf::Mouse::getPosition(*the_sfml_canvas_widget);
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f mouse_pos_in_canvas_coords
		( (double)mouse_pos_in_canvas_widget_coords.x 
		/ (double)the_sfml_canvas_widget->scale_factor,
		(double)mouse_pos_in_canvas_widget_coords.y
		/ (double)the_sfml_canvas_widget->scale_factor );
	
	u32 scale_factor = the_sfml_canvas_widget->scale_factor;
	
	vec2_s32 block_grid_coords_of_mouse_pos
		= { (s32)( mouse_pos_in_canvas_coords.x
		/ ( sfml_canvas_widget::num_pixels_per_block_row ) ),
		
		(s32)( ( the_sublevel->real_size_2d.y 
		- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
		- mouse_pos_in_canvas_coords.y )
		/ sfml_canvas_widget::num_pixels_per_block_column ) ) };
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	//if ( block_grid_coords_of_mouse_pos.x < (s32)0
	//	|| block_grid_coords_of_mouse_pos.x 
	//		>= (s32)the_sublevel->size_2d.x
	//	|| block_grid_coords_of_mouse_pos.y < (s32)0
	//	|| block_grid_coords_of_mouse_pos.y 
	//		>= (s32)the_sublevel->size_2d.y )
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
	//	= the_sublevel->uncompressed_block_data_vec_2d
	//	.at((u32)block_grid_coords_of_mouse_pos.y)
	//	.at((u32)block_grid_coords_of_mouse_pos.x);
	
	//sprite_init_param_group_with_size& the_sprite_ipgws_at_mouse_pos
	//	= the_sublevel->sprite_ipgws_vec_2d
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
	//	( *the_sublevel, block_grid_coords_of_mouse_pos.x, 
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
		
		emit sprite_no_longer_selected();
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





