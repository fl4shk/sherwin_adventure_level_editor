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



#include "sprite_type_stuff.hpp"

vector<string> sprite_type_helper::st_name_vec 
= { 
	"st_default", "st_player", "st_waffle", "st_muffin", "st_fire_muffin",
	"st_ice_muffin", "st_chocolate_muffin", "st_door", "st_snow_golem"
};


const string sprite_type_helper::st_unknown_str = string("st_unknown");

//vector<u32> sprite_type_helper::st_height_vec 
//= { 
//	// st_default, st_player, st_waffle, st_muffin
//	16, 32, 16, 16,
//	// st_fire_muffin, st_ice_muffin, st_chocolate_muffin, st_door,
//	16, 16, 16, 32,
//	// st_snow_golem
//	32
//};


