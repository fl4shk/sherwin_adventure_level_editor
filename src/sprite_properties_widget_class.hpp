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
#include "sprite_level_data_stuff.hpp"


class sprite_properties_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	unique_ptr<QGroupBox> group_box;
	unique_ptr<QGridLayout> group_box_grid_layout;
	
	
	// A QGridLayout for the sprite_properties_widget itself.
	unique_ptr<QGridLayout> grid_layout;
	
	// A pointer to the sprite level data instance.
	sprite_ipgws* selected_sprite_ipgws;
	
	
	// This is used for picking the facing_right value of
	// selected_sprite_ipgws.
	unique_ptr<QCheckBox> facing_right_checkbox;
	
	
	// These are used so that the QSpinBox'es can have names.
	vector< unique_ptr<QLabel> > group_box_label_vec;
	
	// These are used for some sprite_type's for changing the values of
	// extra_param_0 to extra_param_3.
	// For st_door, the first type of sprite to need this, extra_param_0
	// extra_param_1, and extra_param_2 are the only extra parameters that
	// should be possible to change.
	vector< unique_ptr<QSpinBox> > group_box_spinbox_vec;
	
	
	
	
public:		// functions
	sprite_properties_widget( QWidget* s_parent,
		sprite_ipgws* s_selected_sprite_ipgws );
	
	
protected:		// functions
	
	// A copy constructor... maybe.
	void init();
	
	
	
	
	// "subfunctions" for use by init()
	
	// The Default Sprite (also used for when there is no sprite in the
	// slot).
	void generate_stuff_for_st_default();
	
	// The Player
	void generate_stuff_for_st_player();
	
	// Powerup Sprites
	void generate_stuff_for_st_waffle();
	
	void generate_stuff_for_st_muffin();
	void generate_stuff_for_st_fire_muffin();
	void generate_stuff_for_st_ice_muffin();
	void generate_stuff_for_st_chocolate_muffin();
	
	// Door sprites
	
	// st_door is used for warping around different parts of either
	// a SINGLE sublevel or MULTIPLE sublevels.  These get connected to
	// sublevel_entrance's during the whole level conversion process.
	void generate_stuff_for_st_door();
	
	
	// Enemy sprites
	void generate_stuff_for_st_snow_golem();
	
	
protected slots:		// slots
	void facing_right_checkbox_response_to_state_changed( int n_state );
	
	
	// st_door stuff
	// extra_param_0
	void door_sprite_destination_sublevel_entrance_index_changed
		( int n_sublevel_entrance_index );
	
	// extra_param_1
	void door_sprite_destination_sublevel_index_changed
		( int n_sublevel_index );
	
	// extra_param_2
	void door_sprite_door_number_changed( int n_door_number );
	
};


#endif		// sprite_properties_widget_class_hpp
