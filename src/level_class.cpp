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


#include "level_class.hpp"


level::level( u32 s_num_sublevels, 
	const vec2_u32& shared_sublevel_size_2d )
	: curr_sublevel_index(0)
{
	if ( s_num_sublevels > max_num_sublevels )
	{
		cout << "Weird bug:  s_num_sublevels > max_num_sublevels.  "
			<< "Specific value is " << s_num_sublevels << ".\n";
		for ( u32 i=0; i<max_num_sublevels; ++i )
		{
			sublevel_vec.push_back(sublevel());
		}
	}
	else
	{
		for ( u32 i=0; i<s_num_sublevels; ++i )
		{
			sublevel_vec.push_back(sublevel());
		}
	}
}

level::level( const vector<vec2_u32>& s_sublevel_size_2d_vec )
	: curr_sublevel_index(0)
{
	if ( s_sublevel_size_2d_vec.size() > max_num_sublevels )
	{
		cout << "Weird bug:  s_sublevel_size_2d_vec.size() > "
			<< "max_num_sublevels.  Specific value is " 
			<< s_sublevel_size_2d_vec.size() << ".\n";
		
		for ( u32 i=0; i<max_num_sublevels; ++i )
		{
			sublevel_vec.push_back(sublevel(s_sublevel_size_2d_vec.at(i)));
		}
	}
	else
	{
		for ( u32 i=0; i<s_sublevel_size_2d_vec.size(); ++i )
		{
			sublevel_vec.push_back(sublevel(s_sublevel_size_2d_vec.at(i)));
		}
	}
}


