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



#ifndef sublevel_class_hpp
#define sublevel_class_hpp

#include "array_2d_helper_class.hpp"
#include "sprite_level_data_stuff.hpp"
#include "block_stuff.hpp"

#include "sublevel_entrance_class.hpp"


#include <vector>
#include <string>
#include <fstream>
using namespace std;


//// Here is a reference class for sublevels as they appear in the GBA-side
////code.
//template< u32 compressed_block_data_size, u32 xsize, u32 ysize, 
//	u32 sprite_ipg_arr_size, u32 sublevel_entrance_arr_size >
//class sublevel
//{
//public:		// variables
//	const u32 compressed_block_data[compressed_block_data_size];
//	const sprite_init_param_group sprite_ipg_arr[sprite_ipg_arr_size];
//	const sublevel_entrance sublevel_entrance_arr[sublevel_entrance_arr_size];
//	
//} __attribute__((aligned(4)));




class sublevel
{
public:		// constants
	//static constexpr u32 lowest_warp_id = 0, highest_warp_id = 51;
	
	// This is limited entirely by the number of graphics that have been
	// created that can be displayed to show which entrance is attached to
	// which game entity.
	static constexpr u32 max_num_door_sublevel_entrances = 52;
	
	static const vec2_u32 max_size_2d;
	static const u32 max_num_blocks;
	
public:		// variables
	
	// size_2d is generally set to max_size_2d
	vec2_u32 size_2d;
	
	//// real_size_2d is set to the size_2d of the primary Tiled layer.
	vec2_u32 real_size_2d;
	
	
	// The std::vector of std::vector<block>'s used as a dynamically
	// allocated 2D array of UNCOMPRESSED block data.  However, this vector
	// of vectors is initialized to the SAME SIZE (both for the outer
	// vector and the inner vectors) every time the program is run!  This
	// is so that, IN THE GBA-SIDE CODE, the COMPRESSED array of block data
	// can be DECOMPRESSED DIRECTLY to the statically allocated 2D array of
	// block data.
	// Also note that ROWS are stored in the INNER VECTOR!  That means that
	// this vector of vectors should be indexed as [y][x]!
	vector< vector<block> > uncompressed_block_data_vec_2d;
	
	// On the other hand, compressed_block_data_vec can have a variable
	// size.
	vector<u32> compressed_block_data_vec;
	
	
	// The size of sprite_ipg_with_size_vec_2d is set to the maximum size 
	// of a level just in case, though it isn't really necessary in this
	// case.
	// Also note that ROWS are stored in the INNER VECTOR!  That means that
	// this vector of vectors should be indexed as [y][x]!
	vector< vector<sprite_init_param_group_with_size> > 
		sprite_ipgws_vec_2d;
	
	// sprite_ipg_vec can also have a variable size.
	vector<sprite_init_param_group> sprite_ipg_vec;
	
	// Additionally, sublevel_entrance_vec can have a variable size.
	vector<sublevel_entrance> sublevel_entrance_vec;
	
public:		// functions
	inline sublevel() : sublevel(max_size_2d)
	{
	}
	
	sublevel( const vec2_u32& s_size_2d );
	
	void init( const vec2_u32& s_size_2d );
	
	void generate_compressed_block_data_vec( const string& output_dirname,
		const string& output_basename );
	
	
protected:		// functions
	void write_uncompressed_block_data_to_file
		( const string& output_dirname, const string& output_basename );
	
	void read_compressed_block_data_from_file
		( const string& output_dirname, const string& output_basename );
};



#endif		// sublevel_class_hpp
