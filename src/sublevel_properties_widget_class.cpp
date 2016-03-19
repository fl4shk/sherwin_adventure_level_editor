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


#include "sublevel_properties_widget_class.hpp"


sublevel_properties_widget::sublevel_properties_widget( QWidget* s_parent, 
	sublevel* s_the_sublevel ) : QDialog(s_parent),
	the_sublevel(s_the_sublevel)
{
	setModal(true);
	//setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
	//setMinimumSize( QSize( 100, 100 ) );
	//setMaximumSize( QSize( 100, 100 ) );
	
	setWindowTitle(QString("Sublevel Properties"));
	
	
	grid_layout.reset(new QGridLayout);
	setLayout(grid_layout.get());
	
	
	width_label.reset(new QLabel("<b>Width</b>"));
	height_label.reset(new QLabel("<b>Height</b>"));
	
	grid_layout->addWidget( width_label.get(), 0, 0 );
	grid_layout->addWidget( height_label.get(), 0, 1 );
	
	
	sublevel_width_spinbox.reset(new QSpinBox);
	sublevel_width_spinbox->setRange( 1, sublevel::max_size_2d.x );
	sublevel_width_spinbox->setValue(the_sublevel->real_size_2d.x);
	
	sublevel_height_spinbox.reset(new QSpinBox);
	sublevel_height_spinbox->setRange( 1, sublevel::max_size_2d.y );
	sublevel_height_spinbox->setValue(the_sublevel->real_size_2d.y);
	
	grid_layout->addWidget( sublevel_width_spinbox.get(), 1, 0 );
	grid_layout->addWidget( sublevel_height_spinbox.get(), 1, 1 );
	
	
	//connect( sublevel_width_spinbox.get(), 
	//	static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	//	this, &sublevel_properties_widget::sublevel_width_changed );
	//
	//connect( sublevel_height_spinbox.get(), 
	//	static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	//	this, &sublevel_properties_widget::sublevel_height_changed );
	
	
	okay_button.reset(new QPushButton("&Okay"));
	cancel_button.reset(new QPushButton("&Cancel")); 
	grid_layout->addWidget( okay_button.get(), 2, 0 );
	grid_layout->addWidget( cancel_button.get(), 2, 1 );
	
	connect( okay_button.get(), &QPushButton::clicked, this,
		&sublevel_properties_widget::accept );
	connect( cancel_button.get(), &QPushButton::clicked, this,
		&sublevel_properties_widget::reject );
}


void sublevel_properties_widget::accept()
{
	//cout << "accept()\n";
	
	//cout << "width:  " << sublevel_width_spinbox->value() << endl;
	//cout << "height:  " << sublevel_height_spinbox->value() << endl;
	
	the_sublevel->resize(vec2_u32( sublevel_width_spinbox->value(), 
		sublevel_height_spinbox->value() ));
	
	
	//cout << "done(1) now.\n";
	done(1);
}

void sublevel_properties_widget::reject()
{
	//cout << "reject()\n";
	
	//cout << "done(0) now.\n";
	done(0);
}


