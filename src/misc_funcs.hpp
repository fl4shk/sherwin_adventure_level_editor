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


#ifndef misc_funcs_hpp
#define misc_funcs_hpp

#include "misc_includes.hpp"

inline void show_quit_message()
{
	cout << "Exiting....\n";
}

inline void quit_non_slot( int return_val = 0 )
{
	show_quit_message();
	qApp->quit();
	exit(return_val);
}

template< typename type >
inline type convert_range( type val_old, type min_old, type max_old, 
	type min_new, type max_new )
{
	type ret;
	
	type range_old = max_old - min_old;
	type range_new = max_new - min_new;
	
	if ( range_old == 0 )
	{
		return min_new;
	}
	
	ret = ( ( ( val_old - min_old ) * range_new ) / range_old ) + min_new;
	
	return ret;
}


// insert_or_assign() from C++17 would be helpful here
template< typename key, typename type >
inline type& get_or_create_umap_value
	( unordered_map< key, type >& the_umap, const key& the_key )
{
	auto iter = the_umap.find(the_key);
	
	if ( iter == the_umap.end() )
	{
		the_umap[the_key] = type();
	}
	
	return the_umap[the_key];
}


#endif		// misc_funcs_hpp
