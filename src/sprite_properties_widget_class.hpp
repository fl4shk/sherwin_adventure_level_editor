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


#ifndef sprite_properties_widget_class_hpp
#define sprite_properties_widget_class_hpp

#include "misc_includes.hpp"


class sprite_properties_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	unique_ptr<QGroupBox> group_box;
	unique_ptr<QGridLayout> group_box_grid_layout;
	
	unique_ptr<QGridLayout> grid_layout;
	
public:		// functions
	sprite_properties_widget( QWidget* s_parent );
	//~sprite_properties_widget();
	
	
protected:		// functions
	
	void generate_group_box();
	
};


#endif		// sprite_properties_widget_class_hpp
