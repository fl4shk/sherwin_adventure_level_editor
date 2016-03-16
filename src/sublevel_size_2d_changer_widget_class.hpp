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


#ifndef sublevel_size_2d_changer_widget_class_hpp
#define sublevel_size_2d_changer_widget_class_hpp

#include "misc_includes.hpp"
#include "level_class.hpp"

// This permits changing the size of a specific sublevel.
class sublevel_size_2d_changer_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	sublevel* the_sublevel;
	
	unique_ptr<QGridLayout> grid_layout;
	
	unique_ptr<QLabel> width_label, height_label;
	
	unique_ptr<QSpinBox> sublevel_width_spinbox, sublevel_height_spinbox;
	
	
public:		// functions
	sublevel_size_2d_changer_widget( QWidget* s_parent, 
		sublevel* s_the_sublevel );
	
	
protected:		// functions
	
	
protected slots:		// slots
	void sublevel_width_changed( int n_width );
	void sublevel_height_changed( int n_height );
	
};



#endif		// sublevel_size_2d_changer_widget_class_hpp
