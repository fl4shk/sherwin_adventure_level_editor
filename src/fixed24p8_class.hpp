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



#ifndef fixed24p8_class_hpp
#define fixed24p8_class_hpp


#include "misc_types.hpp"

class fixed24p8
{
public:		// variables and constants
	static const u32 shift = 8;
	static const u32 frac_mask = 0xff;
	
	s32 data;
	
	
public:		// functions
	
	inline fixed24p8()
	{
	}
	
	inline fixed24p8( s32 s_data )
		: data(s_data)
	{
	}
	
	inline fixed24p8( s32 whole_part, u8 frac_part )
	{
		if ( whole_part < 0 )
		{
			data = (-whole_part) << shift;
			data |= frac_part;
			
			data = -data;
		}
		else
		{
			data = whole_part << shift;
			data |= frac_part;
		}
	}
	
	// This function does TRUE rounding
	inline s32 round_to_int() const;
	
	// This function does "regular rounding", also known as simply
	// truncating the faction bits
	inline s32 trunc_to_int() const;
	
	
	inline u8 get_frac_bits() const;
	
	
} __attribute__((aligned(4)));


inline s32 fixed24p8::round_to_int() const
{
	return (s32)( ( data + ( 1 << ( shift - 1 ) ) ) >> shift );
}

inline s32 fixed24p8::trunc_to_int() const
{
	if ( data < 0 )
	{
		return (s32)( ( data + ( 1 << shift ) - 1 ) >> shift );
	}
	else
	{
		return (s32)( data >> shift );
	}
}

inline u8 fixed24p8::get_frac_bits() const
{
	if ( data < 0 )
	{
		u32 temp1 = (u32)(-data);
		return (u8)( temp1 & frac_mask );
	}
	else
	{
		return (u8)( data & frac_mask );
	}
}



#endif		// fixed24p8_class_hpp
