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


class primary_widget : public QMainWindow
{
	Q_OBJECT
	
	
protected:		// variables
	vector<string> argv_copy;
	
	// Menu Stuff
	QMenu* file_menu, * second_menu;
	
	QAction* menu_laugh_action, * menu_save_action, * menu_save_as_action, 
		* menu_quit_action;
	
	// Toolbar Stuff
	static const QString laugh_icon_file_name, quit_icon_file_name;
	QToolBar* toolbar;
	QAction* toolbar_laugh_action, * toolbar_quit_action;
	
	//// The TEMPORARY central widget.
	//grid_widget* the_grid_widget;
	
	// The central widget.
	level_editor_widget* the_central_widget;
	
	
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
