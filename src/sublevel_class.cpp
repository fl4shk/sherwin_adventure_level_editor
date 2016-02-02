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



#include "sublevel_class.hpp"

const vec2_u32 sublevel::max_size_2d = { 512, 32 };
const u32 sublevel::max_num_blocks = max_size_2d.x * max_size_2d.y;

//sublevel::sublevel()
//{
//	init(max_size_2d);
//}


sublevel::sublevel( const vec2_u32& s_size_2d )
	: size_2d(s_size_2d)
{
	for ( u32 j=0; j<size_2d.y; ++j )
	{
		uncompressed_block_data_vec.push_back( vector<block>() );
		
		for ( u32 i=0; i<size_2d.x; ++i )
		{
			uncompressed_block_data_vec[j].push_back( block() );
		}
	}
}

void sublevel::init( const vec2_u32& s_size_2d )
{
	size_2d = s_size_2d;
	
	for ( u32 j=0; j<size_2d.y; ++j )
	{
		uncompressed_block_data_vec.push_back( vector<block>() );
		
		for ( u32 i=0; i<size_2d.x; ++i )
		{
			uncompressed_block_data_vec[j].push_back( block() );
		}
	}
}


