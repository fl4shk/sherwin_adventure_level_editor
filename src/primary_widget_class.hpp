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



#ifndef primary_widget_class_hpp
#define primary_widget_class_hpp

#include "misc_includes.hpp"
#include "layout_widget_classes.hpp"
#include "level_editor_widget_class.hpp"
#include "mouse_mode_button_group_widget_class.hpp"


class primary_widget : public QMainWindow
{
	Q_OBJECT
	
public:		// constants
	// Toolbar Stuff
	static const QString laugh_icon_file_name, draw_mode_icon_file_name,
		sprite_properties_mode_icon_file_name, 
		rect_selection_mode_icon_file_name;
	
	int test;
	
	
protected:		// variables
	vector<string> argv_copy;
	
	// Menu Stuff
	QMenu* file_menu, * edit_menu;
	
	// File Menu Actions
	unique_ptr<QAction> file_menu_laugh_action, file_menu_open_action, 
		file_menu_save_action, file_menu_save_as_action, 
		file_menu_quit_action;
	
	// Edit Menu Actions
	unique_ptr<QAction> edit_menu_sublevel_properties_action,
		edit_menu_level_properties_action;
	
	
	// Toolbar Stuff
	QToolBar* toolbar;
	QAction* toolbar_laugh_action;
	
	
	// These are used with the_mouse_mode_button_group_widget.
	//unique_ptr<QAction> draw_mode_tool_button_action,
	//	sprite_properties_mode_tool_button_action,
	//	rect_selection_mode_tool_button_action;
	QAction * draw_mode_tool_button_action,
		* sprite_properties_mode_tool_button_action,
		* rect_selection_mode_tool_button_action;
	
	unique_ptr<mouse_mode_button_group_widget> 
		the_mouse_mode_button_group_widget;
	
	
	//// The TEMPORARY central widget.
	//grid_widget* the_grid_widget;
	
	// The central widget.
	unique_ptr<level_editor_widget> the_central_widget;
	
	
public:		// functions
	primary_widget( vector<string>& s_argv_copy, QWidget* parent = 0 );
	
	
protected:		// functions
	void generate_menus();
	bool generate_toolbar();
	void generate_central_widget();
	
	//inline void keyPressEvent( QKeyEvent* event )
	//{
	//	if ( event->key() == Qt::Key_Escape )
	//	{
	//		quit_non_slot();
	//	}
	//}
	inline void closeEvent( QCloseEvent* event )
	{
		show_quit_message();
	}
	
	//inline void mouseMoveEvent( QMouseEvent* event )
	//{
	//	the_central_widget->mouseMoveEvent(event);
	//}
	
protected slots:		// slots
	void laugh();
	void quit();
	
	
};


#endif		// primary_widget_class_hpp
