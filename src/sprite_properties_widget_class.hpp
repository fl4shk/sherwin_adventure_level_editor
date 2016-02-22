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
	
	unique_ptr<QGridLayout> grid_layout;
	
	sprite_init_param_group_with_size* selected_sprite_ipgws;
	
	unique_ptr<QLabel> test_label;
	
public:		// functions
	sprite_properties_widget( QWidget* s_parent,
		sprite_init_param_group_with_size* s_selected_sprite_ipgws );
	
	
protected:		// functions
	
	void generate_group_box();
	
	// "subfunctions" for use by generate_group_box()
	
	// The Default Sprite (also used for when there is no sprite in the
	// slot).
	void generate_group_box_for_st_default();
	
	// The Player
	void generate_group_box_for_st_player();
	
	// Powerup Sprites
	void generate_group_box_for_st_waffle();
	
	void generate_group_box_for_st_muffin();
	void generate_group_box_for_st_fire_muffin();
	void generate_group_box_for_st_ice_muffin();
	void generate_group_box_for_st_chocolate_muffin();
	
	// Door sprites
	
	// st_door is used for warping around different parts of either
	// a SINGLE sublevel or MULTIPLE sublevels.  These get connected to
	// sublevel_entrance's during the whole level conversion process.
	void generate_group_box_for_st_door();
	
	
	// Enemy sprites
	void generate_group_box_for_st_snow_golem();
	
	
};


#endif		// sprite_properties_widget_class_hpp
