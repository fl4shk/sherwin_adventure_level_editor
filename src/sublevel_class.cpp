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


//sublevel::sublevel( const vec2_u32& s_size_2d )
//	: size_2d(s_size_2d)
//{
//	for ( u32 j=0; j<size_2d.y; ++j )
//	{
//		uncompressed_block_data_vec.push_back( vector<block>() );
//		
//		for ( u32 i=0; i<size_2d.x; ++i )
//		{
//			uncompressed_block_data_vec[j].push_back( block() );
//		}
//	}
//}

sublevel::sublevel( const vec2_u32& s_size_2d )
{
	init(s_size_2d);
}

void sublevel::init( const vec2_u32& s_size_2d )
{
	size_2d = s_size_2d;
	
	for ( u32 j=0; j<size_2d.y; ++j )
	{
		uncompressed_block_data_vec_2d.push_back( vector<block>() );
		sprite_ipg_vec_2d.push_back( vector<sprite_init_param_group>() );
		
		for ( u32 i=0; i<size_2d.x; ++i )
		{
			uncompressed_block_data_vec_2d[j].push_back( block() );
			sprite_ipg_vec_2d[j].push_back( sprite_init_param_group() );
		}
	}
}

void sublevel::generate_compressed_block_data_vec
	( const string& output_dirname, const string& output_basename )
{
	write_uncompressed_block_data_to_file( output_dirname, 
		output_basename );
	
	// Run gbalzss
	const string gbalzss_command = "gbalzss e "
		+ block_stuff::get_uncompressed_block_data_file_name
		( output_dirname, output_basename ) + " "
		+ block_stuff::get_compressed_block_data_file_name( output_dirname,
			output_basename );
	
	cout << "Running this command:  " << gbalzss_command << endl;
	if ( system(gbalzss_command.c_str()) != 0 )
	{
		cout << "There was an error running gbalzss!  Exiting....\n";
		exit(1);
	}
	
	read_compressed_block_data_from_file( output_dirname, output_basename );
	
	// Remove the temporary files
	const string clean_up_command = "rm " 
		+ block_stuff::get_uncompressed_block_data_file_name
		( output_dirname, output_basename ) + " "
		+ block_stuff::get_compressed_block_data_file_name( output_dirname,
			output_basename );
	
	cout << "Running this command:  " << clean_up_command << endl;
	system(clean_up_command.c_str());
}

void sublevel::write_uncompressed_block_data_to_file
	( const string& output_dirname, const string& output_basename )
{
	string output_file_name 
		= block_stuff::get_uncompressed_block_data_file_name
		( output_dirname, output_basename );
	//cout << output_file_name << endl;
	
	fstream output_file( output_file_name, ios_base::out 
		| ios_base::binary );
	
	if ( !output_file.is_open() )
	{
		cout << "There was an error opening \"" << output_file_name 
			<< "\"!\n";
		
		exit(1);
	}
	
	for ( u32 j=0; j<uncompressed_block_data_vec.size(); ++j )
	{
		vector<block>& bd_col = uncompressed_block_data_vec[j];
		
		for ( u32 i=0; i<bd_col.size(); ++i )
		{
			output_file.write( reinterpret_cast<char*>(&bd_col[i]), 
				sizeof(bd_col[i]) );
		}
	}
	
	output_file.close();
}


void sublevel::read_compressed_block_data_from_file
	( const string& output_dirname, const string& output_basename )
{
	string input_file_name 
		= block_stuff::get_compressed_block_data_file_name( output_dirname, 
		output_basename );
	
	fstream input_file( input_file_name, ios_base::in | ios_base::binary );
	
	if ( !input_file.is_open() )
	{
		cout << "There was an error opening \"" << input_file_name
			<< "\"!\n";
		
		exit(1);
	}
	
	vector<u8> compressed_block_data_as_u8s;
	
	while ( !input_file.eof() )
	{
		int get_result = input_file.get();
		
		if ( get_result == -1 )
		{
			break;
		}
		
		u8 byte = get_result;
		compressed_block_data_as_u8s.push_back(byte);
	}
	
	input_file.close();
	
	if ( compressed_block_data_as_u8s.size() % 4 != 0 )
	{
		cout << "Error:  compressed_block_data_as_u8s.size() isn't "
			<< "divisible by 4!\n";
		exit(1);
	}
	
	// Construct the std::vector<u32> of compressed block data
	for ( u32 i=0; i<compressed_block_data_as_u8s.size(); i+=4 )
	{
		u32 the_u32 = ( compressed_block_data_as_u8s.at( i + 3 ) << 24 )
			| ( compressed_block_data_as_u8s.at( i + 2 ) << 16 )
			| ( compressed_block_data_as_u8s.at( i + 1 ) << 8 )
			| compressed_block_data_as_u8s.at( i + 0 );
		
		compressed_block_data_vec.push_back(the_u32);
	}
	
}


