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
	const QPoint& s_position, const QSize& s_size, 
	const string& s_level_file_name )
	: QWidget(s_parent), parent(s_parent),
	level_file_name(s_level_file_name),
	the_mouse_mode(mm_place_level_elements)
{
	// Set strong focus to enable keyboard events (and maybe mouse events?)
	// to be received.
	setFocusPolicy(Qt::StrongFocus);
	
	move(s_position);
	resize(s_size);
	
	the_sfml_canvas_widget = new sfml_canvas_widget( this, s_position, 
		s_size );
	
	the_sublevel.real_size_2d = vec2_u32( s_size.width() 
		/ sfml_canvas_widget::num_pixels_per_block_row, s_size.height() 
		/ sfml_canvas_widget::num_pixels_per_block_column );
	
	the_sfml_canvas_widget->set_the_sublevel(&the_sublevel);
	
	setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	setMinimumSize( QSize( 512, 512 ) );
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

void level_editor_core_widget::mousePressEvent( QMouseEvent* event )
{
	//cout << "level_editor_core_widget's mouse position:  "
	//	<< event->x() << ", " << event->y() << endl;
	
	//cout << event->x() << ", " << event->y() << endl;
	
	
	
	//// This converts the clicked coordinate to pixel coordinates.
	//sf::Vector2f event_pos_in_canvas_coords 
	//	= the_sfml_canvas_widget->mapPixelToCoords
	//	( sf::Vector2i( event->x(), event->y() ), 
	//	the_sfml_canvas_widget->get_apparent_view() );
	
	//sf::Vector2i event_pos_in_canvas_pixel_coords
	//	= sf::Vector2i( (int)event_pos_in_canvas_coords.x,
	//	(int)event_pos_in_canvas_coords.y );
	
	
	sf::Vector2i mouse_pos_in_canvas_widget_coords 
		= sf::Mouse::getPosition(*the_sfml_canvas_widget);
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f mouse_pos_in_canvas_coords
		( (double)mouse_pos_in_canvas_widget_coords.x 
		/ (double)the_sfml_canvas_widget->scale_factor,
		(double)mouse_pos_in_canvas_widget_coords.y
		/ (double)the_sfml_canvas_widget->scale_factor );
	
	//cout << mouse_pos_in_canvas_coords.x << ", "
	//	<< mouse_pos_in_canvas_coords.y << endl;
	
	
	if ( !the_sfml_canvas_widget->point_is_in_visible_rect
		( sf::Vector2i( event->x(), event->y() ) ) )
	{
		cout << "out of bounds\n";
		return;
	}
	
	u32 scale_factor = the_sfml_canvas_widget->scale_factor;
	
	vec2_u32 block_grid_coords_of_mouse_pos
		= { (u32)( mouse_pos_in_canvas_coords.x
	/ ( sfml_canvas_widget::num_pixels_per_block_row ) ),
	
	(u32)( ( the_sublevel.real_size_2d.y 
	- ( ( the_sfml_canvas_widget->getSize().y / scale_factor )
	- mouse_pos_in_canvas_coords.y )
	/ sfml_canvas_widget::num_pixels_per_block_column ) ) };
	
	
	block& the_block_at_mouse_pos 
		= the_sublevel.uncompressed_block_data_vec_2d
		[block_grid_coords_of_mouse_pos.y]
		[block_grid_coords_of_mouse_pos.x];
	
	sprite_init_param_group_with_size& the_sprite_ipgws_at_mouse_pos
		= the_sublevel.sprite_ipgws_vec_2d
		[block_grid_coords_of_mouse_pos.y]
		[block_grid_coords_of_mouse_pos.x];
	
	
	bool current_tabbed_widget_is_for_blocks 
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_block_selector_widget );
	bool current_tabbed_widget_is_for_16x16_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x16_selector_widget );
	bool current_tabbed_widget_is_for_16x32_sprites
		= ( level_element_selectors_tab_widget->currentWidget()
		== the_sprite_16x32_selector_widget );
	
	
	auto func_for_placing_level_elements = [&]() -> void
	{
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
				the_ptr_group( the_sublevel, 
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
				the_ptr_group( the_sublevel, 
				block_grid_coords_of_mouse_pos.x,
				block_grid_coords_of_mouse_pos.y );
			
			
			if ( the_ptr_group.can_add_sprite() )
			{
				add_sprite();
			}
			
		}
		
		cout << endl;
	};
	
	auto func_for_selecting_sprites = [&]() -> void
	{
		adj_sprite_ipgws_ptr_group_for_selecting_sprite the_ptr_group
			( the_sublevel, block_grid_coords_of_mouse_pos.x,
			block_grid_coords_of_mouse_pos.y );
		
		
		if ( the_ptr_group.origin_ptr != NULL )
		{
			if ( the_ptr_group.origin_ptr->type == st_default )
			{
				the_sfml_canvas_widget->disable_rect_selection();
				
				//cout << "st_default\n";
				
				return;
			}
		}
		// I am not sure this will ever be the case.
		else 
		{
			the_sfml_canvas_widget->disable_rect_selection();
			
			//cout << "else\n";
			
			return;
		}
		
		//cout << ( the_ptr_group.origin_ptr == NULL ) << endl;
		//cout << "This sprite was clicked:  " 
		//	<< the_ptr_group.origin_ptr->type << ";  "
		//	<< the_ptr_group.origin_ptr->initial_block_grid_x_coord << ", "
		//	<< the_ptr_group.origin_ptr->initial_block_grid_y_coord
		//	<< endl;
		
		
		sprite_init_param_group_with_size* clicked_sprite_ipgws 
			= the_ptr_group.origin_ptr;
		
		the_sfml_canvas_widget->enable_rect_selection( sf::IntRect
			( clicked_sprite_ipgws->initial_block_grid_x_coord,
			clicked_sprite_ipgws->initial_block_grid_y_coord,
			clicked_sprite_ipgws->size_2d.x 
			/ sfml_canvas_widget::num_pixels_per_block_row, 
			clicked_sprite_ipgws->size_2d.y 
			/ sfml_canvas_widget::num_pixels_per_block_column ) );
		
	};
	
	auto func_for_erasing_sprites = [&]() -> void
	{
		
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
			
			case mm_select_sprites:
				func_for_selecting_sprites();
				break;
			
			case mm_erase_sprites:
				func_for_erasing_sprites();
				break;
			
			case mm_rect_select:
				
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
		//canvas_image->setPixel( (u32)mouse_pos_in_image_pixel_coords.x,
		//	(u32)mouse_pos_in_image_pixel_coords.y, 
		//	the_block_selector_core_widget->palette.at
		//	(the_block_selector_core_widget
		//	->right_current_color_index) );
		
	}
	
	
	//prev_mouse_pos = event->pos();
	block_grid_coords_of_prev_mouse_pos = block_grid_coords_of_mouse_pos;
}

void level_editor_core_widget::mouseMoveEvent( QMouseEvent* event )
{
	
}


