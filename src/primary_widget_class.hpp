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
//#include "level_editor_widget_class.hpp"
#include "editing_manager_class.hpp"
#include "mouse_mode_button_group_widget_class.hpp"


#define list_of_debug_tbt_suffixes_part_1(macro) \
	macro(laugh)

#define list_of_mm_switching_tbt_suffixes(macro) \
	macro(draw_mode) macro(erase_mode) \
	macro(sprite_properties_mode) macro(rect_selection_mode)


#define list_of_tbt_suffixes(macro) \
	list_of_debug_tbt_suffixes_part_1(macro) \
	list_of_mm_switching_tbt_suffixes(macro)

enum mm_toolbar_button_type
{
	#define mm_tbt_suffix_to_enum_value(suffix) \
	mm_tbt_##suffix,
	
	list_of_mm_switching_tbt_suffixes(mm_tbt_suffix_to_enum_value)
	
	#undef mm_tbt_suffix_to_enum_value
	
	
	// mm_tbt_count, the number of mm_toolbar_button_type's, is
	// automatically updated by the compiler so long as it is the last
	// entry in this enum.
	mm_tbt_count,
};

enum toolbar_button_type
{
	#define tbt_suffix_to_enum_value(suffix) \
	tbt_##suffix,
	
	list_of_tbt_suffixes(tbt_suffix_to_enum_value)
	
	#undef tbt_suffix_to_enum_value
	
	
	// tbt_count, the number of toolbar_button_type's, is automatically
	// updated by the compiler so long as it is the last entry in this
	// enum.
	tbt_count,
};


class primary_widget : public QMainWindow
{
	Q_OBJECT
	
public:		// constants
	// Toolbar Stuff
	//static const QString laugh_icon_file_name, draw_mode_icon_file_name,
	//	erase_mode_icon_file_name, sprite_properties_mode_icon_file_name, 
	//	rect_selection_mode_icon_file_name;
	static const vector<QString> toolbar_icon_file_name_vec;
	static const vector<QString> toolbar_action_text_vec;
	
	int test;
	
	
protected:		// variables
	vector<string> argv_copy;
	
	// Menu Stuff
	QMenu* file_menu, * edit_menu, * level_menu;
	
	// File Menu Actions
	unique_ptr<QAction> file_menu_laugh_action, file_menu_open_action, 
		file_menu_save_action, file_menu_save_as_action, 
		
		file_menu_quit_action;
	
	// Edit Menu Actions
	unique_ptr<QAction> edit_menu_copy_rs_contents_action,
		edit_menu_paste_copied_rs_contents_action, edit_menu_undo_action,
		edit_menu_redo_action;
	
	// Level Menu Actions
	unique_ptr<QAction> level_menu_sublevel_properties_action,
		//level_menu_level_properties_action, 
		level_menu_export_action;
	
	
	// Toolbar Stuff
	QToolBar* toolbar;
	
	
	QAction* laugh_tool_button_action;
	
	// These are used with the_mouse_mode_button_group_widget.
	//QAction * draw_mode_tool_button_action,
	//	* erase_mode_tool_button_action,
	//	* sprite_properties_mode_tool_button_action,
	//	* rect_selection_mode_tool_button_action;
	vector<QAction*> mm_switching_tool_button_action_vec;
	
	unique_ptr<mouse_mode_button_group_widget> 
		the_mouse_mode_button_group_widget;
	
	
	//// The TEMPORARY central widget.
	//grid_widget* the_grid_widget;
	
	// The central widget.
	unique_ptr<level_editor_widget> the_central_widget;
	
	
	// The editing manager
	unique_ptr<editing_manager> the_editing_manager;
	
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
	
	// File Menu Slots
	void file_menu_laugh();
	void file_menu_quit();
	
	
	// Edit Menu Slots
	void edit_menu_copy_rs_contents();
	void edit_menu_paste_copied_rs_contents();
	void edit_menu_undo();
	void edit_menu_redo();
	
	
	// Level Menu Slots
	void level_menu_export();
};


#endif		// primary_widget_class_hpp
