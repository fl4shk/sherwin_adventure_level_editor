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


#include "sprite_level_data_stuff.hpp"
#include "sublevel_class.hpp"

adj_sprite_ipgws_ptr_group_for_placing_sprite_16x16
	::adj_sprite_ipgws_ptr_group_for_placing_sprite_16x16
	( sublevel& the_sublevel, u32 origin_block_grid_x_coord, 
	u32 origin_block_grid_y_coord )
{
	// Do some sanity checks.
	if ( origin_block_grid_x_coord >= the_sublevel.real_size_2d.x
		|| origin_block_grid_y_coord >= the_sublevel.real_size_2d.y )
	{
		up_left_ptr = up_ptr
			= left_ptr = origin_ptr = NULL;
		return;
	}
	origin_ptr = &the_sublevel.sprite_ipgws_vec_2d
		.at(origin_block_grid_y_coord).at(origin_block_grid_x_coord);
	
	vec2_s32 up_left_block_grid_pos( (s32)origin_block_grid_x_coord - 1,
			(s32)origin_block_grid_y_coord - 1 ),
		up_block_grid_pos( (s32)origin_block_grid_x_coord, 
			(s32)origin_block_grid_y_coord - 1 ),
		
		left_block_grid_pos( (s32)origin_block_grid_x_coord - 1, 
			(s32)origin_block_grid_y_coord ),
		origin_block_grid_pos( (s32)origin_block_grid_x_coord, 
			(s32)origin_block_grid_y_coord );
	
	if ( up_left_block_grid_pos.x < 0 || up_left_block_grid_pos.y < 0 )
	{
		//up_left_block_grid_pos = { -1, -1 };
		up_left_ptr = NULL;
	}
	else
	{
		up_left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_left_block_grid_pos.y).at(up_left_block_grid_pos.x);
	}
	
	if ( up_block_grid_pos.y < 0 )
	{
		//up_block_grid_pos = { -1, -1 };
		up_ptr = NULL;
	}
	else
	{
		up_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_block_grid_pos.y).at(up_block_grid_pos.x);
	}
	
	
	if ( left_block_grid_pos.x < 0 )
	{
		//left_block_grid_pos = { -1, -1 };
		left_ptr = NULL;
	}
	else
	{
		left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(left_block_grid_pos.y).at(left_block_grid_pos.x);
	}
	
	
}

bool adj_sprite_ipgws_ptr_group_for_placing_sprite_16x16::can_add_sprite()
{
	// Don't let existing sprites be overwritten.
	if ( up_left_ptr != NULL )
	{
		if ( up_left_ptr->size_2d.x > 16 && up_left_ptr->size_2d.y > 16 )
		{
			return false;
		}
	}
	
	if ( up_ptr != NULL )
	{
		if ( up_ptr->size_2d.y > 16 )
		{
			return false;
		}
	}
	
	
	if ( left_ptr != NULL )
	{
		if ( left_ptr->size_2d.x > 16 )
		{
			return false;
		}
	}
	
	if ( origin_ptr != NULL )
	{
		if ( origin_ptr->type != st_default )
		{
			return false;
		}
	}
	
	return true;
}

adj_sprite_ipgws_ptr_group_for_placing_sprite_16x32
	::adj_sprite_ipgws_ptr_group_for_placing_sprite_16x32( sublevel& the_sublevel, 
	u32 origin_block_grid_x_coord, u32 origin_block_grid_y_coord )
{
	// Do some sanity checks.
	if ( origin_block_grid_x_coord >= the_sublevel.real_size_2d.x
		|| origin_block_grid_y_coord >= the_sublevel.real_size_2d.y )
	{
		up_left_ptr = up_ptr
			= left_ptr = origin_ptr
			= down_left_ptr = down_ptr = NULL;
		return;
	}
	origin_ptr = &the_sublevel.sprite_ipgws_vec_2d
		.at(origin_block_grid_y_coord).at(origin_block_grid_x_coord);
	
	vec2_s32 up_left_block_grid_pos( (s32)origin_block_grid_x_coord - 1,
			(s32)origin_block_grid_y_coord - 1 ),
		up_block_grid_pos( (s32)origin_block_grid_x_coord, 
			(s32)origin_block_grid_y_coord - 1 ),
		
		left_block_grid_pos( (s32)origin_block_grid_x_coord - 1, 
			(s32)origin_block_grid_y_coord ),
		origin_block_grid_pos( (s32)origin_block_grid_x_coord, 
			(s32)origin_block_grid_y_coord ),
		
		down_left_block_grid_pos( (s32)origin_block_grid_x_coord - 1,
			(s32)origin_block_grid_y_coord + 1 ),
		down_block_grid_pos( (s32)origin_block_grid_x_coord,
			(s32)origin_block_grid_y_coord + 1 );
	
	if ( up_left_block_grid_pos.x < 0 || up_left_block_grid_pos.y < 0 )
	{
		//up_left_block_grid_pos = { -1, -1 };
		up_left_ptr = NULL;
	}
	else
	{
		up_left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_left_block_grid_pos.y).at(up_left_block_grid_pos.x);
	}
	
	if ( up_block_grid_pos.y < 0 )
	{
		//up_block_grid_pos = { -1, -1 };
		up_ptr = NULL;
	}
	else
	{
		up_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_block_grid_pos.y).at(up_block_grid_pos.x);
	}
	
	
	if ( left_block_grid_pos.x < 0 )
	{
		//left_block_grid_pos = { -1, -1 };
		left_ptr = NULL;
	}
	else
	{
		left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(left_block_grid_pos.y).at(left_block_grid_pos.x);
	}
	
	
	if ( down_left_block_grid_pos.x < 0 
		|| down_left_block_grid_pos.y >= (s32)the_sublevel.real_size_2d.y )
	{
		//down_left_block_grid_pos = { -1, -1 };
		down_left_ptr = NULL;
	}
	else
	{
		down_left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(down_left_block_grid_pos.y).at(down_left_block_grid_pos.x);
	}
	
	if ( down_block_grid_pos.y >= (s32)the_sublevel.real_size_2d.y )
	{
		//down_block_grid_pos = { -1, -1 };
		down_ptr = NULL;
	}
	else
	{
		down_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(down_block_grid_pos.y).at(down_block_grid_pos.x);
	}
	
	
}

bool adj_sprite_ipgws_ptr_group_for_placing_sprite_16x32::can_add_sprite()
{
	// Don't let existing sprites be overwritten.
	if ( up_left_ptr != NULL )
	{
		if ( up_left_ptr->size_2d.x > 16 && up_left_ptr->size_2d.y > 16 )
		{
			return false;
		}
	}
	
	if ( up_ptr != NULL )
	{
		if ( up_ptr->size_2d.y > 16 )
		{
			return false;
		}
	}
	
	
	if ( left_ptr != NULL )
	{
		if ( left_ptr->size_2d.x > 16 )
		{
			return false;
		}
	}
	
	if ( origin_ptr != NULL )
	{
		if ( origin_ptr->type != st_default )
		{
			return false;
		}
	}
	
	
	if ( down_left_ptr != NULL )
	{
		if ( down_left_ptr->size_2d.x > 16 )
		{
			return false;
		}
	}
	
	if ( down_ptr != NULL )
	{
		if ( down_ptr->type != st_default )
		{
			return false;
		}
	}
	
	return true;
}


adj_sprite_ipgws_ptr_group_for_placing_sprite_32x32
	::adj_sprite_ipgws_ptr_group_for_placing_sprite_32x32
	( sublevel& the_sublevel, u32 origin_block_grid_x_coord, 
	u32 origin_block_grid_y_coord )
{
	// Do some sanity checks.
	if ( origin_block_grid_x_coord >= the_sublevel.real_size_2d.x
		|| origin_block_grid_y_coord >= the_sublevel.real_size_2d.y )
	{
		up_left_ptr = up_ptr = up_right_ptr 
			= left_ptr = origin_ptr = right_ptr 
			= down_left_ptr = down_ptr = down_right_ptr = NULL;
		return;
	}
	origin_ptr = &the_sublevel.sprite_ipgws_vec_2d
		.at(origin_block_grid_y_coord).at(origin_block_grid_x_coord);
	
	vec2_s32 up_left_block_grid_pos( (s32)origin_block_grid_x_coord - 1,
			(s32)origin_block_grid_y_coord - 1 ),
		up_block_grid_pos( (s32)origin_block_grid_x_coord, 
			(s32)origin_block_grid_y_coord - 1 ),
		up_right_block_grid_pos( (s32)origin_block_grid_x_coord + 1,
			(s32)origin_block_grid_y_coord - 1 ),
		
		left_block_grid_pos( (s32)origin_block_grid_x_coord - 1, 
			(s32)origin_block_grid_y_coord ),
		origin_block_grid_pos( (s32)origin_block_grid_x_coord, 
			(s32)origin_block_grid_y_coord ),
		right_block_grid_pos( (s32)origin_block_grid_x_coord + 1, 
			(s32)origin_block_grid_y_coord ),
		
		down_left_block_grid_pos( (s32)origin_block_grid_x_coord - 1,
			(s32)origin_block_grid_y_coord + 1 ),
		down_block_grid_pos( (s32)origin_block_grid_x_coord,
			(s32)origin_block_grid_y_coord + 1 ),
		down_right_block_grid_pos( (s32)origin_block_grid_x_coord + 1,
			(s32)origin_block_grid_y_coord + 1 );
	
	if ( up_left_block_grid_pos.x < 0 || up_left_block_grid_pos.y < 0 )
	{
		//up_left_block_grid_pos = { -1, -1 };
		up_left_ptr = NULL;
	}
	else
	{
		up_left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_left_block_grid_pos.y).at(up_left_block_grid_pos.x);
	}
	
	if ( up_block_grid_pos.y < 0 )
	{
		//up_block_grid_pos = { -1, -1 };
		up_ptr = NULL;
	}
	else
	{
		up_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_block_grid_pos.y).at(up_block_grid_pos.x);
	}
	
	if ( up_right_block_grid_pos.x >= (s32)the_sublevel.real_size_2d.x 
		|| up_right_block_grid_pos.y < 0 )
	{
		//up_right_block_grid_pos = { -1, -1 };
		up_right_ptr = NULL;
	}
	else
	{
		up_right_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_right_block_grid_pos.y).at(up_right_block_grid_pos.x);
	}
	
	
	if ( left_block_grid_pos.x < 0 )
	{
		//left_block_grid_pos = { -1, -1 };
		left_ptr = NULL;
	}
	else
	{
		left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(left_block_grid_pos.y).at(left_block_grid_pos.x);
	}
	
	if ( right_block_grid_pos.x >= (s32)the_sublevel.real_size_2d.x )
	{
		//right_block_grid_pos = { -1, -1 };
		right_ptr = NULL;
	}
	else
	{
		right_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(right_block_grid_pos.y).at(right_block_grid_pos.x);
	}
	
	
	if ( down_left_block_grid_pos.x < 0 
		|| down_left_block_grid_pos.y >= (s32)the_sublevel.real_size_2d.y )
	{
		//down_left_block_grid_pos = { -1, -1 };
		down_left_ptr = NULL;
	}
	else
	{
		down_left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(down_left_block_grid_pos.y).at(down_left_block_grid_pos.x);
	}
	
	if ( down_block_grid_pos.y >= (s32)the_sublevel.real_size_2d.y )
	{
		//down_block_grid_pos = { -1, -1 };
		down_ptr = NULL;
	}
	else
	{
		down_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(down_block_grid_pos.y).at(down_block_grid_pos.x);
	}
	
	if ( down_right_block_grid_pos.x >= (s32)the_sublevel.real_size_2d.x 
		|| down_right_block_grid_pos.y 
		>= (s32)the_sublevel.real_size_2d.y )
	{
		//down_right_block_grid_pos = { -1, -1 };
		down_right_ptr = NULL;
	}
	else
	{
		down_right_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(down_right_block_grid_pos.y)
			.at(down_right_block_grid_pos.x);
	}
	
}



adj_sprite_ipgws_ptr_group_for_selecting_sprite
	::adj_sprite_ipgws_ptr_group_for_selecting_sprite
	( sublevel& the_sublevel, u32 mouse_pos_block_grid_x_coord, 
	u32 mouse_pos_block_grid_y_coord )
{
	origin_ptr = NULL;
	
	// Do some sanity checks.
	if ( mouse_pos_block_grid_x_coord >= the_sublevel.real_size_2d.x
		|| mouse_pos_block_grid_y_coord >= the_sublevel.real_size_2d.y )
	{
		up_left_ptr = up_right_ptr
			= down_left_ptr = down_right_ptr 
			= origin_ptr = NULL;
		return;
	}
	down_right_ptr = &the_sublevel.sprite_ipgws_vec_2d
		.at(mouse_pos_block_grid_y_coord).at(mouse_pos_block_grid_x_coord);
	
	vec2_s32 up_left_block_grid_pos( (s32)mouse_pos_block_grid_x_coord - 1,
			(s32)mouse_pos_block_grid_y_coord - 1 ),
		up_right_block_grid_pos( (s32)mouse_pos_block_grid_x_coord, 
			(s32)mouse_pos_block_grid_y_coord - 1 ),
		
		down_left_block_grid_pos( (s32)mouse_pos_block_grid_x_coord - 1, 
			(s32)mouse_pos_block_grid_y_coord ),
		down_right_block_grid_pos( (s32)mouse_pos_block_grid_x_coord, 
			(s32)mouse_pos_block_grid_y_coord );
	
	if ( up_left_block_grid_pos.x < 0 || up_left_block_grid_pos.y < 0 )
	{
		//up_left_block_grid_pos = { -1, -1 };
		up_left_ptr = NULL;
	}
	else
	{
		up_left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_left_block_grid_pos.y).at(up_left_block_grid_pos.x);
		
		if ( up_left_ptr->size_2d.x > 16 && up_left_ptr->size_2d.y > 16 )
		{
			origin_ptr = up_left_ptr;
		}
	}
	
	if ( up_right_block_grid_pos.y < 0 )
	{
		//up_right_block_grid_pos = { -1, -1 };
		up_right_ptr = NULL;
	}
	else
	{
		up_right_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(up_right_block_grid_pos.y).at(up_right_block_grid_pos.x);
		
		if ( up_right_ptr->size_2d.y > 16 && origin_ptr == NULL )
		{
			origin_ptr = up_right_ptr;
		}
	}
	
	
	if ( down_left_block_grid_pos.x < 0 )
	{
		//down_left_block_grid_pos = { -1, -1 };
		down_left_ptr = NULL;
	}
	else
	{
		down_left_ptr = &the_sublevel.sprite_ipgws_vec_2d
			.at(down_left_block_grid_pos.y).at(down_left_block_grid_pos.x);
		
		if ( down_left_ptr->size_2d.x > 16 && origin_ptr == NULL )
		{
			origin_ptr = down_left_ptr;
		}
	}
	
	if ( origin_ptr == NULL )
	{
		origin_ptr = down_right_ptr;
	}
}





