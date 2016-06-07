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




#include "misc_includes.hpp"
#include "primary_widget_class.hpp"



int main( int argc, char** argv )
{
	vector<string> argv_copy;
	argv_copy.clear();
	
	for ( int i=0; i<argc; ++i )
	{
		argv_copy.push_back(string(argv[i]));
	}
	
	if ( argv_copy.size() != 1 && argv_copy.size() != 3 )
	{
		cout << "Usage 1:  " << argv_copy.at(0) << "\n";
		cout << "Usage 2:  " << argv_copy.at(0) << " level_file_name "
			<< "output_prefix\n";
		return 1;
	}
	
	QApplication app( argc, argv );
	
	primary_widget window(argv_copy);
	window.show();
	
	return app.exec();
}
