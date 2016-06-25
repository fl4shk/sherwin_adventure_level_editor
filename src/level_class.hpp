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



#ifndef level_class_hpp
#define level_class_hpp

#include "sublevel_class.hpp"


class level
{
public:		// constants
	// This is 
	static constexpr u32 max_num_sublevels = 8;
	
public:		// variables
	
	vector<sublevel> sublevel_vec;
	//u32 curr_sublevel_index;
	
	
public:		// functions
	inline level() : level(max_num_sublevels)
	{
	}
	
	inline level( u32 s_num_sublevels ) 
		: level( s_num_sublevels, sublevel::max_size_2d )
	{
	}
	
	level( u32 s_num_sublevels, const vec2_u32& shared_sublevel_size_2d );
	
	level( const vector<vec2_u32>& s_sublevel_size_2d_vec );
	
	//inline sublevel& get_curr_sublevel()
	//{
	//	return sublevel_vec.at(curr_sublevel_index);
	//}
	
	
	void generate_compressed_block_data_vectors
		( const string& temp_output_dirname,
		const string& temp_output_basename );
	
	void generate_sprite_ipgws_and_sle_stuff_for_exporting();
	
};



#endif		// level_class_hpp
