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
#include "sublevel_properties_widget_class.hpp"


#include "undo_and_redo_stack_class.hpp"

//#include "editing_manager_class.hpp"

class editing_manager;

class level_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	vector<string>* argv_copy;
	
	
	static const QString default_parent_title;
	
	QWidget* parent;
	
	unique_ptr<QHBoxLayout> hbox_layout;
	//unique_ptr<QVBoxLayout> vbox_layout;
	
	unique_ptr<QSplitter> horiz_splitter, vert_splitter;
	
	
	unique_ptr<QTabWidget> core_widgets_tab_widget;
	
	//// This QScrollArea is used for the_core_widget.
	//QScrollArea* core_widget_scroll_area;
	//
	//// Much of the "magic" happens within the_core_widget.
	//level_editor_core_widget* the_core_widget;
	
	vector< unique_ptr<QScrollArea> > core_widget_scroll_area_vec;
	vector< unique_ptr<level_editor_core_widget> > the_core_widget_vec;
	
	
	
	// This variable has been moved from the level_editor_core_widget
	// class, because more than one instance of the class will be made.
	level the_level;
	
	
	
	// Level element selector widgets are placed into
	// level_element_selectors_tab_widget.
	unique_ptr<QTabWidget> level_element_selectors_tab_widget;
	
	unique_ptr<block_selector_widget> the_block_selector_widget;
	unique_ptr<sprite_16x16_selector_widget> 
		the_sprite_16x16_selector_widget;
	unique_ptr<sprite_16x32_selector_widget> 
		the_sprite_16x32_selector_widget;
	
	
	//bool sprite_properties_widget_enabled;
	
	//vector<u32> sprite_properties_widget_enabled_vec;
	//vector<sprite_ipgws> selected_sprite_ipgws_before_modification_vec;
	vector<sprite_pw_extras> sprite_pw_extras_vec;
	
	//sprite_properties_widget* the_sprite_properties_widget;
	unique_ptr<sprite_properties_widget> the_sprite_properties_widget;
	
	unique_ptr<sublevel_properties_widget> the_sublevel_properties_widget;
	
	
	editing_manager* the_editing_manager;
	
	
public:		// functions
	level_editor_widget( editing_manager* s_the_editing_manager, 
		vector<string>* s_argv_copy, QWidget* s_parent = 0 );
	
	inline s32 get_curr_level_editor_core_widget_index()
	{
		for ( u32 i=0; i<the_core_widget_vec.size(); ++i )
		{
			if ( core_widgets_tab_widget->currentWidget()
				== core_widget_scroll_area_vec.at(i).get() )
			{
				return i;
			}
		}
		
		return -1;
	}
	
	inline sublevel& get_curr_sublevel()
	{
		return the_level.sublevel_vec
			.at(get_curr_level_editor_core_widget_index());
	}
	
	inline level_editor_core_widget* get_curr_level_editor_core_widget()
	{
		return the_core_widget_vec
			.at(get_curr_level_editor_core_widget_index()).get();
	}
	
	inline level_editor_core_widget* get_curr_level_editor_core_widget
		( const string& calling_func_name )
	{
		s32 curr_tab_index = get_curr_level_editor_core_widget_index();
		
		level_editor_core_widget* the_core_widget = NULL;
		
		if ( curr_tab_index != -1 )
		{
			the_core_widget = the_core_widget_vec.at(curr_tab_index).get();
		}
		
		if ( the_core_widget == NULL )
		{
			cout << "Weird bug in " << calling_func_name << "():  "
				<< "the_core_widget == NULL.\nExpect a segfault....";
		}
		
		return the_core_widget;
	}
	
	
	
protected:		// functions
	void init_level_element_selectors_tab_widget();
	void init_tab_stuff_for_core_widgets();
	void init_splitters_and_hbox_layout();
	
	
	//void modify_sublevel_size_2d_changer_widget_at_tab_switch();
	void modify_sprite_properties_widget_at_tab_switch();
	
	
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
	//inline bool open_level_core_func()
	//{
	//	return open_level_core_func(the_core_widget_vec.front()
	//		->level_file_name);
	//}
	
	bool open_level_core_func( const string& n_level_file_name );
	
	
	inline void save_level_core_func()
	{
		save_level_as_core_func(the_core_widget_vec.front()
			->level_file_name);
	}
	
	void save_level_as_core_func( const string& output_file_name );
	
	//inline void export_source_core_func()
	//{
	//	export_source_as_core_func(the_core_widget_vec.front()
	//		->level_file_name);
	//}
	
	void export_source_as_core_func( const string& output_file_name );
	
	
	void switch_mouse_mode_shared_code( const mouse_mode& n_the_mouse_mode,
		const string& mouse_mode_suffix );
	void switch_mouse_mode_to_place_level_elements();
	void switch_mouse_mode_to_erase_level_elements();
	void switch_mouse_mode_to_select_single_sprite();
	void switch_mouse_mode_to_rect_selection();
	
	
	void resize_the_sublevel_part_2
		( level_editor_core_widget* the_core_widget, 
		sublevel* the_sublevel,
		const sublevel& the_sublevel_before_modification,
		bool block_grid_was_enabled, u32 prev_scale_factor,
		const rect_selection_stuff& copy_of_the_rect_selection_stuff, 
		bool notify_the_editing_manager );
	
protected slots:		// slots
	void show_sprite_properties_widget();
	void hide_sprite_properties_widget();
	
	void handle_tab_switch( int n_index );
	
	
	
	
	void hello();
	void show_horizontal_scroll_bar_stuff();
	void show_vertical_scroll_bar_stuff();
	void show_geometry_stuff();
	
	
	
public slots:		// slots
	void create_sublevel_properties_widget();
	
	
	void open_level();
	
	void save_level();
	void save_level_as();
	
	void export_source();
	void export_source_as();
	
	
	// the_central_widget->the_mouse_mode_button_group_widget is connected
	// to this slot.
	void switch_mouse_mode( int button_id );
	
	
public:		// friend classes
	friend class editing_manager;
	
};


#endif		// level_editor_widget_class_hpp
