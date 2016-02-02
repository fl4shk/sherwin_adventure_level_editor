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



#ifndef level_editor_widget_class_hpp
#define level_editor_widget_class_hpp

#include "misc_includes.hpp"
#include "sfml_canvas_widget_classes.hpp"
#include "level_editor_core_widget_class.hpp"
#include "block_selector_widget_class.hpp"
#include "sprite_16x16_selector_widget_class.hpp"
#include "sprite_16x32_selector_widget_class.hpp"


class level_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	vector<string>* argv_copy;
	
	
	static const QString default_parent_title;
	
	QWidget* parent;
	
	QHBoxLayout* hbox_layout;
	//QVBoxLayout* vbox_layout;
	
	
	
	
	// This QScrollArea is used for the_core_widget
	QScrollArea* scroll_area;
	
	level_editor_core_widget* the_core_widget;
	
	
	
	// Level element selector widgets are placed into
	// level_element_selectors_tab_widget.
	QTabWidget* level_element_selectors_tab_widget;
	
	block_selector_widget* the_block_selector_widget;
	sprite_16x16_selector_widget* the_sprite_16x16_selector_widget;
	sprite_16x32_selector_widget* the_sprite_16x32_selector_widget;
	
	
	
public:		// functions
	level_editor_widget( vector<string>* s_argv_copy, 
		QWidget* s_parent = 0 );
	
	
protected:		// functions
	//void adjust_scroll_bar( QScrollBar* scroll_bar );
	bool zoom_in();
	bool zoom_out();
	
	// Events
	void keyPressEvent( QKeyEvent* event );
	//void mousePressEvent( QMouseEvent* event );
	//void mouseMoveEvent( QMouseEvent* event );
	////void mouseReleaseEvent( QMouseEvent* event );
	//void paintEvent( QPaintEvent* event );
	////void resizeEvent( QResizeEvent* event );
	
	//inline const sf::Vector2i widget_pos_to_scroll_area_coords
	//	( int widget_x, int widget_y ) const
	//{
	//	return sf::Vector2i( widget_x - scroll_area->geometry().x()
	//		+ scroll_area->horizontalScrollBar()->value(),
	//		widget_y - scroll_area->geometry().y()
	//		+ scroll_area->verticalScrollBar()->value() );
	//}
	
	
	inline bool open_level( const string& n_level_file_name )
	{
		the_core_widget->level_file_name = n_level_file_name;
		return open_level();
	}
	
	bool open_level();
	
	
	inline void export_file()
	{
		export_file_as(the_core_widget->level_file_name);
	}
	
	void export_file_as( const string& output_file_name );
	
	
	
protected slots:		// slots.
	void hello();
	void show_horizontal_scroll_bar_stuff();
	void show_vertical_scroll_bar_stuff();
	void show_geometry_stuff();
	
	
public slots:		// slots
	void save_file();
	void save_file_as();
	
};


#endif		// level_editor_widget_class_hpp
