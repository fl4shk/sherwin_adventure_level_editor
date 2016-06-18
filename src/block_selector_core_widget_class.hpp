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



#ifndef block_selector_core_widget_class_hpp
#define block_selector_core_widget_class_hpp

//#include "sfml_canvas_widget_classes.hpp"
//#include "block_selector_widget_class.hpp"
//#include "block_stuff.hpp"

#include "level_element_selector_core_widget_base_class.hpp" 
#include "block_stuff.hpp"

class block_selector_core_widget 
	: public level_element_selector_core_widget_base
{
public:		// functions
	block_selector_core_widget( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size ) ;
	
protected:		// stuffs
	virtual bool level_element_index_is_valid( u32 level_element_index )
		const;
	
	friend class block_selector_widget;
	
	
};


#endif		// block_selector_core_widget_class_hpp
