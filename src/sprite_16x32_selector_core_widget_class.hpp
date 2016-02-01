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



#ifndef sprite_16x32_selector_core_widget_class_hpp
#define sprite_16x32_selector_core_widget_class_hpp

//#include "sfml_canvas_widget_classes.hpp"
//#include "sprite_16x32_selector_widget_class.hpp"
//#include "sprite_16x32_stuff.hpp"

#include "level_element_selector_core_widget_base_class.hpp" 
//#include "sprite_16x32_stuff.hpp"

class sprite_16x32_selector_core_widget 
	: public level_element_selector_core_widget_base
{
public:		// constants
	static constexpr u32 slot_inner_width = 16, slot_inner_height = 32, 
		slot_outer_width = slot_inner_width + 2, 
		slot_outer_height = slot_inner_height + 2;
	
	static const string level_element_gfx_file_name;
	
	
public:		// functions
	sprite_16x32_selector_core_widget( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size ) ;
	
	virtual inline const string& get_level_element_gfx_file_name() const
	{
		return level_element_gfx_file_name;
	}
	
	virtual inline u32 get_slot_inner_width() const
	{
		return slot_inner_width;
	}
	virtual inline u32 get_slot_inner_height() const
	{
		return slot_inner_height;
	}
	
	virtual inline u32 get_slot_outer_width() const
	{
		return slot_outer_width;
	}
	virtual inline u32 get_slot_outer_height() const
	{
		return slot_outer_height;
	}
	
protected:		// stuffs
	friend class sprite_16x32_selector_widget;
	
	
};


#endif		// sprite_16x32_selector_core_widget_class_hpp
