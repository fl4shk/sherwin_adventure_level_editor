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

#include <iostream>


bool sprite_ipgws::operator == ( const sprite_ipgws& to_cmp ) const
{
	return ( ( type == to_cmp.type ) 
		&& ( initial_block_grid_x_coord 
			== to_cmp.initial_block_grid_x_coord )
		&& ( initial_block_grid_y_coord 
			== to_cmp.initial_block_grid_y_coord )
		&& ( facing_right == to_cmp.facing_right )
		&& ( extra_param_0 == to_cmp.extra_param_0 )
		&& ( extra_param_1 == to_cmp.extra_param_1 )
		&& ( extra_param_2 == to_cmp.extra_param_2 )
		&& ( extra_param_3 == to_cmp.extra_param_3 )
		&& ( spawn_state == to_cmp.spawn_state )
		&& ( size_2d == to_cmp.size_2d ) );
}

void sprite_ipgws::show_rejection_message() const
{
	cout << "The " << sprite_type_helper::get_st_name(type) << " sprite "
		<< "at the (" << initial_block_grid_x_coord << ", "
		<< initial_block_grid_y_coord << ") coordinate has been "
		<< "rejected!\n";
}


//namespace std
//{
//	size_t hash<sprite_ipgws>::operator () ( const sprite_ipgws& to_hash ) 
//		const
//	{
//		size_t hash_block_grid_coord_x
//			( hash<u32>()(to_hash.initial_block_grid_x_coord) );
//		size_t hash_block_grid_coord_y
//			( hash<u32>()(to_hash.initial_block_grid_y_coord) );
//		size_t hash_facing_right
//			( hash<bool>()(to_hash.facing_right) );
//		size_t hash_extra_param_0
//			( hash<u32>()(to_hash.extra_param_0) );
//		size_t hash_extra_param_1
//			( hash<u32>()(to_hash.extra_param_1) );
//		size_t hash_extra_param_2
//			( hash<u32>()(to_hash.extra_param_2) );
//		size_t hash_extra_param_3
//			( hash<u32>()(to_hash.extra_param_3) );
//		size_t hash_spawn_state
//			( hash<sprite_spawn_state>()(to_hash.spawn_state) );
//		size_t hash_size_2d_x
//			( hash<u32>()(to_hash.size_2d.x) );
//		size_t hash_size_2d_y
//			( hash<u32>()(to_hash.size_2d.y) );
//		
//		const u32 hash_arr_size = 10; 
//		size_t hash_arr[hash_arr_size] = { hash_block_grid_coord_x,
//			hash_block_grid_coord_y, hash_facing_right, hash_extra_param_0,
//			hash_extra_param_1, hash_extra_param_2, hash_extra_param_3,
//			hash_spawn_state, hash_size_2d_x, hash_size_2d_y };
//		
//		size_t ret = hash_arr[0];
//		
//		for ( u32 i=1; i<hash_arr_size-1; ++i )
//		{
//			ret = ( ret ^ ( hash_arr[i] << 1 ) ) >> 1;
//		}
//		
//		ret = ret ^ ( hash_arr[hash_arr_size-1] << 1 );
//		
//		return ret;
//	}
//}


adj_sprite_ipgws_ptr_group_16x16::adj_sprite_ipgws_ptr_group_16x16
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
	
	
	// This doesn't include the origin since it was already assigned.
	// Also, this isn't as efficient as it used to be, with the benefit of
	// fewer lines in this file (and possibly more clarity).
	#define assign_rel_pos_ptr(rel_pos) \
		if ( !the_sublevel.contains_block_grid_pos \
			(rel_pos##_block_grid_pos) ) \
		{ \
			rel_pos##_ptr = NULL; \
		} \
		else \
		{ \
			rel_pos##_ptr = &the_sublevel.sprite_ipgws_vec_2d \
				.at(rel_pos##_block_grid_pos.y) \
				.at(rel_pos##_block_grid_pos.x); \
		}
	
	list_of_16x16_sprite_pg_stuff_no_origin(assign_rel_pos_ptr)
	#undef assign_rel_pos_ptr
	
}

bool adj_sprite_ipgws_ptr_group_16x16::can_add_sprite()
{
	// Don't let existing sprites be overwritten.
	return !( overlaps_up_left() || overlaps_up() || overlaps_left() 
		|| overlaps_origin() );
}

void adj_sprite_ipgws_ptr_group_16x16::erase_overlapping_sprites()
{
	#define macro_to_erase_overlapping_sprite(rel_pos) \
		if ( overlaps_##rel_pos() ) \
		{ \
			*rel_pos##_ptr = sprite_ipgws(); \
		}
	
	list_of_16x16_sprite_pg_stuff(macro_to_erase_overlapping_sprite);
}

adj_sprite_ipgws_ptr_group_16x32::adj_sprite_ipgws_ptr_group_16x32
	( sublevel& the_sublevel, u32 origin_block_grid_x_coord, 
	u32 origin_block_grid_y_coord )
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
	
	
	
	// This doesn't include the origin since it was already assigned.
	// Also, this isn't as efficient as it used to be, with the benefit of
	// fewer lines in this file (and possibly more clarity).
	#define assign_rel_pos_ptr(rel_pos) \
		if ( !the_sublevel.contains_block_grid_pos \
			(rel_pos##_block_grid_pos) ) \
		{ \
			rel_pos##_ptr = NULL; \
		} \
		else \
		{ \
			rel_pos##_ptr = &the_sublevel.sprite_ipgws_vec_2d \
				.at(rel_pos##_block_grid_pos.y) \
				.at(rel_pos##_block_grid_pos.x); \
		}
	
	list_of_16x32_sprite_pg_stuff_no_origin(assign_rel_pos_ptr)
	#undef assign_rel_pos_ptr
	
}

bool adj_sprite_ipgws_ptr_group_16x32::can_add_sprite()
{
	// Don't let existing sprites be overwritten.
	return !( overlaps_up_left() || overlaps_up() || overlaps_left() 
		|| overlaps_origin() || overlaps_down_left() || overlaps_down() );
}

void adj_sprite_ipgws_ptr_group_16x32::erase_overlapping_sprites()
{
	#define macro_to_erase_overlapping_sprite(rel_pos) \
		if ( overlaps_##rel_pos() ) \
		{ \
			*rel_pos##_ptr = sprite_ipgws(); \
		}
	
	list_of_16x32_sprite_pg_stuff(macro_to_erase_overlapping_sprite);
}



adj_sprite_ipgws_ptr_group_32x32::adj_sprite_ipgws_ptr_group_32x32
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
	
	
	// This doesn't include the origin since it was already assigned.
	// Also, this isn't as efficient as it used to be, with the benefit of
	// fewer lines in this file (and possibly more clarity).
	#define assign_rel_pos_ptr(rel_pos) \
		if ( !the_sublevel.contains_block_grid_pos \
			(rel_pos##_block_grid_pos) ) \
		{ \
			rel_pos##_ptr = NULL; \
		} \
		else \
		{ \
			rel_pos##_ptr = &the_sublevel.sprite_ipgws_vec_2d \
				.at(rel_pos##_block_grid_pos.y) \
				.at(rel_pos##_block_grid_pos.x); \
		}
	
	list_of_32x32_sprite_pg_stuff_no_origin(assign_rel_pos_ptr)
	#undef assign_rel_pos_ptr
	
	
}

bool adj_sprite_ipgws_ptr_group_32x32::can_add_sprite()
{
	// Don't let existing sprites be overwritten.
	return !( overlaps_up_left() || overlaps_up() || overlaps_up_right()
		|| overlaps_left() || overlaps_origin() || overlaps_right()
		|| overlaps_down_left() || overlaps_down() 
		|| overlaps_down_right() );
}

void adj_sprite_ipgws_ptr_group_32x32::erase_overlapping_sprites()
{
	#define macro_to_erase_overlapping_sprite(rel_pos) \
		if ( overlaps_##rel_pos() ) \
		{ \
			*rel_pos##_ptr = sprite_ipgws(); \
		}
	
	list_of_32x32_sprite_pg_stuff(macro_to_erase_overlapping_sprite);
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





