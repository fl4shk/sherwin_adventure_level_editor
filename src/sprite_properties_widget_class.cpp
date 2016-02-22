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


#include "sprite_properties_widget_class.hpp"



sprite_properties_widget::sprite_properties_widget( QWidget* s_parent )
	: QWidget(s_parent)
{
	//grid_layout = new QGridLayout(NULL);
	//grid_layout = new QGridLayout;
	grid_layout.reset(new QGridLayout);
	
	generate_group_box();
	
	setLayout(&(*grid_layout));
}


void sprite_properties_widget::generate_group_box()
{
	//group_box = new QGroupBox( tr("HOLY CRAP IT'S A QGroupBox!") );
	group_box.reset(new QGroupBox( tr("HOLY CRAP IT'S A QGroupBox!") ));
	
	grid_layout->addWidget( &(*group_box), 0, 0 );
	
}


