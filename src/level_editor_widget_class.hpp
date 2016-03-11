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

#include "sprite_properties_widget_class.hpp"


class level_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	vector<string>* argv_copy;
	
	
	static const QString default_parent_title;
	
	QWidget* parent;
	
	QHBoxLayout* hbox_layout;
	//QVBoxLayout* vbox_layout;
	
	QSplitter* horiz_splitter, * vert_splitter;
	
	
	
	// This QScrollArea is used for the_core_widget.
	QScrollArea* core_widget_scroll_area;
	
	// Much of the "magic" happens within the_core_widget.
	level_editor_core_widget* the_core_widget;
	
	// This variable has been moved from the level_editor_core_widget
	// class, because more than one instance of the class will be made.
	level the_level;
	
	
	
	// Level element selector widgets are placed into
	// level_element_selectors_tab_widget.
	QTabWidget* level_element_selectors_tab_widget;
	
	block_selector_widget* the_block_selector_widget;
	sprite_16x16_selector_widget* the_sprite_16x16_selector_widget;
	sprite_16x32_selector_widget* the_sprite_16x32_selector_widget;
	
	
	bool sprite_properties_widget_enabled;
	
	//sprite_properties_widget* the_sprite_properties_widget;
	unique_ptr<sprite_properties_widget> the_sprite_properties_widget;
	
	
public:		// functions
	level_editor_widget( vector<string>* s_argv_copy, 
		QWidget* s_parent = 0 );
	
	
protected:		// functions
	//void adjust_scroll_bar( QScrollBar* scroll_bar );
	//bool zoom_in();
	//bool zoom_out();
	
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
	
	
	// Functions used by the slots
	inline bool open_level_core_func()
	{
		return open_level_core_func(the_core_widget->level_file_name);
	}
	
	bool open_level_core_func( const string& n_level_file_name );
	
	
	inline void save_level_core_func()
	{
		save_level_as_core_func(the_core_widget->level_file_name);
	}
	
	void save_level_as_core_func( const string& output_file_name );
	
	//inline void export_source_core_func()
	//{
	//	export_source_as_core_func(the_core_widget->level_file_name);
	//}
	
	void export_source_as_core_func( const string& output_file_name );
	
	
	
protected slots:		// slots
	void show_sprite_properties_widget();
	void hide_sprite_properties_widget();
	
	
	void hello();
	void show_horizontal_scroll_bar_stuff();
	void show_vertical_scroll_bar_stuff();
	void show_geometry_stuff();
	
	
	
public slots:		// slots
	void open_level();
	
	void save_level();
	void save_level_as();
	
	void export_source();
	void export_source_as();
	
};


#endif		// level_editor_widget_class_hpp
