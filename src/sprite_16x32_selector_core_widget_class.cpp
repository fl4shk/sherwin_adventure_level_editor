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


#include "sprite_16x32_selector_core_widget_class.hpp"

const string sprite_16x32_selector_core_widget::level_element_gfx_file_name 
	= "gfx/the_16x32_sprite_gfx.png";


sprite_16x32_selector_core_widget::sprite_16x32_selector_core_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: level_element_selector_core_widget_base( s_parent, s_position, 
	s_size, get_level_element_gfx_file_name(), get_slot_inner_width(),
	get_slot_inner_height(), get_slot_outer_width(),
	get_slot_outer_height() )
{
}


