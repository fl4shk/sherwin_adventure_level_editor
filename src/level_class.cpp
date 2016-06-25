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


void level::generate_compressed_block_data_vectors
	( const string& temp_output_dirname, 
	const string& temp_output_basename )
{
	// Run gbalzss
	const string gbalzss_command = "gbalzss e "
		+ block_stuff::get_uncompressed_block_data_file_name
		( temp_output_dirname, temp_output_basename ) + " "
		+ block_stuff::get_compressed_block_data_file_name
		( temp_output_dirname, temp_output_basename );
	
	// Remove the temporary files
	const string clean_up_command = "rm " 
		+ block_stuff::get_uncompressed_block_data_file_name
		( temp_output_dirname, temp_output_basename ) + " "
		+ block_stuff::get_compressed_block_data_file_name
		( temp_output_dirname, temp_output_basename );
	
	
	for ( sublevel& the_sublevel : sublevel_vec )
	{
		the_sublevel.write_uncompressed_block_data_to_file
			( temp_output_dirname, temp_output_basename );
		
		cout << "Running this command:  " << gbalzss_command << endl;
		if ( system(gbalzss_command.c_str()) != 0 )
		{
			cout << "There was an error running gbalzss!  Exiting....\n";
			exit(1);
		}
		
		the_sublevel.read_compressed_block_data_from_file
			( temp_output_dirname, temp_output_basename );
		
		cout << "Running this command:  " << clean_up_command << endl;
		system(clean_up_command.c_str());
	}
}


void level::generate_sprite_ipgws_and_sle_stuff_for_exporting()
{
	// Sprites that are tied to a sublevel_entrance
	sprite_ipgws the_player_at_starting_position;
	vector<sprite_ipgws> sprites_tied_to_sle_vec;
	
	bool found_player = false;
	
	
	auto most_inner_loop_contents = [&]( u32 k, sublevel& the_sublevel, 
		const sprite_ipgws& the_sprite_ipgws ) -> void
	{
		if ( the_sprite_ipgws.type == st_default )
		{
			return;
		}
		
		if ( the_sprite_ipgws.type == st_player )
		{
			// Only use the first st_player sprite that was found.
			// This check could be avoided by either making it so that
			// only one st_player sprite could be placed in a level, or
			// just not allowing placing st_player, instead having a
			// tool for picking the position of the start of a level,
			// which is in sublevel 0.
			if ( k == 0 && !found_player )
			{
				found_player = true;
				
				the_player_at_starting_position = the_sprite_ipgws;
			}
			else if ( k == 0 && found_player )
			{
				cout << "Warning:  Multiple starting points found in "
					<< "sublevel 0!\n";
				the_sprite_ipgws.show_rejection_message();
			}
			else if ( k != 0 )
			{
				cout << "Warning:  Level starting positions must be in "
					<< "sublevel 0!\n";
				the_sprite_ipgws.show_rejection_message();
			}
		}
		else if ( the_sprite_ipgws.type == st_door )
		{
			the_sublevel.sprite_ipgws_vec_for_exporting.push_back
				(the_sprite_ipgws);
		}
		else
		{
			the_sublevel.sprite_ipgws_vec_for_exporting.push_back
				(the_sprite_ipgws);
		}
	};
	
	for ( u32 k=0; k<sublevel_vec.size(); ++k )
	{
		sublevel& the_sublevel = sublevel_vec.at(k);
		
		the_sublevel.sprite_ipgws_vec_for_exporting.clear();
		the_sublevel.sublevel_entrance_vec.clear();
		
		for ( u32 j=0; j<the_sublevel.real_size_2d.y; ++j )
		{
			for ( u32 i=0; i<the_sublevel.real_size_2d.x; ++i )
			{
				most_inner_loop_contents( k, the_sublevel, the_sublevel
					.sprite_ipgws_vec_2d.at(j).at(i) );
			}
		}
	}
}



