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



#ifndef misc_types_hpp
#define misc_types_hpp

#include <stdint.h>

typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
typedef uint64_t u64; typedef int64_t s64;
typedef unsigned int uint;


// This corresponds to an active tool.
enum mouse_mode 
{
	// Place level elements.  Blocks can be erased by placing bt_air.  For
	// this reason, there isn't a tool for erasing blocks.  At some point,
	// perhaps the block graphics file should have "dummy" graphics instead
	// of just looking like bt_air.
	mm_place_level_elements,
	
	// Erase sprites in particular
	mm_erase_sprites, 
	
	// Select a single sprite to allow changing its properties
	mm_select_single_sprite, 
	
	// Rectangular selection 
	mm_rect_select,
};


// This is intended to be used for the purposes 
enum rect_selection_layer { rsl_blocks, rsl_sprites };




#endif		// misc_types_hpp
