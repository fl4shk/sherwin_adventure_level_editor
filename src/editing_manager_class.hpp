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

#ifndef editing_manager_class_hpp
#define editing_manager_class_hpp

#include "level_editor_widget_class.hpp"


// This class is supposed to help manage editing levels, including undo and
// redo stuff.  Editing actions are supposed to go through this class.
// Therefore, level_editor_widget, level_editor_core_widget, and
// sfml_canvas_widget will contain a pointer to this class.
class editing_manager
{
protected:		// variables
	
	// Pointers to the objects that use an instance of this.
	level_editor_widget* the_level_editor_widget;
	//vector< unique_ptr<level_editor_widget> >* the_core_widget_vec;
	
	
public:		// functions
	inline editing_manager() : the_level_editor_widget(NULL)
	{
	}
	
	inline void set_the_level_editor_widget
		( level_editor_widget* n_the_level_editor_widget )
	{
		the_level_editor_widget = n_the_level_editor_widget;
	}
	inline level_editor_widget* get_the_level_editor_widget()
	{
		return the_level_editor_widget;
	}
	
	
	// Events from other classes
	
	// level_editor_widget
	void key_press_event( QKeyEvent* event );
	
	// level_editor_core_widget
	void mouse_press_event( level_editor_core_widget* the_core_widget, 
		QMouseEvent* event );
	void mouse_move_event( level_editor_core_widget* the_core_widget, 
		QMouseEvent* event );
	void mouse_release_event( level_editor_core_widget* the_core_widget, 
		QMouseEvent* event );
	
	
protected:		// functions
	inline vector< unique_ptr<level_editor_core_widget> >&
		get_the_core_widget_vec()
	{
		return the_level_editor_widget->the_core_widget_vec;
	}
	inline unique_ptr<level_editor_sfml_canvas_widget>&
		get_the_sfml_canvas_widget
		( unique_ptr<level_editor_core_widget>& the_core_widget )
	{
		return the_core_widget->the_sfml_canvas_widget;
	}
	
	void copy_selection_contents();
	void paste_copied_selection_contents();
	
	
	friend class level_editor_widget;
	friend class level_editor_core_widget;
	friend class level_editor_sfml_canvas_widget;
	
};



#endif		// editing_manager_class_hpp