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
	
	group_box_vbox_layout.reset(new QVBoxLayout);
	group_box->setLayout(group_box_vbox_layout.get());
	
	facing_right_checkbox.reset(new QCheckBox( QString("Facing Right") ));
	
	connect( facing_right_checkbox.get(), &QCheckBox::stateChanged, this,
		&sprite_properties_widget
		::facing_right_checkbox_response_to_state_changed );
	group_box_vbox_layout->addWidget(facing_right_checkbox.get());
	
	
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
	//test_label.reset(new QLabel( "You selected a player sprite!", this ));
	test_label.reset(new QLabel("You selected a player sprite!"));
	group_box_vbox_layout->addWidget(test_label.get());
	
	
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


