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

#include "level_class.hpp"


sprite_properties_widget::sprite_properties_widget( QWidget* s_parent,
	sprite_init_param_group_with_size* s_selected_sprite_ipgws )
	: QWidget(s_parent), selected_sprite_ipgws(s_selected_sprite_ipgws)
{
	init();
}


void sprite_properties_widget::init()
{
	grid_layout.reset(new QGridLayout);
	setLayout(grid_layout.get());
	
	//group_box.reset(new QGroupBox( tr("HOLY CRAP IT'S A QGroupBox!") ));
	
	group_box.reset(new QGroupBox( tr
		( sprite_type_helper::get_st_name_debug
		(selected_sprite_ipgws->type).c_str() ) ));
	
	group_box_grid_layout.reset(new QGridLayout);
	group_box->setLayout(group_box_grid_layout.get());
	
	facing_right_checkbox.reset(new QCheckBox(QString("Facing Right")));
	
	if (selected_sprite_ipgws->facing_right)
	{
		facing_right_checkbox->setCheckState(Qt::Checked);
	}
	
	connect( facing_right_checkbox.get(), &QCheckBox::stateChanged, this,
		&sprite_properties_widget
		::facing_right_checkbox_response_to_state_changed );
	
	
	group_box_grid_layout->addWidget( facing_right_checkbox.get(),
		group_box_grid_layout->rowCount(), 0 );
	
	
	switch (selected_sprite_ipgws->type)
	{
		// The Default Sprite (also used for when there is no sprite in the
		// slot).
		case st_default:
			generate_stuff_for_st_default();
			break;
		
		// The Player
		case st_player:
			generate_stuff_for_st_player();
			break;
		
		// Powerup Sprites
		case st_waffle:
			generate_stuff_for_st_waffle();
			break;
		
		case st_muffin:
			generate_stuff_for_st_muffin();
			break;
		case st_fire_muffin:
			generate_stuff_for_st_fire_muffin();
			break;
		case st_ice_muffin:
			generate_stuff_for_st_ice_muffin();
			break;
		case st_chocolate_muffin:
			generate_stuff_for_st_chocolate_muffin();
			break;
		
		// Door sprites
		
		// st_door is used for warping around different parts of either
		// a SINGLE sublevel or MULTIPLE sublevels.  These get connected to
		// sublevel_entrance's during the whole level conversion process.
		case st_door:
			generate_stuff_for_st_door();
			break;
		
		
		// Enemy sprites
		case st_snow_golem:
			generate_stuff_for_st_snow_golem();
			break;
		
		default:
			cout << "Message from sprite_properties_widget:  unknown "
				<< "sprite_type!\n";
			break;
	}
	
	
	grid_layout->addWidget( group_box.get(), 0, 0 );
	
}



// "subfunctions" for use by generate_stuff()

// The Default Sprite (also used for when there is no sprite in the
// slot).
void sprite_properties_widget::generate_stuff_for_st_default()
{
	
}

// The Player
void sprite_properties_widget::generate_stuff_for_st_player()
{
	// Testing stuff
	group_box_label_vec.push_back
		(unique_ptr<QLabel>(new QLabel("You selected a player sprite!")));
	group_box_grid_layout->addWidget( group_box_label_vec.at(0).get(),
		group_box_grid_layout->rowCount(), 0 );
}

// Powerup Sprites
void sprite_properties_widget::generate_stuff_for_st_waffle()
{
	
}

void sprite_properties_widget::generate_stuff_for_st_muffin()
{
	
}
void sprite_properties_widget::generate_stuff_for_st_fire_muffin()
{
	
}
void sprite_properties_widget::generate_stuff_for_st_ice_muffin()
{
	
}
void sprite_properties_widget::generate_stuff_for_st_chocolate_muffin()
{
	
}

// Door sprites

// st_door is used for warping around different parts of either
// a SINGLE sublevel or MULTIPLE sublevels.  These get connected to
// sublevel_entrance's during the whole level conversion process.
void sprite_properties_widget::generate_stuff_for_st_door()
{
	const int initial_row_count = group_box_grid_layout->rowCount();
	
	group_box_label_vec.clear();
	group_box_spinbox_vec.clear();
	
	
	// Destination Sublevel Entrance Index (extra_param_0)
	group_box_label_vec.push_back(unique_ptr<QLabel>(new QLabel
		("<small>Dest. <b>Sublevel Entrance</b> Index</small>:  ")));
	group_box_label_vec.at(0)->setTextFormat(Qt::RichText);
	group_box_grid_layout->addWidget( group_box_label_vec.at(0).get(),
		initial_row_count, 0 );
	
	group_box_spinbox_vec.push_back(unique_ptr<QSpinBox>(new QSpinBox));
	group_box_spinbox_vec.at(0)->setRange( 0, 
		level::max_num_sublevels - 1 );
	group_box_spinbox_vec.at(0)->setValue
		(selected_sprite_ipgws->extra_param_0);
	
	group_box_grid_layout->addWidget( group_box_spinbox_vec.at(0).get(),
		initial_row_count, 1 );
	
	//connect( group_box_spinbox_vec.at(0).get(),
	//	static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	//	this, &sprite_properties_widget
	//	::door_sprite_destination_sublevel_entrance_index_changed );
	connect( group_box_spinbox_vec.at(0).get(),
		(void (QSpinBox::*)(int))(&QSpinBox::valueChanged),
		this, &sprite_properties_widget
		::door_sprite_destination_sublevel_entrance_index_changed );
	
	
	
	
	// Destination Sublevel Index (extra_param_1).  At some point, this
	// COULD be made to default to whatever the current sublevel is, since
	// multi-sublevel editing IS now a thing.
	group_box_label_vec.push_back(unique_ptr<QLabel>(new QLabel
		("<small>Dest. <b>Sublevel</b> Index</small>:  ")));
	group_box_label_vec.at(1)->setTextFormat(Qt::RichText);
	group_box_grid_layout->addWidget( group_box_label_vec.at(1).get(),
		initial_row_count + 1, 0 );
	
	group_box_spinbox_vec.push_back(unique_ptr<QSpinBox>(new QSpinBox));
	group_box_spinbox_vec.at(1)->setRange( 0,
		sublevel::max_num_door_sublevel_entrances - 1 );
	group_box_spinbox_vec.at(1)->setValue
		(selected_sprite_ipgws->extra_param_1);
	
	group_box_grid_layout->addWidget( group_box_spinbox_vec.at(1).get(),
		initial_row_count + 1, 1 );
	
	//connect( group_box_spinbox_vec.at(1).get(), 
	//	static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	//	this, &sprite_properties_widget
	//	::door_sprite_destination_sublevel_index_changed );
	connect( group_box_spinbox_vec.at(1).get(), 
		(void (QSpinBox::*)(int))(&QSpinBox::valueChanged),
		this, &sprite_properties_widget
		::door_sprite_destination_sublevel_index_changed );
	
	
	
	// Door Number (extra_param_2)
	group_box_label_vec.push_back(unique_ptr<QLabel>(new QLabel
		("<small>Door Number</small>:  ")));
	group_box_label_vec.at(1)->setTextFormat(Qt::RichText);
	group_box_grid_layout->addWidget( group_box_label_vec.at(2).get(),
		initial_row_count + 2, 0 );
	
	group_box_spinbox_vec.push_back(unique_ptr<QSpinBox>(new QSpinBox));
	group_box_spinbox_vec.at(2)->setRange( 0,
		sublevel::max_num_door_sublevel_entrances - 1 );
	group_box_spinbox_vec.at(2)->setValue
		(selected_sprite_ipgws->extra_param_2);
	
	group_box_grid_layout->addWidget( group_box_spinbox_vec.at(2).get(),
		initial_row_count + 2, 1 );
	
	//connect( group_box_spinbox_vec.at(2).get(), 
	//	static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	//	this, &sprite_properties_widget::door_sprite_door_number_changed );
	connect( group_box_spinbox_vec.at(2).get(), 
		(void (QSpinBox::*)(int))(&QSpinBox::valueChanged),
		this, &sprite_properties_widget::door_sprite_door_number_changed );
}


// Enemy sprites
void sprite_properties_widget::generate_stuff_for_st_snow_golem()
{
	
}



// slots
void sprite_properties_widget
	::facing_right_checkbox_response_to_state_changed( int n_state )
{
	if (n_state)
	{
		selected_sprite_ipgws->facing_right = true;
	}
	else //if (!n_state)
	{
		selected_sprite_ipgws->facing_right = false;
	}
}



// st_door stuff
// extra_param_0
void sprite_properties_widget
	::door_sprite_destination_sublevel_entrance_index_changed
	( int n_sublevel_entrance_index )
{
	selected_sprite_ipgws->extra_param_0 = n_sublevel_entrance_index;
}

// extra_param_1
void sprite_properties_widget
	::door_sprite_destination_sublevel_index_changed
	( int n_sublevel_index )
{
	selected_sprite_ipgws->extra_param_1 = n_sublevel_index;
}

// extra_param_2
void sprite_properties_widget::door_sprite_door_number_changed
	( int n_door_number )
{
	selected_sprite_ipgws->extra_param_2 = n_door_number;
}



