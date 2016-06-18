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



#include "sprite_16x32_selector_widget_class.hpp"

sprite_16x32_selector_widget::sprite_16x32_selector_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: QWidget(s_parent), parent(s_parent)
{
	move(s_position);
	resize(s_size);
	
	the_core_widget = new sprite_16x32_selector_core_widget( this, 
		QPoint(), QSize() );
	
	scroll_area = new QScrollArea(this);
	scroll_area->setWidget(the_core_widget);
	//scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	//scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	
	//grid_layout = new QGridLayout(this);
	//grid_layout->addWidget( the_core_widget, 0, 0 );
	
	//setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	//setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	
	setMinimumSize( QSize( the_core_widget
		->get_level_element_palette_render_texture().getSize().x + 1,
		the_core_widget->get_level_element_palette_render_texture()
		.getSize().y + 1 ) );
	setMaximumSize( QSize( the_core_widget
		->get_level_element_palette_render_texture().getSize().x + 1,
		the_core_widget->get_level_element_palette_render_texture()
		.getSize().y + 1 ) );
}


